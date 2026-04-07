#include "eil_model.hpp"

namespace eil {

void EmbeddedIntelligenceLayer::reset() {
    last_temperature_c_ = 0.0f;
    has_last_temperature_ = false;
}

float EmbeddedIntelligenceLayer::clamp01(float value) {
    if (value < 0.0f) return 0.0f;
    if (value > 1.0f) return 1.0f;
    return value;
}

OutputFrame EmbeddedIntelligenceLayer::evaluate(const InputFrame& input) {
    OutputFrame out{};

    if (input.cover_open) {
        out.anomaly_score = 1.0f;
        out.advisory_score = 1.0f;
        out.alert_state = AlertState::Fail;
        out.reason_code = ReasonCode::AccessOpen;
        return out;
    }

    // "Trained" portion: simple weighted linear advisory model.
    const float temp_feature = clamp01((input.temperature_c - 25.0f) / 45.0f);
    const float vib_feature = clamp01((input.vibration_g - 0.10f) / 1.90f);
    out.advisory_score = clamp01(0.65f * temp_feature + 0.35f * vib_feature);

    // "Anomaly" portion: compare against simple expected operating region.
    float anomaly = 0.0f;
    if (input.temperature_c > 55.0f) {
        anomaly += clamp01((input.temperature_c - 55.0f) / 20.0f) * 0.60f;
    }
    if (input.vibration_g > 1.20f) {
        anomaly += clamp01((input.vibration_g - 1.20f) / 1.00f) * 0.40f;
    }

    // Trend-based drift detection.
    const float delta_t = has_last_temperature_ ? (input.temperature_c - last_temperature_c_) : 0.0f;
    if (delta_t > 2.0f) {
        anomaly = clamp01(anomaly + 0.20f);
    }
    out.anomaly_score = clamp01(anomaly);

    const float effective_score = (out.advisory_score > out.anomaly_score) ? out.advisory_score : out.anomaly_score;

    if (effective_score >= fail_threshold_) {
        out.alert_state = AlertState::Fail;
    } else if (effective_score >= warn_threshold_) {
        out.alert_state = AlertState::Warning;
    } else {
        out.alert_state = AlertState::Normal;
    }

    if (input.vibration_g > 1.20f) {
        out.reason_code = ReasonCode::VibrationHigh;
    } else if (input.temperature_c > 55.0f || delta_t > 2.0f) {
        out.reason_code = ReasonCode::ThermalDrift;
    } else {
        out.reason_code = ReasonCode::Normal;
    }

    last_temperature_c_ = input.temperature_c;
    has_last_temperature_ = true;
    return out;
}

} // namespace eil

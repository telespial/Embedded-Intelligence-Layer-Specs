#include <iostream>
#include "eil_model.hpp"

static const char* to_string(eil::AlertState state) {
    switch (state) {
        case eil::AlertState::Normal: return "Normal";
        case eil::AlertState::Warning: return "Warning";
        case eil::AlertState::Fail: return "Fail";
        default: return "Unknown";
    }
}

static const char* to_string(eil::ReasonCode reason) {
    switch (reason) {
        case eil::ReasonCode::Normal: return "NORMAL";
        case eil::ReasonCode::ThermalDrift: return "THERMAL_DRIFT";
        case eil::ReasonCode::VibrationHigh: return "VIBRATION_HIGH";
        case eil::ReasonCode::AccessOpen: return "ACCESS_OPEN";
        default: return "UNKNOWN";
    }
}

int main() {
    eil::EmbeddedIntelligenceLayer eil;

    const eil::InputFrame samples[] = {
        {28.0f, 0.20f, false},
        {58.0f, 0.35f, false},
        {59.5f, 1.50f, false},
        {32.0f, 0.10f, true}
    };

    for (const auto& sample : samples) {
        const auto out = eil.evaluate(sample);
        std::cout << "input: temp=" << sample.temperature_c
                  << "C, vib=" << sample.vibration_g
                  << "g, cover_open=" << (sample.cover_open ? "true" : "false") << '\n';
        std::cout << "  anomaly_score=" << out.anomaly_score
                  << ", advisory_score=" << out.advisory_score
                  << ", alert=" << to_string(out.alert_state)
                  << ", reason=" << to_string(out.reason_code) << "\n\n";
    }

    return 0;
}

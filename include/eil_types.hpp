#pragma once

#include <cstdint>

namespace eil {

enum class AlertState : std::uint8_t {
    Normal = 0,
    Warning = 1,
    Fail = 2
};

enum class ReasonCode : std::uint8_t {
    Normal = 0,
    ThermalDrift = 1,
    VibrationHigh = 2,
    AccessOpen = 3
};

struct InputFrame {
    float temperature_c = 0.0f;
    float vibration_g = 0.0f;
    bool cover_open = false;
};

struct OutputFrame {
    float anomaly_score = 0.0f;
    float advisory_score = 0.0f;
    AlertState alert_state = AlertState::Normal;
    ReasonCode reason_code = ReasonCode::Normal;
};

} // namespace eil

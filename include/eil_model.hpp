#pragma once

#include "eil_types.hpp"

namespace eil {

class EmbeddedIntelligenceLayer {
public:
    EmbeddedIntelligenceLayer(float warn_threshold = 0.50f, float fail_threshold = 0.80f)
        : warn_threshold_(warn_threshold), fail_threshold_(fail_threshold) {}

    void reset();
    OutputFrame evaluate(const InputFrame& input);

private:
    float warn_threshold_;
    float fail_threshold_;
    float last_temperature_c_ = 0.0f;
    bool has_last_temperature_ = false;

    static float clamp01(float value);
};

} // namespace eil

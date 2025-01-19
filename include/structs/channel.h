#pragma once

#include "enums/modulation.h"

#include <cstdint>

class Edge;

struct Channel {
    ModulationType modulation;
    bool is_multi_channel {false};
    std::size_t first_slot {0uz};
    std::size_t size {0uz};
    double max_bitrate {0.0};
    double current_bitrate {0.0};
    Edge* assigned_edge {nullptr};
};

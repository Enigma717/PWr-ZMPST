#pragma once

#include "enums/modulation.h"

#include <cstdint>
#include <list>

class Edge;

struct Channel {
    ModulationType modulation;
    bool is_multi_channel {false};
    std::size_t multi_channel_counter {0uz};
    std::size_t first_slot {0uz};
    std::size_t size {0uz};
    double max_bitrate {0.0};
    double current_bitrate {0.0};
    std::list<Edge*> assigned_edges {};
};

#pragma once

#include <cstdint>

struct Demand {
    std::size_t source_id {0};
    std::size_t destination_id {0};
    double bitrate {0.0};
};

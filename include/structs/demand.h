#pragma once

#include <cstdint>
#include <vector>

class Edge;
class Channel;

using Route = std::vector<Edge*>;

struct Demand {
    std::size_t number {0uz};
    std::size_t source_id {0uz};
    std::size_t destination_id {0uz};
    double previous_bitrate {0.0};
    double current_bitrate {0.0};
    Route assigned_route {};
    std::size_t assigned_route_number {1uz};
    Channel* assigned_channel {nullptr};
};

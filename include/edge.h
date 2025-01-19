#pragma once

#include <array>
#include <cstdint>
#include <vector>

class Vertex;
class Channel;

using SpectrumSlots = std::array<bool, 320>;

class Edge {
public:
    Edge() = delete;
    Edge(Vertex& source, Vertex& destination, const std::size_t weight);

// private:
    Vertex* source;
    Vertex* destination;
    std::size_t weight {0uz};
    double bitrate {0.0};
    SpectrumSlots spectrum_slots {};
    std::vector<Channel*> assigned_channels {};
};

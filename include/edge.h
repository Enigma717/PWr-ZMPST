#pragma once

#include <array>
#include <cstdint>

class Vertex;

class Edge {
public:
    Edge() = delete;
    Edge(Vertex& source, Vertex& destination, const std::size_t weight);

// private:
    Vertex* source;
    Vertex* destination;
    std::size_t weight {0uz};
    std::array<bool, 320> spectrum_slots;
};

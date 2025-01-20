#pragma once

#include "vertex.h"
#include "edge.h"

#include <cstdint>
#include <vector>

class Graph {
public:
    Graph() = delete;
    Graph(const std::size_t vertices_count, const std::size_t edges_count);

    std::string print_edges_for_vertices() const;
    void add_edge(
        const std::size_t source_id,
        const std::size_t destination_id,
        const std::size_t weight);

public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
};

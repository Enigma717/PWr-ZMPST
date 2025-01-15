#pragma once

#include "graph.h"
#include "vertex.h"
#include "structs/demand.h"

#include <iomanip>
#include <ostream>
#include <set>
#include <vector>

inline std::ostream& operator<<(std::ostream& stream, const Vertex& vertex)
{
    stream << "(i:" << vertex.get_id() << ")";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Edge& edge)
{
    stream << "(s:" << *edge.source
           << ", d:" << *edge.destination
           << ", w:" << edge.weight << ")";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Demand& demand)
{
    stream << "[s:" << demand.source_id
           << ", d:" << demand.destination_id
           << std::setprecision(10)
           << ", b:" << demand.bitrate << "]";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const std::vector<Vertex>& vertices)
{
    const std::size_t size {vertices.size()};

    for (std::size_t i {0uz}; i < size; i++) {
        const auto& vertex {vertices.at(i)};

        stream << vertex;

        if (i != size - 1)
            stream << ", ";
    }

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const std::vector<Vertex*>& vertices)
{
    const std::size_t size {vertices.size()};

    for (std::size_t i {0uz}; i < size; i++) {
        const auto& vertex {vertices.at(i)};

        stream << *vertex;

        if (i != size - 1)
            stream << ", ";
    }

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const std::vector<Edge>& edges)
{
    const std::size_t size {edges.size()};

    for (std::size_t i {0uz}; i < size; i++) {
        const auto& vertex {edges.at(i)};

        stream << vertex;

        if (i != size - 1)
            stream << ", ";
    }

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const std::set<Vertex*>& neighbours)
{
    for (auto it {neighbours.begin()}; it != neighbours.end(); it++) {

        stream << **it;

        if (it != std::prev(neighbours.end(), 1))
            stream << ", ";
    }

    return stream;
}

inline bool operator<(const Vertex& lhs, const Vertex& rhs)
{
    return lhs.get_id() < rhs.get_id();
}

inline std::ostream& operator<<(std::ostream& stream, const Graph& graph)
{
    stream << graph.vertices;

    return stream;
}

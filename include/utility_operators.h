#pragma once

#include "graph.h"
#include "vertex.h"
#include "structs/channel.h"
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
    stream << std::left
        << "(s:" << *edge.source
        << ", d:" << *edge.destination
        << ", w:" << edge.weight
        << ", b:" << edge.bitrate << ")";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Route& route)
{
    const std::size_t size {route.size()};

    for (std::size_t i {0uz}; i < size; i++) {
        const auto& edge {route[i]};

        stream << "("
            << edge->source->get_id()
            << ", "
            << edge->destination->get_id()
            << ", ["
            << edge->bitrate
            << "])";

        if (i != size - 1)
            stream << " -> ";
    }

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Channel& channel)
{
    stream << "[f: " << channel.first_slot
           << " \ts: " << channel.size
           << std::setprecision(10)
           << " \tm_b: " << channel.max_bitrate
           << " \tc_b: " << channel.current_bitrate
           << " \te: " << channel.assigned_edge << "]";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Demand& demand)
{
    stream << "[s: " << demand.source_id
           << " \td: " << demand.destination_id
           << std::setprecision(10)
           << " \tp_b: " << demand.previous_bitrate
           << " \tc_b: " << demand.current_bitrate
           << " \tch: [" << demand.assigned_channel << "]"
           << " \tr_n: " << demand.assigned_route_number
           << " \tr: [" << demand.assigned_route << "]]";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const SpectrumSlots& slots)
{
    stream << "[";
    for (auto slot : slots)
        stream << slot;
    stream << "]";

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

inline std::ostream& operator<<(std::ostream& stream, const std::vector<Route>& paths)
{
    for (const auto& route : paths)
        stream << "\n|-> [" << route << "]";

    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const std::vector<Demand>& demands)
{
    const std::size_t size {demands.size()};

    for (std::size_t i {0uz}; i < size; i++) {
        const auto& demand {demands.at(i)};

        stream << demand;

        if (i != size - 1)
            stream << "\n";
    }

    return stream;
}

inline bool operator<(const Vertex& lhs, const Vertex& rhs)
{
    return lhs.get_id() < rhs.get_id();
}

inline bool operator>(const Demand& lhs, const Demand& rhs)
{
    return lhs.current_bitrate > rhs.current_bitrate;
}

inline std::ostream& operator<<(std::ostream& stream, const Graph& graph)
{
    stream << graph.vertices;

    return stream;
}

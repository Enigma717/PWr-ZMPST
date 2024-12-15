#include "../include/graph.h"
#include "../include/utility_operators.h"

#include <stack>
#include <queue>
#include <iostream>
#include <sstream>

Graph::Graph(const std::size_t vertices_count, const std::size_t edges_count)
{
    vertices.reserve(vertices_count);
    edges.reserve(edges_count);

    for (std::size_t i {0}; i < vertices_count; i++)
        vertices.push_back(Vertex(i));
}

std::string Graph::print_edges_for_vertices() const
{
    std::stringstream stream;
    const std::size_t size {edges.size()};

    const auto& first_edge {edges.at(0)};
    stream << "Edges for " << *(first_edge.source) << ": [" << first_edge;

    for (std::size_t i {1uz}; i < size; i++) {
        const auto& current_edge {edges.at(i)};
        const auto& previous_edge {edges.at(i-1)};

        if (current_edge.source == previous_edge.source)
            stream << ", ";
        else
            stream << "]\nEdges for " << *(current_edge.source) << ": [";

        stream << current_edge;
    }

    stream << "]";

    return stream.str();
}

void Graph::add_edge(
    const std::size_t source_id,
    const std::size_t destination_id,
    const std::size_t weight)
{
    Vertex& source {vertices.at(source_id)};
    Vertex& destination {vertices.at(destination_id)};

    edges.push_back(Edge(source, destination, weight));
    source.update_neighbourship_with(destination);
}

void Graph::BFS(const std::size_t starting_vertex_id)
{
    std::vector<bool> visited(vertices.size());
    std::queue<Vertex*> queue;

    visited.at(starting_vertex_id) = true;
    queue.push(&vertices.at(starting_vertex_id));

    while (!queue.empty()) {
        const Vertex* current {queue.front()};
        queue.pop();

        std::cout << *current << " -> ";

        for (const auto& neighbour : current->get_neighbours()) {
            const std::size_t neighbour_id {neighbour->get_id()};

            if (!visited.at(neighbour_id)) {
                visited.at(neighbour_id) = true;
                queue.push(neighbour);
            }
        }
    }

    std::cout << "|\n";
}

void Graph::DFS(const std::size_t starting_vertex_id)
{
    std::vector<bool> visited(vertices.size());
    std::stack<Vertex*> stack;

    visited.at(starting_vertex_id) = true;
    stack.push(&vertices.at(starting_vertex_id));

    while (!stack.empty()) {
        const Vertex* current {stack.top()};
        stack.pop();

        std::cout << *current << " -> ";

        for (const auto& neighbour : current->get_neighbours()) {
            const std::size_t neighbour_id {neighbour->get_id()};

            if (!visited.at(neighbour_id)) {
                visited.at(neighbour_id) = true;
                stack.push(neighbour);
            }
        }
    }

    std::cout << "|\n";
}

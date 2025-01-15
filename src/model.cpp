#include "../include/model.h"
#include "../include/graph.h"
#include "../include/utility_operators.h"
#include "../include/structs/demand.h"

#include <algorithm>
#include <iostream>
#include <sstream>

Model::Model()
: loader {*this}, base_graph {nullptr}, solver {*this} {}

void Model::load_file(const std::string& instance_path)
{
    loader.parse_instance(instance_path);
}

void Model::create_base_graph()
{
    base_graph = std::make_unique<Graph>(model_params.vertices, model_params.edges);
}

void Model::add_edge_to_base_graph(
    const std::size_t source_id,
    const std::size_t destination_id,
    const std::size_t weight)
{
    base_graph->add_edge(source_id, destination_id, weight);
}

std::vector<Edge> Model::get_path_between_vertices(
    const std::size_t source_id,
    const std::size_t destination_id)
{
    const auto indexes {
        loader.get_candidate_path(source_id, destination_id, 30)};

    std::vector<Edge> path;
    path.reserve(indexes.size());

    for (const auto index : indexes)
        path.push_back(base_graph->edges[index]);

    return path;
}

Demand Model::get_current_demand(const std::size_t demand_number)
{
    return loader.get_demand(demand_number, 5);
}

std::vector<Edge> Model::get_sorted_path(std::vector<Edge> path, const std::size_t starting_vertex_id) const
{
    std::vector<Edge> sorted_path;
    sorted_path.reserve(path.size());

    const auto& start_edge = std::find_if(
        path.begin(),
        path.end(),
        [=] (const Edge& edge) {return edge.source->get_id() == starting_vertex_id;});

    sorted_path.push_back(*start_edge);

    std::size_t index {0uz};
    while (index < path.size() - 1) {
        for (const auto& edge : path) {
            if (sorted_path.back().destination->get_id() == edge.source->get_id()) {
                sorted_path.push_back(edge);
                index++;
                break;
            }
        }
    }

    return sorted_path;
}

std::string Model::print_model_parms() const
{
    std::stringstream stream;
    stream << "-> Model parameters:"
        << "\n|-> Vertices: " << model_params.vertices
        << "\n|-> Edges: " << model_params.edges
        << "\n|-> Paths file: " << model_params.paths_file
        << "\n|-> Demands dir: " << model_params.demands_dir;

    return stream.str();
}

std::string Model::print_path(std::vector<Edge> path) const
{
    std::stringstream stream;

    const std::size_t size {path.size()};

    for (std::size_t i {0uz}; i < size; i++) {
        const auto& edge {path[i]};

        stream << "("
            << edge.source->get_id()
            << ", "
            << edge.destination->get_id()
            << ")";

        if (i != size - 1)
            stream << " -> ";
    }

    return stream.str();
}

std::size_t Model::solve()
{
    return solver.greedy_solution();
}

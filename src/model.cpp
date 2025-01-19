#include "model.h"
#include "graph.h"
#include "utility_operators.h"
#include "structs/demand.h"

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

Route Model::get_route_between_vertices(
    const std::size_t source_id,
    const std::size_t destination_id,
    const std::size_t path_number)
{
    const auto indexes {
        loader.get_candidate_route(source_id, destination_id, path_number)};

    Route route;
    route.reserve(indexes.size());

    for (const auto index : indexes)
        route.push_back(&base_graph->edges[index]);

    return route;
}

Demand Model::create_demand(const std::size_t demand_number)
{
    return loader.get_demand(demand_number);
}

void Model::update_demand_bitrate(Demand& demand, std::size_t iteration)
{
    loader.update_bitrate(demand, iteration);
}

Route Model::get_sorted_route(const Route& route, const std::size_t starting_vertex_id) const
{
    Route sorted_route;
    sorted_route.reserve(route.size());

    const auto& start_edge {std::find_if(
        route.begin(),
        route.end(),
        [=] (const Edge* edge) { return edge->source->get_id() == starting_vertex_id; })};

    sorted_route.push_back(*start_edge);

    std::size_t index {0uz};
    while (index < route.size() - 1) {
        for (const auto& edge : route) {
            if (sorted_route.back()->destination->get_id() == edge->source->get_id()) {
                sorted_route.push_back(edge);
                index++;
                break;
            }
        }
    }

    return sorted_route;
}

void Model::update_data_on_demand_path(const Demand& demand)
{
    for (auto& edge : demand.assigned_route) {
        auto twin_edge {find_twin_edge(edge)};

        edge->bitrate -= demand.previous_bitrate;
        edge->bitrate += demand.current_bitrate;
        twin_edge->bitrate -= demand.previous_bitrate;
        twin_edge->bitrate += demand.current_bitrate;
    }
}

void Model::remove_demand_from_its_path(const Demand& demand)
{
    for (auto& edge : demand.assigned_route) {
        auto twin_edge {find_twin_edge(edge)};

        edge->bitrate -= demand.current_bitrate;
        twin_edge->bitrate -= demand.current_bitrate;
    }
}

Edge* Model::find_twin_edge(const Edge* const edge_on_path)
{
    return &*std::find_if(
        base_graph->edges.begin(),
        base_graph->edges.end(),
        [=] (const Edge& edge) {
            return edge.source == edge_on_path->destination
                && edge.destination == edge_on_path->source;
            });
}

std::size_t Model::solve()
{
    return solver.greedy_solution();
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

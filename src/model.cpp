#include "model.h"
#include "graph.h"
#include "utility_operators.h"
#include "structs/demand.h"

#include <algorithm>
#include <iostream>
#include <sstream>

Model::Model()
: loader {*this}, base_graph {nullptr}, solver {*this} {}

void Model::load_file(const std::string& instance_path, const std::string& demands_dir)
{
    loader.parse_instance(instance_path, demands_dir);
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

void Model::update_data_in_demand_route(const Demand& demand)
{
    for (auto& edge : demand.assigned_route) {
        edge->bitrate -= demand.previous_bitrate;
        edge->bitrate += demand.current_bitrate;
    }
}

void Model::update_data_in_demand_channel(const Demand& demand)
{
    demand.assigned_channel->current_bitrate -= demand.previous_bitrate;
    demand.assigned_channel->current_bitrate += demand.current_bitrate;
}

void Model::remove_demand_from_its_route(const Demand& demand)
{
    for (auto& edge : demand.assigned_route)
        edge->bitrate -= demand.current_bitrate;
}

double Model::solve(const std::size_t demands_count)
{
    return solver.greedy_solution(demands_count);
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

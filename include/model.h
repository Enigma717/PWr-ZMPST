#pragma once

#include "structs/demand.h"
#include "structs/model_params.h"
#include "loader.h"
#include "solver.h"

#include <cstdint>
#include <memory>
#include <vector>

class Graph;

class Model {
public:
    Model();

    void load_file(const std::string& file_path);
    void create_base_graph();
    void add_edge_to_base_graph(
        const std::size_t source_id,
        const std::size_t destination_id,
        const std::size_t weight);

    Route get_route_between_vertices(
        const std::size_t source_id,
        const std::size_t destination_id,
        const std::size_t path_number);
    Demand create_demand(const std::size_t demand_number);
    void update_demand_bitrate(Demand& demand, std::size_t iteration);

    Route get_sorted_route(
        const Route& route,
        const std::size_t starting_vertex_id) const;
    void update_data_on_demand_path(const Demand& demand);
    void remove_demand_from_its_path(const Demand& demand);
    Edge* find_twin_edge(const Edge* const edge_on_path);

    std::size_t solve();
    std::string print_model_parms() const;

public:
    ModelParams model_params;
    Loader loader;
    std::unique_ptr<Graph> base_graph;
    Solver solver;

    std::size_t simulation_iteration {0uz};

    friend class Loader;
};

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

    void load_file(
        const std::string& instance_path, const std::string& demands_dir);
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

    void update_data_in_demand_route(const Demand& demand);
    void update_data_in_demand_channel(const Demand& demand);
    void remove_demand_from_its_route(const Demand& demand);

    double solve(const std::size_t demands_count);
    std::string print_model_parms() const;

public:
    ModelParams model_params;
    Loader loader;
    std::unique_ptr<Graph> base_graph;
    Solver solver;

    std::size_t simulation_iteration {0uz};

    friend class Loader;
};

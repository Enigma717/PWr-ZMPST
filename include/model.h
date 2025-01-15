#pragma once

#include "structs/model_params.h"
#include "edge.h"
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

    std::vector<Edge> get_path_between_vertices(
        const std::size_t source_id,
        const std::size_t destination_id);
    Demand get_current_demand(const std::size_t demand_number);

    std::vector<Edge> get_sorted_path(
        std::vector<Edge> path,
        const std::size_t starting_vertex_id) const;
    std::string print_path(std::vector<Edge> path) const;
    std::string print_model_parms() const;
    std::size_t solve();

public:
    ModelParams model_params;
    Loader loader;
    std::unique_ptr<Graph> base_graph;
    Solver solver;

    friend class Loader;
};

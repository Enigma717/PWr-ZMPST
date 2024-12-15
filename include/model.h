#pragma once

#include "structs/model_params.h"
#include "loader.h"
#include "solver.h"

#include <cstdint>
#include <memory>

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

    std::string print_model_parms() const;
    std::size_t solve();

public:
    ModelParams model_params;
    Loader loader;
    std::unique_ptr<Graph> base_graph;
    Solver solver;

    friend class Loader;
};

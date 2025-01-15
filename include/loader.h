#pragma once

#include "structs/demand.h"

#include <cstdint>
#include <string>
#include <vector>

class Model;

class Loader {
public:
    Loader() = delete;
    Loader(Model& model_ref);

    void parse_instance(const std::string& instance_path);
    std::vector<std::size_t> get_candidate_path(
        const std::size_t source_id,
        const std::size_t destination_id,
        const std::size_t path_number);
    Demand get_demand(
        const std::size_t demand_number, const std::size_t sim_iteration);

private:
    void update_model_params_strings(std::string& instance_path_copy);
    void parse_header(const std::string& read_line);
    void parse_distances_matrix(const std::string& read_line);

    Model& model_ref;
    std::size_t currently_read_line {0};
};

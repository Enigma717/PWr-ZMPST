#include "loader.h"
#include "model.h"
#include "structs/demand.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    std::size_t header_offset {1uz};
    std::size_t default_route_number {1uz};
    std::size_t possible_paths {30uz};

    std::vector<std::string> tokenize(const std::string& string, const char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream temp(string);
        std::string token;

        while (getline(temp, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
}

Loader::Loader(Model& model_ref) : model_ref{model_ref} {}

void Loader::parse_instance(const std::string& instance_path)
{
    std::string read_line;
    std::ifstream input_stream;
    input_stream.open(instance_path);

    std::cout << "\nInstance path: " << instance_path;

    if(!input_stream.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    std::string instance_path_copy {instance_path};
    update_model_params_strings(instance_path_copy);

    currently_read_line = 0;

    while (getline(input_stream, read_line)) {
        if (currently_read_line < 2)
            parse_header(read_line);
        else
            parse_distances_matrix(read_line);

        currently_read_line++;
    }
}

std::vector<std::size_t> Loader::get_candidate_route(
    const std::size_t source_id,
    const std::size_t destination_id,
    const std::size_t path_number)
{
    std::string read_line;
    std::ifstream input_stream;
    input_stream.open(model_ref.model_params.paths_file);

    if(!input_stream.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    const std::size_t destination_number {
        source_id < destination_id ? destination_id - 1uz : destination_id};
    const std::size_t vertices_multiplier {
        (source_id * (model_ref.model_params.vertices - 1uz)) + destination_number};
    const std::size_t path_target_line {
        header_offset + (possible_paths * vertices_multiplier) + path_number};

    currently_read_line = 0;

    while (currently_read_line != path_target_line && getline(input_stream, read_line))
        currently_read_line++;

    const auto tokens {tokenize(read_line, ' ')};

    std::vector<std::size_t> edges_indexes;
    edges_indexes.reserve(tokens.size());

    for (std::size_t index {0uz}; index < tokens.size(); index++)
    {
        if (tokens[index] == "1")
            edges_indexes.push_back(index);
    }

    return edges_indexes;
}

Demand Loader::get_demand(const std::size_t demand_number)
{
    std::string read_line;
    std::ifstream input_stream;
    std::string demand_path {
        std::format("{}{}.txt", model_ref.model_params.demands_dir, demand_number)};
    input_stream.open(demand_path);

    if(!input_stream.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    currently_read_line = 0;

    std::size_t source_id {0};
    std::size_t destination_id {0};
    double start_bitrate {0.0};

    while (getline(input_stream, read_line)) {
        if (currently_read_line == 0) {
            source_id = std::stoi(read_line);
        }
        else if (currently_read_line == 1) {
            destination_id = std::stoi(read_line);
        }
        else if (currently_read_line == 3) {
            start_bitrate = std::stod(read_line);
            break;
        }

        currently_read_line++;
    }

    const auto assigned_route {
        model_ref.get_route_between_vertices(source_id, destination_id, default_route_number)};

    return {
        demand_number,
        source_id,
        destination_id,
        0.0,
        start_bitrate,
        assigned_route,
        default_route_number,
        nullptr};
}

void Loader::update_bitrate(Demand& demand, const std::size_t sim_iteration)
{
    std::string read_line;
    std::ifstream input_stream;
    std::string demand_path {
        std::format("{}{}.txt", model_ref.model_params.demands_dir, demand.number)};
    input_stream.open(demand_path);

    if(!input_stream.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    currently_read_line = 0;

    while (currently_read_line != sim_iteration + 4 && getline(input_stream, read_line))
        currently_read_line++;

    demand.previous_bitrate = demand.current_bitrate;
    demand.current_bitrate = std::stod(read_line);
}


void Loader::update_model_params_strings(std::string& instance_path_copy)
{
    while (instance_path_copy.back() != '.')
        instance_path_copy.pop_back();
    model_ref.model_params.paths_file = instance_path_copy + "pat";

    while (instance_path_copy.back() != '/')
        instance_path_copy.pop_back();
    model_ref.model_params.demands_dir = instance_path_copy + "demands_2/";
}

void Loader::parse_header(const std::string& read_line)
{
    const auto tokens {tokenize(read_line, ' ')};

    const auto delimiter_pos {read_line.find('\t')};
    const auto value {read_line.substr(delimiter_pos + 1, std::string::npos)};

    switch (currently_read_line) {
    case 0:
        model_ref.model_params.vertices = std::stoi(value);
        model_ref.create_base_graph();
        break;
    case 1:
        model_ref.model_params.edges = std::stoi(value);
        break;
    default: break;
    }
}

void Loader::parse_distances_matrix(const std::string& read_line)
{
    const auto tokens {tokenize(read_line, '\t')};

    const std::size_t source_vertex_id {currently_read_line - 2};
    for (std::size_t destination_vertex_id {0uz}; destination_vertex_id < model_ref.model_params.vertices; destination_vertex_id++) {
        const auto weight {std::stoi(tokens[destination_vertex_id])};

        if (weight != 0)
            model_ref.add_edge_to_base_graph(source_vertex_id, destination_vertex_id, weight);
    }
}

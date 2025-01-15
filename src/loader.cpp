#include "../include/loader.h"
#include "../include/model.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace
{
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

void Loader::parse_instance(const std::string& file_path)
{
    std::string read_line;
    std::ifstream input_stream;
    input_stream.open(file_path);

    std::cout << "\nInstance path: " << file_path;

    if(!input_stream.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    currently_read_line = 0;

    while (getline(input_stream, read_line)) {
        if (currently_read_line < 2)
            parse_header(read_line);
        else
            parse_distances_matrix(read_line);

        currently_read_line++;
    }
}

void Loader::get_candidate_path(
    const std::size_t source_id,
    const std::size_t destination_id,
    const std::size_t path_number)
{
    std::string read_line;
    std::ifstream input_stream;
    input_stream.open("./instances/POL12/pol12.pat");

    if(!input_stream.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    const std::size_t destination_number {source_id < destination_id ? destination_id - 1 : destination_id};
    const std::size_t vertices_multiplier {(source_id * (model_ref.model_params.vertices - 1)) + destination_number};
    const std::size_t path_target_line {1 + (30 * vertices_multiplier) + path_number};

    std::cout << "\nTARGET LINE: " << path_target_line;

    currently_read_line = 0;

    while (currently_read_line != path_target_line && getline(input_stream, read_line))
        currently_read_line++;

    std::cout << "\nREAD LINE: " << read_line;
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

#include "../include/model.h"
#include "../include/graph.h"

#include <sstream>

Model::Model()
: loader {*this}, base_graph {nullptr}, solver {*this} {}

void Model::load_file(const std::string& file_path)
{
    loader.parse_instance(file_path);
}

void Model::get_path_between_vertices(
    const std::size_t source_id,
    const std::size_t destination_id)
{
    loader.get_candidate_path(source_id, destination_id, 30);
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

std::string Model::print_model_parms() const
{
    std::stringstream stream;
    stream << "-> Model parameters:"
        << "\n|-> Vertices: " << model_params.vertices
        << "\n|-> Edges: " << model_params.edges;

    return stream.str();
}

std::size_t Model::solve()
{
    return solver.greedy_solution();
}

#include "./include/model.h"
#include "./include/utility_operators.h"

#include <iostream>

int main(int argc, char* argv[])
{
    Model model;

    model.load_file("./instances/POL12/pol12.net");
    // model.load_file("./instances/US26/us26.net");

    std::cout << "\n\n========[ Instance ]========\n\n";
    std::cout << model.print_model_parms();

    std::cout << "\n\n========[ Neighbours ]========\n\n";
    for (const auto& vertex : model.base_graph->vertices)
    {
        std::cout << vertex.print_neighbours();
    }

    std::cout << "\n\n========[ Edges ]========\n\n";
    const auto& edges {model.base_graph->edges};
    std::cout << edges;

    std::cout << "\n\n==============================\n\n";
    std::cout << model.base_graph->print_edges_for_vertices();

    std::cout << "\n\n========[ BFS(0) ]========\n\n";
    model.base_graph->BFS(0);

    std::cout << "\n\n========[ DFS(0) ]========\n\n";
    model.base_graph->DFS(0);

    std::cout << "\n\n==============================\n\n";

    return 0;
}

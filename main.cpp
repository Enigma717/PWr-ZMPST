#include "model.h"
#include "utility_operators.h"

#include <algorithm>
#include <iostream>

int main(int argc, char* argv[])
{
    Model model;

    std::string instance_path {"./instances/POL12/pol12.net"};
    // std::string instance_path {"./instances/US26/us26.net"};
    model.load_file(instance_path);

    std::cout << "\n\n========[ Instance ]========\n\n";
    std::cout << model.print_model_parms();

    std::cout << "\n\n========[ Neighbours ]========\n\n";
    for (const auto& vertex : model.base_graph->vertices)
        std::cout << vertex.print_neighbours();

    std::cout << "\n\n========[ Edges ]========\n\n";
    const auto& edges {model.base_graph->edges};
    std::cout << edges;

    model.solve();

    std::cout << "\n\n==============================\n\n";
    std::cout << model.base_graph->print_edges_for_vertices();

    std::cout << "\n\n==============================\n\n";

    return 0;
}

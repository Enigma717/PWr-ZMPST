#include "model.h"
#include "utility_operators.h"

#include <algorithm>
#include <iostream>

int main(int argc, char* argv[])
{
    Model model;

    if (argc < 4) {
        std::cerr << "\nMissing arguments:";
        std::cerr << "\nProper usage: ./zmpst <instance type [POL/US]> <demands directory [0-9]> <number of demands [1-500]>\n\n";

        return 1;
    }

    std::string instance_path {};

    if (std::string(argv[1]) == "POL") {
        instance_path = "./instances/POL12/pol12.net";
    }
    else if (std::string(argv[1]) == "US") {
        instance_path = "./instances/US26/us26.net";
    }
    else {
        std::cerr << "\nInstance type is invalid, proper values are \"POL\" or \"US\"";
        return 1;
    }


    std::string demands_dir {};

    if (std::stoi(argv[2]) < 0 && std::stoi(argv[2]) > 9) {
        std::cerr << "\nDemands directory number is invalid, proper values are [0-9]";
        return 1;
    }
    else {
        demands_dir = std::string(argv[2]);
    }

    std::size_t demands_count {static_cast<std::size_t>(std::atoi(argv[3]))};

    if (demands_count < 1 && demands_count > 500) {
        std::cerr << "\nNumber of demands is invalid, proper values are [1-500]";
        return 1;
    }


    model.load_file(instance_path, demands_dir);

    std::cout << "\n\n========[ Instance ]========\n\n";
    std::cout << model.print_model_parms();

    double result {model.solve(demands_count)};

    std::cout << "\n\nFinal result: " << result << " used transceivers on average\n\n";


    return 0;
}

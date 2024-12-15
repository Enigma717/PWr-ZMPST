#pragma once

#include <cstdint>

class Model;

class Solver {
public:
    Solver() = delete;
    Solver(Model& model_ref);

    std::size_t greedy_solution() const;

private:
    Model& model_ref;
};

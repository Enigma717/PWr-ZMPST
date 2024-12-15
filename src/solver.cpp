#include "../include/solver.h"
#include "../include/model.h"

Solver::Solver(Model& model_ref) : model_ref{model_ref} {}

std::size_t Solver::greedy_solution() const
{
    return 0;
}

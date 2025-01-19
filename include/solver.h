#pragma once

#include "structs/channel.h"
#include "structs/demand.h"

#include <cstdint>
#include <vector>

class Model;

class Solver {
public:
    Solver() = delete;
    Solver(Model& model_ref);

    std::size_t greedy_solution();
    std::vector<Demand> create_demands(const std::size_t demands_count) const;
    void create_or_update_channel(Demand& demand);
    Channel deduce_best_channel_type(Demand& demand) const;
    int find_first_free_slot(
        const Route& route, const std::size_t channel_size) const;


    std::vector<Channel> channels;

private:
    Model& model_ref;
};

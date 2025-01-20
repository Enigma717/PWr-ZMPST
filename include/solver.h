#pragma once

#include "structs/channel.h"
#include "structs/demand.h"

#include <cstdint>
#include <list>
#include <set>
#include <vector>

class Model;

class Solver {
public:
    Solver() = delete;
    Solver(Model& model_ref);

    double greedy_solution(const std::size_t demands_count);

private:
    std::vector<Demand> create_demands(const std::size_t demands_count) const;
    void assign_channel(Demand& demand);
    std::set<Channel*> check_for_grooming(Demand& demand) const;
    Channel create_new_channel(Demand& demand) const;
    void remove_demand_from_channel(Demand& demand);
    void remove_channel(Demand& demand);

    int find_first_free_slot(
        const Route& route, const std::size_t channel_size) const;

    std::list<Channel> channels;
    Model& model_ref;
};

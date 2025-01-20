#include "model.h"
#include "solver.h"
#include "utility_operators.h"
#include "structs/channel.h"
#include "structs/demand.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <iterator>
#include <map>
#include <set>

Solver::Solver(Model& model_ref) : model_ref{model_ref} {}

double Solver::greedy_solution(const std::size_t demands_count)
{
    auto demands {create_demands(demands_count)};
    std::size_t transcievers_count {0uz};
    std::size_t max_iterations {288uz};

    for (auto& demand : demands)
        assign_channel(demand);

    std::cout << "\n\n========[ Simulation start ]========\n";
    for (std::size_t i {1uz}; i < max_iterations; i++) {
        std::cout << "\n-----------[ Iteration: " << i << " ]-----------\n";

        std::vector<Demand*> lower_bitrate_demands {};
        std::vector<Demand*> higher_bitrate_demands {};

        for (auto& demand : demands) {
            model_ref.update_demand_bitrate(demand, i);

            if (demand.current_bitrate > demand.previous_bitrate)
                higher_bitrate_demands.push_back(&demand);
            else
                lower_bitrate_demands.push_back(&demand);
        }

        for (auto demand : lower_bitrate_demands) {
            model_ref.update_data_in_demand_route(*demand);
            model_ref.update_data_in_demand_channel(*demand);
        }

        for (auto demand : higher_bitrate_demands) {
            model_ref.update_data_in_demand_route(*demand);
            model_ref.update_data_in_demand_channel(*demand);

            auto channel {demand->assigned_channel};

            if (channel->current_bitrate > channel->max_bitrate) {
                remove_demand_from_channel(*demand);
                assign_channel(*demand);
            }
        }

        std::sort(demands.begin(), demands.end(), std::greater<Demand&>());

        std::size_t current_transcievers_count {0uz};

        for (const auto& channel : channels)
            current_transcievers_count += 2 * channel.multi_channel_counter;

        std::cout << "Current number of used transceivers: " << current_transcievers_count << "\n";

        transcievers_count += current_transcievers_count;
    }

    return static_cast<double>(transcievers_count) / static_cast<double>(max_iterations);
}

std::vector<Demand> Solver::create_demands(const std::size_t demands_count) const
{
    std::vector<Demand> demands;
    demands.reserve(demands_count);

    for (std::size_t i {0uz}; i < demands_count; i++)
        demands.push_back(model_ref.create_demand(i));

    std::sort(demands.begin(), demands.end(), std::greater<Demand&>());

    for (const auto& demand : demands)
        model_ref.update_data_in_demand_route(demand);

    return demands;
}

void Solver::assign_channel(Demand& demand)
{
    auto possible_channels {check_for_grooming(demand)};

    if (!possible_channels.empty()) {
        const auto best_fit_channel {*std::max_element(
            possible_channels.begin(),
            possible_channels.end(),
            [] (const Channel* const lhs, const Channel* const rhs) {
                return (lhs->max_bitrate - lhs->current_bitrate)
                    > (rhs->max_bitrate - rhs->current_bitrate);
                })};

        demand.assigned_channel = best_fit_channel;
        best_fit_channel->current_bitrate += demand.current_bitrate;
    }
    else {
        channels.emplace_back(create_new_channel(demand));
        demand.assigned_channel = &channels.back();

        for (auto edge : demand.assigned_route) {
            edge->assigned_channels.push_back(demand.assigned_channel);
            demand.assigned_channel->assigned_edges.push_back(edge);
        }
    }
}

std::set<Channel*> Solver::check_for_grooming(Demand& demand) const
{
    std::set<Channel*> possible_channels {};
    std::map<Channel*, std::size_t> channel_continuity {};

    for (const auto edge : demand.assigned_route) {
        if (edge->assigned_channels.empty())
            return {};

        for (const auto channel : edge->assigned_channels) {
            if (channel == demand.assigned_channel)
                continue;

            auto available_bitrate {channel->max_bitrate - channel->current_bitrate};

            if (available_bitrate >= demand.current_bitrate)
                channel_continuity[channel]++;
        }
    }

    for (const auto [channel, count] : channel_continuity)
        if (count == demand.assigned_route.size())
            possible_channels.insert(channel);

    return possible_channels;
}

Channel Solver::create_new_channel(Demand& demand) const
{
    ModulationType channel_modulation {};
    bool is_multi_channel_needed {false};
    std::size_t multi_channel_counter {1uz};
    std::size_t channel_first_slot {0z};
    std::size_t channel_size {0uz};
    double channel_max_bitrate {0.0};

    bool is_channel_created {false};

    while (!is_channel_created) {
        const auto route {demand.assigned_route};
        const auto total_route_distance {
            std::accumulate(
                route.begin(),
                route.end(),
                0uz,
                [] (const std::size_t sum, const Edge* edge) { return sum + edge->weight; })};

        if (total_route_distance <= 200uz) {
            channel_modulation = ModulationType::QAM32;
            is_multi_channel_needed = false;
            channel_size = 9uz;
            channel_max_bitrate = 800.0;
        }
        else if (total_route_distance <= 1600uz) {
            channel_modulation = ModulationType::QAM16;
            is_multi_channel_needed = false;
            channel_size = 9uz;
            channel_max_bitrate = 600.0;
        }
        else if (total_route_distance > 1600uz) {
            channel_modulation = ModulationType::QAM8;

            if (demand.current_bitrate > 400.0) {
                is_multi_channel_needed = true;
                multi_channel_counter = static_cast<std::size_t>(demand.current_bitrate / 400.0) + 1uz;
            }
            else {
                is_multi_channel_needed = false;
                multi_channel_counter = 1uz;
            }

            channel_size = multi_channel_counter * 9uz;
            channel_max_bitrate = multi_channel_counter * 400.0;
        }

        const auto start_slot {find_first_free_slot(route, channel_size)};

        if (start_slot < 0) {
            model_ref.remove_demand_from_its_route(demand);

            demand.assigned_route_number = (demand.assigned_route_number + 1) <= 30 ? (demand.assigned_route_number + 1) : 1uz;
            demand.assigned_route = model_ref.get_route_between_vertices(
                demand.source_id, demand.destination_id, demand.assigned_route_number);

            model_ref.update_data_in_demand_route(demand);
        }
        else {
            channel_first_slot = start_slot;
            is_channel_created = true;

            for (auto edge : route)
                std::fill(
                    edge->spectrum_slots.begin() + channel_first_slot,
                    edge->spectrum_slots.begin() + channel_first_slot + channel_size,
                    true);
        }
    }

    return {
        channel_modulation,
        is_multi_channel_needed,
        multi_channel_counter,
        channel_first_slot,
        channel_size,
        channel_max_bitrate,
        demand.current_bitrate,
        {}};
}

int Solver::find_first_free_slot(const Route& route, const std::size_t channel_size) const
{
    const auto first_edge {route[0]};
    auto slots {first_edge->spectrum_slots};

    int first_free_index {-1};

    for (std::size_t index {0uz}; index < slots.size(); index++) {
        auto slot {slots[index]};

        if (slot == true)
            continue;

        if (index + channel_size > slots.size())
            return -1;

        std::vector<bool> taken_channel {};
        bool is_free_space_for_channel {true};

        for (const auto edge : route) {
            const auto edge_slots {edge->spectrum_slots};

            std::vector<bool> candidate_channel(
                edge_slots.begin() + index, edge_slots.begin() + index + channel_size);

            is_free_space_for_channel = std::all_of(
                candidate_channel.begin(),
                candidate_channel.end(),
                [] (bool val) { return val == false; });

            if (!is_free_space_for_channel) {
                taken_channel = candidate_channel;
                break;
            }
        }

        if (is_free_space_for_channel) {
            first_free_index = index;
            break;
        }
        else {
            const auto last_taken_slot {std::find(taken_channel.rbegin(), taken_channel.rend(), true)};
            index += taken_channel.rend() - last_taken_slot - 1;
        }
    }

    return first_free_index;
}

void Solver::remove_demand_from_channel(Demand& demand)
{
    const auto channel {demand.assigned_channel};

    channel->current_bitrate -= demand.current_bitrate;

    if (channel->current_bitrate == 0)
        remove_channel(demand);

    demand.assigned_channel = nullptr;
}

void Solver::remove_channel(Demand& demand)
{
    const auto channel {demand.assigned_channel};

    for (auto& edge : demand.assigned_route)
        std::fill(
            edge->spectrum_slots.begin() + channel->first_slot,
            edge->spectrum_slots.begin() + channel->first_slot + channel->size,
            false);

    channels.remove(*demand.assigned_channel);
}

#include "model.h"
#include "solver.h"
#include "utility_operators.h"
#include "structs/channel.h"
#include "structs/demand.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <iterator>

Solver::Solver(Model& model_ref) : model_ref{model_ref} {}

std::size_t Solver::greedy_solution()
{
    auto demands {create_demands(50uz)};

    std::cout << "\nPATHS WITH APPLIED DEMANDS: [\n";
    for (auto& demand : demands)
        std::cout << demand.assigned_route << "\n";

    std::cout << "]";


    // auto twin_edge1 {model_ref.find_twin_edge(demands[0].assigned_route[0])};
    // demands[0].assigned_route[0]->spectrum_slots[10] = true;
    // demands[0].assigned_route[0]->spectrum_slots[12] = true;
    // demands[0].assigned_route[0]->spectrum_slots[20] = true;

    // twin_edge1->spectrum_slots[10] = true;
    // twin_edge1->spectrum_slots[12] = true;
    // twin_edge1->spectrum_slots[20] = true;

    // auto twin_edge2 {model_ref.find_twin_edge(demands[0].assigned_route[1])};
    // demands[0].assigned_route[1]->spectrum_slots[2] = true;
    // demands[0].assigned_route[1]->spectrum_slots[3] = true;

    // twin_edge2->spectrum_slots[2] = true;
    // twin_edge2->spectrum_slots[3] = true;

    // auto twin_edge3 {model_ref.find_twin_edge(demands[1].assigned_route[0])};
    // demands[1].assigned_route[0]->spectrum_slots[7] = true;
    // demands[1].assigned_route[0]->spectrum_slots[8] = true;
    // demands[1].assigned_route[0]->spectrum_slots[9] = true;

    // twin_edge3->spectrum_slots[7] = true;
    // twin_edge3->spectrum_slots[8] = true;
    // twin_edge3->spectrum_slots[9] = true;

    // auto twin_edge4 {model_ref.find_twin_edge(demands[3].assigned_route[0])};
    // demands[3].assigned_route[0]->spectrum_slots[2] = true;
    // demands[3].assigned_route[0]->spectrum_slots[5] = true;
    // demands[3].assigned_route[0]->spectrum_slots[6] = true;

    // twin_edge4->spectrum_slots[2] = true;
    // twin_edge4->spectrum_slots[5] = true;
    // twin_edge4->spectrum_slots[6] = true;

    // auto twin_edge5 {model_ref.find_twin_edge(demands[4].assigned_route[0])};
    // demands[4].assigned_route[0]->spectrum_slots[7] = true;
    // demands[4].assigned_route[0]->spectrum_slots[14] = true;
    // demands[4].assigned_route[0]->spectrum_slots[21] = true;
    // demands[4].assigned_route[0]->spectrum_slots[28] = true;

    // twin_edge5->spectrum_slots[7] = true;
    // twin_edge5->spectrum_slots[14] = true;
    // twin_edge5->spectrum_slots[21] = true;
    // twin_edge5->spectrum_slots[28] = true;



    std::cout << "\n\n\nSLOTS IN EACH EDGE BEFORE CREATING: [\n";
    for (const auto& edge : model_ref.base_graph->edges)
        std::cout << "Edge " << edge << ": " << edge.spectrum_slots << "\n";
    std::cout << "]\n\n";

    for (auto& demand : demands) {
        model_ref.update_demand_bitrate(demand, 187);
        model_ref.update_data_on_demand_path(demand);
        create_or_update_channel(demand);
    }

    std::cout << "\n\n\nSLOTS IN EACH EDGE AFTER CREATING: [\n";
    for (const auto& edge : model_ref.base_graph->edges)
        std::cout << "Edge " << &edge << ": slots address (" << &edge.spectrum_slots << ") " << edge.spectrum_slots << "\n";
    std::cout << "]\n\n";


    std::cout << "\n\n========[ Update bitrates ]========\n\n";
    for (std::size_t i {1uz}; i < 2uz; i++) {
        std::cout << "\n\n--------[ Iteration: " << i << " ]--------\n\n";

        for (auto& demand : demands) {
            model_ref.update_demand_bitrate(demand, 187);
            model_ref.update_data_on_demand_path(demand);
        }

        std::cout << "\nDEMANDS BEFORE SORTING: [\n" << demands << "]";
        std::sort(demands.begin(), demands.end(), std::greater<Demand&>());
        std::cout << "\nDEMANDS AFTER SORTING: [\n" << demands << "]";

        std::cout << "\n\nExisting channels: [\n";
        for (const auto& channel : channels)
            std::cout << "Channel: [" << channel << "]\n";

        std::cout << "\nPATHS WITH UPDATED DEMANDS: [\n";
        for (const auto& demand : demands)
            std::cout << demand.assigned_route << "\n";
        std::cout << "]\n";

        std::cout << "\n\n\nSLOTS IN EACH EDGE : [\n";
        for (const auto& edge : model_ref.base_graph->edges)
            std::cout << "Edge " << edge << ": " << edge.spectrum_slots << "\n";
        std::cout << "]\n\n";
    }

    // for (auto& demand : demands) {
    //     remove_channel(demand);

    //     std::cout << "\n\n\nExisting channels: [\n";
    //     for (const auto& channel : channels)
    //         std::cout << "Channel: [" << channel << "]\n";

    //     channels.remove(*demand.assigned_channel);

    //     std::cout << "\n\n\nSLOTS IN EACH EDGE AFTER REMOVING: [\n";
    //     for (const auto& edge : model_ref.base_graph->edges)
    //         std::cout << "Edge " << edge << ": slots " << edge.spectrum_slots << "\n";
    //     std::cout << "]\n\n";
    // }

    std::cout << "\n\n\nTOTAL NUMBER OF CHANNELS: " << channels.size();

    return 0uz;
}

std::vector<Demand> Solver::create_demands(const std::size_t demands_count) const
{
    std::cout << "\n\n========[ Demands ]========\n\n";
    std::vector<Demand> demands;
    demands.reserve(demands_count);

    for (std::size_t i {0uz}; i < demands_count; i++)
        demands.push_back(model_ref.create_demand(i));

    std::sort(demands.begin(), demands.end(), std::greater<Demand&>());
    // std::cout << "\nDEMANDS: [\n" << demands << "]";

    for (const auto& demand : demands)
        model_ref.update_data_on_demand_path(demand);

    return demands;
}

void Solver::create_or_update_channel(Demand& demand)
{
    channels.emplace_back(deduce_best_channel_type(demand));
    demand.assigned_channel = &channels.back();

    for (auto edge : demand.assigned_route) {
        edge->assigned_channels.push_back(demand.assigned_channel);
        demand.assigned_channel->assigned_edges.push_back(edge);
    }
}

void Solver::remove_channel(Demand& demand)
{
    const auto channel {demand.assigned_channel};

    for (auto& edge : demand.assigned_route) {
        auto twin_edge {model_ref.find_twin_edge(edge)};

        std::fill(
            edge->spectrum_slots.begin() + channel->first_slot,
            edge->spectrum_slots.begin() + channel->first_slot + channel->size,
            false);

        std::fill(
            twin_edge->spectrum_slots.begin() + channel->first_slot,
            twin_edge->spectrum_slots.begin() + channel->first_slot + channel->size,
            false);
    }

    model_ref.remove_demand_from_its_path(demand);
    channels.remove(*demand.assigned_channel);
}

Channel Solver::deduce_best_channel_type(Demand& demand) const
{

    ModulationType channel_modulation;
    bool is_multi_channel_needed;
    std::size_t channel_size;
    std::size_t channel_first_slot;
    double channel_max_bitrate;

    bool is_channel_created {false};

    while (!is_channel_created) {
        // std::cout << "\nCURRENT DEMAND ROUTE: " << demand.assigned_route << "\n";

        const auto total_route_distance {
            std::accumulate(
                demand.assigned_route.begin(),
                demand.assigned_route.end(),
                0uz,
                [] (const std::size_t sum, const Edge* edge) { return sum + edge->weight; })};

        std::cout << "\n=======================\n\nMAX DISTANCE IN PATH: " << total_route_distance;

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
                std::size_t channels_needed {static_cast<std::size_t>(demand.current_bitrate / 400.0) + 1uz};

                is_multi_channel_needed = true;
                channel_size = channels_needed * 9uz;
                channel_max_bitrate = channels_needed * 400.0;
            }
            else {
                is_multi_channel_needed = false;
                channel_size = 9uz;
                channel_max_bitrate = 400.0;
            }
        }

        const auto start_slot {find_first_free_slot(demand.assigned_route, channel_size)};

        if (start_slot < 0) {
            std::cout << "\n\nCREATE CHANNEL START SLOT NOT FONUD\n\n";
            model_ref.remove_demand_from_its_path(demand);

            demand.assigned_route_number = (demand.assigned_route_number + 1) % 30;
            demand.assigned_route = model_ref.get_route_between_vertices(
                demand.source_id, demand.destination_id, demand.assigned_route_number);

            model_ref.update_data_on_demand_path(demand);
        }
        else {
            channel_first_slot = start_slot;
            std::cout << "\nCHANNEL START SLOT: " << channel_first_slot << "\n";
            is_channel_created = true;
        }
    }


    std::cout << "\n\nDEMAND BITRATE: " << demand.current_bitrate;
    std::cout << "\n\n\nCHANNEL SUCCESSFULLY CREATED: ["
        << "\nmodulation: " << channel_modulation
        << "\nmulti_channel: " << is_multi_channel_needed
        << "\nstart: " << channel_first_slot
        << "\nsize: " << channel_size
        << "\nmax_bitrate: " << channel_max_bitrate  << "]\n";

    return {
        channel_modulation,
        is_multi_channel_needed,
        channel_first_slot,
        channel_size,
        channel_max_bitrate,
        0.0};
}

int Solver::find_first_free_slot(
    const Route& route, const std::size_t channel_size) const
{
    const auto first_edge {route[0]};
    auto slots {first_edge->spectrum_slots};

    int first_free_index {-1};

    for (std::size_t index {0uz}; index < slots.size(); index++) {
        auto slot {slots[index]};

        if (slot == true)
            continue;

        if (index + channel_size > slots.size()) {
            std::cout << "\n\nDUBSTEP REMIX!!!! " << (index + channel_size) << "\n\n";
            return -1;
        }

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

    if (first_free_index >= 0) {
        for (auto edge : route) {
            auto twin_edge {model_ref.find_twin_edge(edge)};

            std::fill(
                edge->spectrum_slots.begin() + first_free_index,
                edge->spectrum_slots.begin() + first_free_index + channel_size,
                true);

            std::fill(
                twin_edge->spectrum_slots.begin() + first_free_index,
                twin_edge->spectrum_slots.begin() + first_free_index + channel_size,
                true);
        }
    }
    else {
        std::cout << "\nNO FREE SLOTS FOUND\n";
    }


    return first_free_index;
}

#pragma once

#include <cstdint>
#include <set>
#include <string>

class Vertex {
public:
    Vertex() = delete;
    Vertex(const std::size_t);

    std::string print_neighbours() const;
    std::size_t get_id() const;
    const std::set<Vertex*>& get_neighbours() const;
    void update_neighbourship_with(Vertex& new_neighbour);

// private:
    std::size_t id {0};
    std::set<Vertex*> neighbours;
};

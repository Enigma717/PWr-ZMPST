#include "vertex.h"
#include "utility_operators.h"

#include <sstream>

Vertex::Vertex(const std::size_t id) : id{id} {};

std::string Vertex::print_neighbours() const
{
    std::stringstream stream;
    stream << "\nNeighbourhood(" << *this << "): [" << neighbours << "]";

    return stream.str();
}

std::size_t Vertex::get_id() const
{
    return id;
}

const std::set<Vertex*>& Vertex::get_neighbours() const
{
    return neighbours;
}

void Vertex::update_neighbourship_with(Vertex& new_neighbour)
{
    neighbours.insert(&new_neighbour);
}

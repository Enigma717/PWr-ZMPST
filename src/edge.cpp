#include "edge.h"
#include "vertex.h"

#include <iostream>

Edge::Edge(Vertex& source, Vertex& destination, const std::size_t weight)
: source{&source}, destination{&destination}, weight{weight} {};

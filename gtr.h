#ifndef GRAPH_THEORY_RESEARCH_H
#define GRAPH_THEORY_RESEARCH_H

// use boost graph library for graph manipulation
// use graphviz for graph visualization
#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include <boost/graph/graphviz.hpp>
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_color_t, default_color_type>> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::pair<int, int> Edge;

#endif
#include "gtr.h"
#ifndef GRAPH_PROCESSOR_H
#define GRAPH_PROCESSOR_H

void dfs(const Vertex& v_i, Vertex& special_vertex, std::set<Vertex>& seen, Graph& graph, std::vector<Vertex>& newCompleteGraph);
std::vector<std::vector<Vertex> > find_adjacent_complete_graphs(const Vertex& special_vertex, const Graph& graph);
Graph find_original_graph(Graph& coloring_graph, Vertex special_vertex);
std::vector<Vertex> find_special_vertices_in_coloring(const Graph& coloring_graph);
long double simulate_random_walk(const Graph& graph, int number_of_simulations, std::vector<int>& special_vertices_vector);

#endif
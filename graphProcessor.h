#include "gtr.h"
#ifndef GRAPH_PROCESSOR_H
#define GRAPH_PROCESSOR_H

void dfs(const Vertex& v_i, Vertex& special_vertex, std::set<Vertex>& seen, Graph& graph, std::vector<Vertex>& newCompleteGraph);
std::vector<std::vector<Vertex> > find_adjacent_complete_graphs(Vertex& special_vertex, Graph& graph);
Graph find_original_graph(Graph& coloring_graph, Vertex special_vertex);

#endif
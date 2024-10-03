#include "gtr.h"
#ifndef graphProcessor
#define graphProcessor

void dfs(const Vertex& v_i, Vertex& special_vertex, std::set<Vertex>& seen, Graph& graph, std::vector<Vertex>& newCompleteGraph);
std::vector<std::vector<Vertex> > find_adjacent_complete_graphs(Vertex& special_vertex, Graph& graph);

#endif
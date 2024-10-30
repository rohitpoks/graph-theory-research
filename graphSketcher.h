#ifndef GRAPH_SKETCHER_H
#define GRAPH_SKETCHER_H

#include "gtr.h"

Graph create_graph();
void print_graph_as_dot(Graph graph, const std::string& graph_name);

#endif
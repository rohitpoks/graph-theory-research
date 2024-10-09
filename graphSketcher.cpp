#include "gtr.h"
#include "graphSketcher.h"

Graph create_graph() {
  Edge special_graph_edges[] = {
    // k_4 edges
    Edge(0, 1),
    Edge(1, 2),
    Edge(2, 0),
    Edge(3, 1),
    Edge(3, 2),
    Edge(3, 0),
    // k_3 edges
    Edge(0, 4),
    Edge(0, 5),
    Edge(4, 5),
    // k_2 edges
    Edge(0, 6),
    // special vertex 0: draw edge to every other vertex
  };

  Graph special_graph(special_graph_edges, special_graph_edges + sizeof(special_graph_edges)/sizeof(Edge), 7);
  return special_graph;
}

void print_graph_as_dot(Graph graph) {
  std::ofstream output_graph_stream;
  output_graph_stream.open("special_example.dot");
  write_graphviz(output_graph_stream, graph);
  output_graph_stream.close();
}
#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"

int main() {
  Graph coloring_graph = create_graph();
  print_graph_as_dot(coloring_graph, "coloring_graph.dot");
  int special_vertex_int = 0;
  Graph original_graph = find_original_graph(coloring_graph, 0);
  print_graph_as_dot(original_graph, "original_graph.dot");
  Vertex special_vertex = vertex(special_vertex_int, coloring_graph);
  std::vector<std::vector<Vertex> > adjacentCompleteGraphs = find_adjacent_complete_graphs(special_vertex, coloring_graph);

  for (auto& complete_graph: adjacentCompleteGraphs) {
    std::cout << "new adjacent complete graph found with size " << complete_graph.size() << '\n';
    for (auto& vertex: complete_graph) {
      std::cout << vertex << " ";
    }

    std::cout << '\n';
  }
  return 0;
}
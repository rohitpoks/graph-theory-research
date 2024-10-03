#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"

int main() {
  Graph graph = create_graph();
  // print_graph_as_dot(special_graph);
  Vertex special_vertex = vertex(0, graph);
  std::vector<std::vector<Vertex> > adjacentCompleteGraphs = find_adjacent_complete_graphs(special_vertex, graph);

  for (auto& complete_graph: adjacentCompleteGraphs) {
    std::cout << "new adjacent complete graph found with size " << complete_graph.size() << '\n';
    for (auto& vertex: complete_graph) {
      std::cout << vertex << " ";
    }

    std::cout << '\n';
  }
  return 0;
}
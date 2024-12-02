#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"
#include "originalToColoring.h"

int main() {
  Graph original_graph = create_graph();
  Graph coloring_graph = coloringFromOriginal(original_graph, 4);
  std::vector<Vertex> special_vertices = find_special_vertices_in_coloring(coloring_graph);
  const auto& color_map = get(boost::vertex_color, coloring_graph);
  for (auto& vertex: special_vertices) {
    put(color_map, vertex, "red");
  }

  boost::dynamic_properties dp;
  dp.property("color", get(vertex_color, coloring_graph));
  dp.property("node_id", get(boost::vertex_index, coloring_graph));
  print_graph_as_graphml(coloring_graph, "coloring_graph.graphml", dp);
  // print_graph_as_dot(coloring_graph, "coloring_graph.dot", dp);
  // print_graph_as_dot(coloring_graph, "coloring_graph.dot");
  return 0;
}
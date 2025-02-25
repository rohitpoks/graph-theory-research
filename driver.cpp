#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"
#include "originalToColoring.h"


static const int NUM_SIMULATIONS = 100000;
int main() {
  Graph original_graph = create_graph();
  int k = 3;
  std::map<int, std::map<int, int> > adj_list;
  std::set<int> special_vertex_classes;
  std::map<int, int> class_to_num_vertices;
  Graph coloring_graph = coloringFromOriginal(original_graph, k, adj_list, special_vertex_classes, class_to_num_vertices);
  for (const auto& u : adj_list) {
    std::cout << u.first << "(" << class_to_num_vertices[u.first] << ")" << ":" << " ";
    for (const auto& v : adj_list[u.first]) {
      std::cout << v.first << ":" << v.second << " ";
    }

    std::cout << '\n';
  }

  std::cout << "Special vertex classes: ";
  for (int special_vertex_class : special_vertex_classes) {
    std::cout << special_vertex_class << " ";
  }

  std::cout << '\n';
  std::vector<Vertex> special_vertices = find_special_vertices_in_coloring(coloring_graph);
  const auto& color_map = get(boost::vertex_color, coloring_graph);
  std::cout << "Number of special:" << special_vertices.size() << '\n';
  std::cout << "Number of total:" << boost::num_vertices(coloring_graph) << '\n';
  for (auto& vertex: special_vertices) {
    put(color_map, vertex, "red");
  }

  // long double path_length = simulate_random_walk(coloring_graph, NUM_SIMULATIONS);
  // std::cout << "average path length is " << path_length << " for number of colors: " << k << std::endl;
  boost::dynamic_properties dp;
  dp.property("color", get(vertex_color, coloring_graph));
  dp.property("node_id", get(boost::vertex_index, coloring_graph));
  print_graph_as_graphml(coloring_graph, "coloring_graph_" + std::to_string(k) + ".graphml", dp);
  return 0;
}
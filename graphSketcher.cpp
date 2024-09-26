// use boost graph library for graph manipulation
// use graphviz for graph visualization
#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include <boost/graph/graphviz.hpp>

int main() {
  typedef std::pair<int, int> Edge;
  Edge k4_edges[] = {
    Edge(0, 1),
    Edge(1, 2),
    Edge(2, 0),
    Edge(3, 1),
    Edge(3, 2),
    Edge(3, 0),
  };

  using namespace boost;
  typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_color_t, default_color_type>> Graph;
  Graph k4(k4_edges, k4_edges + sizeof(k4_edges)/sizeof(Edge), 4);
  typedef graph_traits<Graph>::vertex_descriptor Vertex;
  std::ofstream output_graph_stream;
  output_graph_stream.open("k4_example.dot");
  write_graphviz(output_graph_stream, k4);
  output_graph_stream.close();
  return 0;
}
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

  using namespace boost;
  typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_color_t, default_color_type>> Graph;
  Graph special_graph(special_graph_edges, special_graph_edges + sizeof(special_graph_edges)/sizeof(Edge), 7);
  typedef graph_traits<Graph>::vertex_descriptor Vertex;
  std::ofstream output_graph_stream;
  output_graph_stream.open("special_example.dot");
  write_graphviz(output_graph_stream, special_graph);
  output_graph_stream.close();
  return 0;
}
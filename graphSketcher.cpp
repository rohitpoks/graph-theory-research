#include "gtr.h"
#include "graphSketcher.h"

Graph create_graph() {
  // Edge special_graph_edges[] = {
  //   // k_4 edges
  //   Edge(0, 1),
  //   Edge(1, 2),
  //   Edge(2, 0),
  //   Edge(3, 1),
  //   Edge(3, 2),
  //   Edge(3, 0),
  //   // k_3 edges
  //   Edge(0, 4),
  //   Edge(0, 5),
  //   Edge(4, 5),
  //   // k_2 edges
  //   Edge(0, 6),
  //   // special vertex 0: draw edge to every other vertex
  // };

  // six cycle:
  // Edge special_graph_edges[] = {
  //     Edge(0, 1),
  //     Edge(1, 2),
  //     Edge(2, 3),
  //     Edge(3, 4),
  //     Edge(4, 5),
  //     Edge(5, 0),
  // };

  // three cycle with extra edge:
  // Edge special_graph_edges[] = {
  //     Edge(0, 1),
  //     Edge(1, 2),
  //     Edge(2, 0),
  //     Edge(3, 0),
  // };

  // seven cycle with extra edge:
  // Edge special_graph_edges[] = {
  //   Edge(0, 1),
  //   Edge(1, 2),
  //   Edge(2, 3),
  //   Edge(3, 4),
  //   Edge(4, 5),
  //   Edge(5, 6),
  //   Edge(6, 0),
  //   Edge(0, 7),
  // };

  // six cycle
  // Edge special_graph_edges[] = {
  //   Edge(0, 1),
  //   Edge(1, 2),
  //   Edge(2, 3),
  //   Edge(3, 4),
  //   Edge(4, 5),
  //   Edge(5, 0),
  // };

  // Edge special_graph_edges[] = {
    // Edge(0, 1),
    // Edge(1, 2),
    // Edge(2, 3),
    // Edge(3, 4),
    // Edge(4, 5),
    // Edge(5, 0),
  //   Edge(0,1),
  //   Edge(0,2),
  //   Edge(0,3),
  //   Edge(0,4),
  //   Edge(0,5),

  //   Edge(1,6),
  //   Edge(1,7),

  //   Edge(2,8),
  //   Edge(3,9),
  //   Edge(3,10),

  //   Edge(4,5),
  //   Edge(4,7),
  //   Edge(4,9),

  //   Edge(5,6),
  //   Edge(5,8),
  //   Edge(5,10),
  // };

  // Edge special_graph_edges[] = {
  //   //First square
  //   Edge(1,2),
  //   Edge(1,3),
  //   Edge(2,4),
  //   Edge(3,4),
  //   //Second square
  //   Edge(5,6),
  //   Edge(5,7),
  //   Edge(6,8),
  //   Edge(7,8),
  //   //Square 3
  //   Edge(9,10),
  //   Edge(9,11),
  //   Edge(10,0),
  //   Edge(11,0),
  //   //Connecting the squares
  //   Edge(2,5),
  //   Edge(3,9),
  //   Edge(8,0)
  // };

  // Edge special_graph_edges[] = {
  //   Edge(0, 1),
  // };

  // line graph
  // Edge special_graph_edges[] = {
  //   Edge(0, 1),
  //   Edge(1, 2),
  // };

  // 5 cycle with extra edge
  Edge special_graph_edges[] = {
    Edge(0, 1),
    Edge(1, 2),
    Edge(2, 3),
    Edge(3, 4),
    Edge(4, 0),
    Edge(5, 0),
  };

  // K(2,3)->special
  // Edge special_graph_edges[] = {
  //   Edge(0,2),
  //   Edge(0,3),
  //   Edge(0,4),
  //   Edge(1,2),
  //   Edge(1,3),
  // }; 

  // incomplete k4
  // Edge special_graph_edges[] = {
  //   Edge(0,1),
  //   Edge(1,2),
  //   Edge(2,3),
  //   Edge(0,3),
  //   Edge(1,3),
  // };

  int number_of_vertices = 5;
  Graph special_graph(special_graph_edges, special_graph_edges + sizeof(special_graph_edges)/sizeof(Edge), number_of_vertices);
  return special_graph;
}

void print_graph_as_dot(const Graph& graph, const std::string& graph_name) {
  std::ofstream output_graph_stream;
  output_graph_stream.open(graph_name);
  write_graphviz(output_graph_stream, graph);
  output_graph_stream.close();
}

void print_graph_as_dot(const Graph& graph, const std::string& graph_name, const boost::dynamic_properties& dp) {
  std::ofstream output_graph_stream;
  output_graph_stream.open(graph_name);
  write_graphviz_dp(output_graph_stream, graph, dp);
  output_graph_stream.close();
}

void print_graph_as_graphml(const Graph& graph, const std::string& graph_name, const boost::dynamic_properties& dp) {
  std::ofstream output_graph_stream;
  output_graph_stream.open(graph_name);
  write_graphml(output_graph_stream, graph, dp);
}
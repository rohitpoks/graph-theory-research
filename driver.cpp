#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"
#include "originalToColoring.h"
#include "eigen/Eigen/Dense"


static const int NUM_SIMULATIONS = 100000;
int main() {
  Graph original_graph = create_graph();
  int k = 7;
  std::map<int, std::map<int, int> > adj_list;
  std::set<int> special_vertex_classes;
  std::map<int, int> class_to_num_vertices;
  std::vector<int> special_vertices;
  Graph coloring_graph = coloringFromOriginal(original_graph, k, adj_list, special_vertex_classes, class_to_num_vertices, special_vertices);
  // for (const auto& u : adj_list) {
  //   std::cout << u.first << "(" << class_to_num_vertices[u.first] << ")" << ":" << " ";
  //   for (const auto& v : adj_list[u.first]) {
  //     std::cout << v.first << ":" << v.second << " ";
  //   }

  //   std::cout << '\n';
  // }

  // std::cout << "Special vertex classes: ";
  // for (int special_vertex_class : special_vertex_classes) {
  //   std::cout << special_vertex_class << " ";
  // }

  // std::cout << '\n';
  const auto& color_map = get(boost::vertex_color, coloring_graph);
  // std::cout << "Number of special:" << special_vertices.size() << '\n';
  // std::cout << "Number of total:" << boost::num_vertices(coloring_graph) << '\n';
  int n = adj_list.size();
  std::vector<std::vector<double> > probability_matrix(n, std::vector<double>(n, 0));
  for (const auto& [vertex, neighbors] : adj_list) {
    int total_neighbors = 0;
    for (const auto& [neighbor, weight] : neighbors) {
      total_neighbors += weight;
    }

    // std::cout << "total neighbors for class " << vertex << " is " << total_neighbors << std::endl;
    // neighbors of 121: 321, 421, 131, 141, 123, 124

    // neighbors of 123: 323, 423, 143, 
    assert(total_neighbors != 0);

    for (const auto& [neighbor, weight] : neighbors) {
      probability_matrix[vertex][neighbor] = (double) weight / (double) total_neighbors;
    }
  }

  // set precision to 2 decimal places
  std::cout << std::setprecision(10) << std::fixed;
  // for (const auto& v : probability_matrix) {
  //   for (double i : v) {
  //     std::cout << i << " ";
  //   }

  //   std::cout << std::endl;
  // }

  // std::cout << std::endl;
  Eigen::MatrixXd matrix(n, n);
  // std::vector<std::vector<double> > A(n, std::vector<double>(n, 0));
  std::vector<int> c = std::vector<int>(n, -1);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (special_vertex_classes.count(i)) {
        if (i == j) matrix(i, j) = 1;
        else matrix(i, j) = 0;
        continue;
      }
      if (i == j) matrix(i, j) = (probability_matrix[i][j] - 1);
      else matrix(i, j) = probability_matrix[i][j];
    }
  }

  Eigen::VectorXd v(n);
  for (int i = 0; i < n; i++) {
    if (special_vertex_classes.count(i)) {
      v(i) = 0;
    } else {
      v(i) = -1;
    }
  }
  Eigen::VectorXd x = matrix.colPivHouseholderQr().solve(v);
  // std::cout << "The solution is: \n" << x << std::endl;

  double expected_value = 0;
  for (int i = 0; i < n; i++) {
    expected_value += (double) class_to_num_vertices[i] / (double) num_vertices(coloring_graph) * (double) x(i);
  }

  std::cout << "The final expected value is " << expected_value << std::endl;

  for (auto& vertex: special_vertices) {
    put(color_map, vertex, "red");
  }

  // long double path_length = simulate_random_walk(coloring_graph, NUM_SIMULATIONS);
  // double path_length = simulate_random_walk(coloring_graph, 10000, special_vertices);
  // std::cout << "average path length is " << path_length << " for number of colors: " << k << std::endl;
  boost::dynamic_properties dp;
  dp.property("color", get(vertex_color, coloring_graph));
  dp.property("node_id", get(boost::vertex_index, coloring_graph));
  print_graph_as_graphml(coloring_graph, "coloring_graph_" + std::to_string(k) + ".graphml", dp);
  return 0;
}
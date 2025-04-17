#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"
#include "originalToColoring.h"
#include "eigen/Eigen/Dense"


static const int NUM_SIMULATIONS = 100000;
int main() {
  Graph original_graph = create_graph();
  int k = 8;
  std::map<int, std::map<int, int> > adj_list;
  std::set<int> special_vertex_classes;
  std::map<int, int> class_to_num_vertices;
  std::vector<int> special_vertices;
  std::vector<int> reconstructible_vertices;
  std::vector<int> n_complete_non_reconstructible_non_special_vertices;
  Graph coloring_graph = coloringFromOriginal(original_graph, k, adj_list, special_vertex_classes, class_to_num_vertices, special_vertices, reconstructible_vertices);
  find_n_complete_non_reconstructible_non_special_vertices(coloring_graph, special_vertices, reconstructible_vertices, num_vertices(original_graph), n_complete_non_reconstructible_non_special_vertices);
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

  // set precision to 10 decimal places
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

  std::cout << "The number of vertices in coloring graph is " << num_vertices(coloring_graph) << std::endl;
  std::cout << "The number of red vertices in coloring graph is " << special_vertices.size() << std::endl;
  std::cout << "The final theoretical expected value to get to a special vertex is " << expected_value << std::endl;
  std::cout << "The proportion of special vertices (red) is " << (long double) special_vertices.size() / (long double) num_vertices(coloring_graph) << std::endl;
  std::cout << "The proportion of reconstructible vertices (green) is " << (long double) reconstructible_vertices.size() / (long double) num_vertices(coloring_graph) << std::endl;
  std::cout << "The proportion of n-complete non-reconstructible vertices (blue) is " << (long double) n_complete_non_reconstructible_non_special_vertices.size() / (long double) num_vertices(coloring_graph) << std::endl;
  std::cout << "The number of colorings that use exactly k colors is " << num_vertices(coloring_graph) - special_vertices.size() << std::endl;

  for (auto& vertex: special_vertices) {
    put(color_map, vertex, "red");
  }

  for (auto& vertex: reconstructible_vertices) {
    put(color_map, vertex, "green");
  }

  for (auto& vertex: n_complete_non_reconstructible_non_special_vertices) {
    put(color_map, vertex, "blue");
  }

  // long double path_length = simulate_random_walk(coloring_graph, NUM_SIMULATIONS);
  // double path_length = simulate_random_walk(coloring_graph, 10000, special_vertices);
  // std::cout << "average path length is " << path_length << " for number of colors: " << k << std::endl;
  int num_sims = 10000;
  long double path_length = simulate_random_walk_with_bfs(coloring_graph, num_sims, special_vertices, n_complete_non_reconstructible_non_special_vertices);
  std::cout << "average path length for bfs is " << path_length << " for num simulations: " << num_sims << std::endl;
  boost::dynamic_properties dp;
  dp.property("color", get(vertex_color, coloring_graph));
  dp.property("node_id", get(boost::vertex_index, coloring_graph));
  print_graph_as_graphml(coloring_graph, "coloring_graph_" + std::to_string(k) + ".graphml", dp);
  return 0;
}
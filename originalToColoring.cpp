#include "originalToColoring.h"

int next_vertex() {
  static int next = 0;
  std::cout << "next called, and gonna return " << next << std::endl;
  return next++;
}

void print_coloring(const std::vector<int>& coloring) {
  for (int i = 0; i < coloring.size(); i++) {
    std::cout << coloring[i] << "";
  }
}

void print_coloring_to_vertex_map(const std::map<std::vector<int>, int>& coloring_to_vertex_map) {
  for (const auto [coloring, vertex_number] : coloring_to_vertex_map) {
    print_coloring(coloring);
    std::cout << "above was the coloring for vertex " << vertex_number << std::endl;
  }
}

bool is_valid_coloring(const Graph& original, const std::vector<int>& next_coloring, int changed_vertex) {
  // check all adjacent vertices of changed_vertex
  const auto& neighbors_of_changed_vertex = make_iterator_range(out_edges(changed_vertex, original));
  for (const auto& e : neighbors_of_changed_vertex) {
    const auto& neighbor_of_changed_vertex = target(e, original);
    if (next_coloring[neighbor_of_changed_vertex] == next_coloring[changed_vertex]) return false;
  }

  return true;
}

// TODO: implement
std::vector<int> lowest_permutation(std::vector<int> coloring) {
  std::cout << "generating lowest perm for -> ";
  print_coloring(coloring); std::cout << std::endl;
  std::map<int, int> colorsToLowestColor;
  int next_color = 0;
  for (int color : coloring) {
    if (!colorsToLowestColor.count(color)) {
      colorsToLowestColor[color] = next_color;
      next_color++;
    }
  }

  std::vector<int> result;
  for (int color : coloring) {
    result.push_back(colorsToLowestColor[color]);
  }
  print_coloring(result);
  std::cout << std::endl;
  return result;
}

Graph coloringFromOriginal(const Graph& original, int k) { 
  std::map<int, std::vector<int> > coloring_from_vertex_number;
  std::map<std::vector<int>, int> vertex_number_from_coloring;
  Graph coloring_graph;
  std::vector<int> color_map(num_vertices(original));
  boost::iterator_property_map<std::vector<int>::iterator, boost::property_map<Graph, boost::vertex_index_t>::const_type> color_property_map(color_map.begin(), get(boost::vertex_index, original));

  // Perform vertex coloring, color_map stores the colorings
  boost::sequential_vertex_coloring(original, color_property_map);
  // Get lowest coloring
  const auto lowest_permutation_of_initial_coloring = lowest_permutation(color_map);

  std::vector<int> current_permutation(k);
  std::vector<std::vector<int> > all_permutations;
  for (int i = 0; i < current_permutation.size(); i++) 
    current_permutation[i] = i;

  do {
    all_permutations.push_back(current_permutation);
  } while (std::next_permutation(current_permutation.begin(), current_permutation.end()));

  // for (const auto& permutation: all_permutations) {
  //   print_coloring(permutation);
  //   std::cout << std::endl;
  // }
  std::queue<std::vector<int> > bfs_queue;
  bfs_queue.push(lowest_permutation_of_initial_coloring);
  std::set<std::vector<int> > seen_colorings;
  seen_colorings.insert(lowest_permutation_of_initial_coloring);

  int n = num_vertices(original);
  while (!bfs_queue.empty()) {
    std::vector<int> alpha = bfs_queue.front();
    std::cout << "next alpha is -> ";
    print_coloring(alpha);
    std::cout << std::endl;
    bfs_queue.pop();
    // create vertex for alpha immediately
    if (!vertex_number_from_coloring.count(alpha)) {
      int vertex_for_alpha = next_vertex();
      add_vertex(coloring_graph);
      if (!coloring_from_vertex_number.count(vertex_for_alpha)) coloring_from_vertex_number[vertex_for_alpha] = alpha;
      vertex_number_from_coloring[alpha] = vertex_for_alpha;
    }

    for (int nextColor = 0; nextColor < k; nextColor++) {
      for (int vertex = 0; vertex < n; vertex++) {
        int previousColor = alpha[vertex];
        if (nextColor == previousColor) continue;
        // color alpha to get an adjacent vertex beta
        std::vector<int> beta = alpha;
        beta[vertex] = nextColor;
        std::cout << "trying to check beta coloring: ";
        print_coloring(beta);
        std::cout << std::endl;
        if (!is_valid_coloring(original, beta, vertex)) {
          std::cout << "invalid coloring" << std::endl;
          continue;
        }

        if (!vertex_number_from_coloring.count(beta)) {
          std::cout << "creating vertex for beta ";
          print_coloring(beta);
          std::cout << std::endl;
          int vertex_for_beta = next_vertex();
          add_vertex(coloring_graph);
          coloring_from_vertex_number[vertex_for_beta] = beta;
          vertex_number_from_coloring[beta] = vertex_for_beta;
        }

        // if (seen_colorings.count(lowest_permutation(beta))) continue;
        std::cout << std::endl;
        std::cout << "continuing with beta" << std::endl;
        if (!seen_colorings.count(lowest_permutation(beta))) {
          seen_colorings.insert(lowest_permutation(beta));
          bfs_queue.push(lowest_permutation(beta));
        }

        for (const auto& permutation : all_permutations) {
          // look at all permutations of beta
          std::vector<int> sigma_i_alpha = alpha;
          std::vector<int> sigma_i_beta = beta;
          for (int i = 0; i < n; i++) {
            int current_color_in_alpha = alpha[i];
            sigma_i_alpha[i] = permutation[current_color_in_alpha];

            int current_color_in_beta = beta[i];
            sigma_i_beta[i] = permutation[current_color_in_beta];
            // need to make an edge between sigma of beta and sigma of alpha
          }

          //sigma_i(beta),sigma_i(alpha)
          if (!vertex_number_from_coloring.count(sigma_i_alpha)) {
            add_vertex(coloring_graph);
            int sigma_i_alpha_vertex_number = next_vertex();
            vertex_number_from_coloring[sigma_i_alpha] = sigma_i_alpha_vertex_number;
            coloring_from_vertex_number[sigma_i_alpha_vertex_number] = sigma_i_alpha;
          }

          if (!vertex_number_from_coloring.count(sigma_i_beta)) {
            add_vertex(coloring_graph);
            int sigma_i_beta_vertex_number = next_vertex();
            vertex_number_from_coloring[sigma_i_beta] = sigma_i_beta_vertex_number;
            coloring_from_vertex_number[sigma_i_beta_vertex_number] = sigma_i_beta;
          }

          // both sigma_i(alpha), sigma_i(beta) exist in the graph
          int u = vertex_number_from_coloring[sigma_i_alpha];
          int v = vertex_number_from_coloring[sigma_i_beta];
          if (!edge(u, v, coloring_graph).second)
            add_edge(u, v, coloring_graph);
        }
      }
    }
  }

  for (const auto [vertex_number, coloring] : coloring_from_vertex_number) {
    std::cout << vertex_number << ":";
    print_coloring(coloring);
    std::cout << std::endl;
  }

  return coloring_graph;
} 

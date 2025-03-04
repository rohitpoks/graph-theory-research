#include "originalToColoring.h"
#include <math.h>

// ASSUMES K > KAI

int next_vertex() {
  static int next = 0;
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


int find_num_colors(const std::vector<int>& coloring) {
  std::set<int> used_colors;
  for (int i : coloring) {
    used_colors.insert(i);
  } 

  return used_colors.size();
}


bool is_special_class(const std::vector<int>& coloring, int chromatic_number) {
  return find_num_colors(coloring) == chromatic_number;
}


std::vector<int> lowest_permutation(std::vector<int> coloring) {
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

  return result;
}

void populate_class_to_num_vertices(std::map<int, int>& class_to_num_vertices, int total_vertices, std::map<int, std::vector<int> >& coloring_from_vertex_number, std::map<std::vector<int>, int>& coloring_class_number_from_lowest_permutation) {
  for (const auto& coloring : coloring_from_vertex_number) {
    class_to_num_vertices[coloring_class_number_from_lowest_permutation[lowest_permutation(coloring.second)]]++;
  }
}

Graph coloringFromOriginal(const Graph& original, int k, std::map<int, std::map<int, int> >& adj_list, std::set<int>& special_vertex_classes, std::map<int, int>& class_to_num_vertices, std::vector<int>& special_vertices) { 
  std::map<int, std::vector<int> > coloring_from_vertex_number;
  std::map<std::vector<int>, int> vertex_number_from_coloring;
  std::map<std::vector<int>, int> coloring_class_number_from_lowest_permutation;
  Graph coloring_graph;
  std::vector<int> color_map(num_vertices(original));
  boost::iterator_property_map<std::vector<int>::iterator, boost::property_map<Graph, boost::vertex_index_t>::const_type> color_property_map(color_map.begin(), get(boost::vertex_index, original));

  // Perform vertex coloring, color_map stores the colorings
  boost::sequential_vertex_coloring(original, color_property_map);
  // Get lowest coloring
  const auto lowest_permutation_of_initial_coloring = lowest_permutation(color_map);

  // Set the 'class number' for the coloring to 0
  coloring_class_number_from_lowest_permutation[lowest_permutation_of_initial_coloring] = 0;

  // Set the current class number to 1
  int current_class_number = 1;

  std::vector<int> current_permutation(k);
  std::vector<std::vector<int> > all_permutations;
  for (int i = 0; i < current_permutation.size(); i++) 
    current_permutation[i] = i;
  do {
    all_permutations.push_back(current_permutation);
  } while (std::next_permutation(current_permutation.begin(), current_permutation.end()));

  std::queue<std::vector<int> > bfs_queue;
  bfs_queue.push(lowest_permutation_of_initial_coloring);
  std::set<std::vector<int> > seen_colorings;
  seen_colorings.insert(lowest_permutation_of_initial_coloring);

  int n = num_vertices(original);
  while (!bfs_queue.empty()) {
    std::vector<int> alpha = bfs_queue.front();
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
        if (!is_valid_coloring(original, beta, vertex)) {
          continue;
        }

        if (!vertex_number_from_coloring.count(beta)) {
          int vertex_for_beta = next_vertex();
          add_vertex(coloring_graph);
          coloring_from_vertex_number[vertex_for_beta] = beta;
          vertex_number_from_coloring[beta] = vertex_for_beta;
        }

        if (!seen_colorings.count(lowest_permutation(beta))) {
          seen_colorings.insert(lowest_permutation(beta));
          bfs_queue.push(lowest_permutation(beta));
          coloring_class_number_from_lowest_permutation[lowest_permutation(beta)] = current_class_number;
          current_class_number++;
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

  int t = boost::num_vertices(coloring_graph);
  for (int i = 0; i < t; i++) {
    // assertion for isolated vertices
    assert(std::distance(boost::adjacent_vertices(i, coloring_graph).first, boost::adjacent_vertices(i, coloring_graph).second) != 0);
  }

  // get every vertex from every 'seen permutation'
  for (const auto& coloring_class : seen_colorings) {
    int u = coloring_class_number_from_lowest_permutation[lowest_permutation(coloring_class)];
    for (const auto& e : make_iterator_range(out_edges(u, coloring_graph))) {
        const auto& neighbor = target(e, coloring_graph);
        if (edge(u, neighbor, coloring_graph).second) {
            int v = coloring_class_number_from_lowest_permutation[lowest_permutation(coloring_from_vertex_number[neighbor])];
            adj_list[u][v]++;
            adj_list[v][u]++;
        }
    }
  }

  int chromatic_number = INT_MAX;
  // get chromatic number
  for (auto& seen_coloring: seen_colorings) {
    int number_of_colors = find_num_colors(seen_coloring);
    chromatic_number = std::min(chromatic_number, number_of_colors);
  }


  // get special vertices
  for (int vertex = 0; vertex < num_vertices(coloring_graph); vertex++) {
    if (is_special_class(coloring_from_vertex_number[vertex], chromatic_number)) {
      special_vertices.push_back(vertex);
    }
  }

  // get all special vertex classes
  for (auto& seen_coloring: seen_colorings) {
    if (is_special_class(seen_coloring, chromatic_number)) {
      special_vertex_classes.insert(coloring_class_number_from_lowest_permutation[seen_coloring]);
    }
  }


  populate_class_to_num_vertices(class_to_num_vertices, num_vertices(coloring_graph), coloring_from_vertex_number, coloring_class_number_from_lowest_permutation);
  return coloring_graph;
}

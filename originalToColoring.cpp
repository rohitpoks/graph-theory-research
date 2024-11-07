#include "originalToColoring.h"
// TODO: implement
bool is_valid_coloring(const Graph& original, const std::vector<int>& next_coloring);

// TODO: implement
std::vector<int> lowest_permutation(std::vector<int> coloring);

void coloringFromOriginal(const Graph& original, int k) {
  std::vector<int> color_map(num_vertices(original));
  boost::iterator_property_map<std::vector<int>::iterator, boost::property_map<Graph, boost::vertex_index_t>::const_type> color_property_map(color_map.begin(), get(boost::vertex_index, original));

  // Perform vertex coloring, color_map stores the colorings
  int num_colors = boost::sequential_vertex_coloring(original, color_property_map);
  

  std::vector<int> current_permutation(num_colors);
  std::vector<std::vector<int> > all_permutations;
  for (int i = 0; i < current_permutation.size(); i++) 
    current_permutation[i] = i;

  do {
    all_permutations.push_back(current_permutation);
  } while (std::next_permutation(current_permutation.begin(), current_permutation.end()));

  std::queue<std::vector<int> > bfs_queue;
  std::set<std::vector<int> > seen_colorings;

  int n = num_vertices(original);
  while (!bfs_queue.empty()) {
    std::vector<int> alpha = bfs_queue.front();
    bfs_queue.pop();

    for (int nextColor = 0; nextColor < k; nextColor++) {
      for (int vertex = 0; vertex < n; vertex++) {
        int previousColor = alpha[vertex];
        if (nextColor == previousColor) continue;
        // color alpha to get an adjacent vertex beta
        alpha[vertex] = nextColor;
        if (!is_valid_coloring(original, alpha) || seen_colorings.count(lowest_permutation(alpha))) {
          // revert beta to alpha if not valid coloring
          alpha[vertex] = previousColor;
          continue;
        }

        seen_colorings.insert(alpha);
        bfs_queue.push(alpha);
        for (const auto& permutation : all_permutations) {
          // look at all permutations of beta
          for (int i = 0; i < n; i++) {
            int current_color = alpha[i];
            // sigma of beta is:
            alpha[i] = permutation[current_color];
            // need to make an edge between sigma of beta and sigma of alpha
            
          }
        }
      }


    }
  }
} 

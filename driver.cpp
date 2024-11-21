#include "gtr.h"
#include "graphSketcher.h"
#include "graphProcessor.h"
#include "originalToColoring.h"

int main() {
  Graph original_graph = create_graph();
  Graph coloring_graph = coloringFromOriginal(original_graph, 8);
  print_graph_as_dot(coloring_graph, "coloring_graph.dot"); 
  return 0;
}
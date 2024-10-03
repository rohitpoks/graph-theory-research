#include "gtr.h"
// Creating the original graph from a coloring graph:
// Find every complete graph H_1, ..., H_n which includes *.
// For i = 1 to n: Draw vertex v_i in G
// For i = 1 to n:
    // For j = i+1 to n:
        // For each pair u in H_i, w in H_j,
            //If there is no square between them, draw edge (v_i, v_j) in G...
            //...and move on to the next pair

// Finding a square
// Given vertices u and w:
// For each vertex adjacent to u, which is not *,
    // if it is adjacent to w, return true
// return false

// Finding complete graphs which include *:
// allCompleteGraphs = {}
// seen = {*}
// for each v_i adjacent to *, which is not in seen,
    // newCompleteGraph = {}
    // dfs(v_i, seen, graph, newCompleteGraph)
    // add newCompleteGraph to allCompleteGraphs

// dfs(vertex, seen, graph, verticesInCompleteGraph):
// if vertex in seen: return
// add vertex to seen
// add vertex to verticesInCompleteGraph
// for neighbor in graph[vertex]:
    // if neighbor adjacent to *, dfs(neighbor, deen, graph, verticesInCompleteGraph)

void dfs(const Vertex& v_i, Vertex& special_vertex, std::set<Vertex>& seen, Graph& graph, std::vector<Vertex>& newCompleteGraph) {
    if (seen.count(v_i)) {
        return;
    }

    seen.insert(v_i);
    newCompleteGraph.push_back(v_i);
    for (auto e : make_iterator_range(out_edges(v_i, graph))) {
        const auto& neighbor = target(e, graph);
        if (edge(neighbor, special_vertex, graph).second) {
            dfs(neighbor, special_vertex, seen, graph, newCompleteGraph);
        }
    }
}

std::vector<std::vector<Vertex> > find_adjacent_complete_graphs(Vertex& special_vertex, Graph& graph) {
    std::set<Vertex> seen;
    seen.insert(special_vertex);
    std::vector<std::vector<Vertex> > adjacent_vertices;
    const auto& neighbors_of_special_vertex = make_iterator_range(out_edges(special_vertex, graph));
    for (const auto& e : neighbors_of_special_vertex) {
        const auto& neighbor_of_special_vertex = target(e, graph);
        if (!seen.count(neighbor_of_special_vertex)) {
            std::vector<Vertex> newCompleteGraph;
            dfs(neighbor_of_special_vertex, special_vertex, seen, graph, newCompleteGraph);
            adjacent_vertices.push_back(newCompleteGraph);
        }
    }

    return adjacent_vertices;
}
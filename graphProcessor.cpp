#include "gtr.h"
#include "graphProcessor.h"

void dfs(const Vertex& v_i, Vertex& special_vertex, std::set<Vertex>& seen, Graph& graph, std::vector<Vertex>& newCompleteGraph) {
    if (seen.count(v_i)) {
        return;
    }

    seen.insert(v_i);
    newCompleteGraph.push_back(v_i);
    for (const auto& e : make_iterator_range(out_edges(v_i, graph))) {
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

// Finding a square
// Given vertices u and w:
// For each vertex adjacent to u, which is not *,
    // if it is adjacent to w, return true
// return false

bool has_square(const Vertex& u, const Vertex& w, const Graph& graph, const Vertex& special_vertex) {
    for (const auto& u_outgoing_edge: make_iterator_range(out_edges(u, graph))) {
        const auto& u_neighbor = target(u_outgoing_edge, graph);
        if (u_neighbor == special_vertex) continue;

        if (edge(u_neighbor, w, graph).second) return true;
    }

    return false;
}

// Creating the original graph from a coloring graph:
// Find every complete graph H_1, ..., H_n which includes *.
// For i = 1 to n: Draw vertex v_i in G
// For i = 1 to n:
    // For j = i+1 to n:
        // For each pair u in H_i, w in H_j,
            //If there is no square between them, draw edge (v_i, v_j) in G...
            //...and move on to the next pair

Graph find_original_graph(Graph& coloring_graph, Vertex special_vertex) {
    const auto& complete_graphs = find_adjacent_complete_graphs(special_vertex, coloring_graph);
    int n = complete_graphs.size();
    std::vector<Edge> edge_list;
    for (int i = 0; i < n; i++) {
        const auto& ith_complete_graph = complete_graphs[i];
        for (int j = i + 1; j < n; j++) {
            bool found_edge = false;
            const auto& jth_complete_graph = complete_graphs[j];
            for (int u_index = 0; u_index < ith_complete_graph.size() && !found_edge; u_index++) {
                
                const auto& u = ith_complete_graph[u_index];

                if (u == special_vertex) 
                    continue;

                for (int w_index = 0; w_index < jth_complete_graph.size() && !found_edge; w_index++) {
                    const auto& w = jth_complete_graph[w_index];

                    if (w == special_vertex) 
                        continue;

                    if (found_edge) 
                        break;

                    if (!has_square(u, w, coloring_graph, special_vertex)) {
                        edge_list.push_back(Edge(i, j));
                        found_edge = true;
                    }
                }
            }
        }
    }

    Edge* edge_list_array = &edge_list[0];
    return Graph(edge_list_array, edge_list_array + sizeof(edge_list_array)/sizeof(Edge), n);
}
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
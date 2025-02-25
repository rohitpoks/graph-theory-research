#include "gtr.h"
#include "graphProcessor.h"

// simulate a random walk for given number of simulations
long double simulate_random_walk(const Graph& graph, int number_of_simulations) {
    srand(time(0));
    std::vector<Vertex> special_vertices_vector = find_special_vertices_in_coloring(graph);
    std::set<Vertex> special_vertices = std::set<Vertex>(special_vertices_vector.begin(), special_vertices_vector.end());
    int n = boost::num_vertices(graph);
    long long sum_of_distances = 0; 
    for (int i = 0; i < number_of_simulations; i++) {
        long long path_length = 0;
        int current_vertex = rand() % n;

        while (!special_vertices.count(current_vertex)) {
            assert(current_vertex >= 0 && current_vertex < n);
            auto [neighbors_begin, neighbors_end] = boost::adjacent_vertices(current_vertex, graph);
            int number_of_neighbors = std::distance(neighbors_begin, neighbors_end);
            neighbors_begin += (rand() % number_of_neighbors);
            current_vertex = *neighbors_begin;
            path_length++;
            // std::cout << "path length is " << path_length << std::endl;
        }

        sum_of_distances += path_length;
    }

    return (long double) sum_of_distances / (long double) number_of_simulations;
}

// returns 1 if v1 "majorizes" v2, 0 if v2 "majorizes" v1, or 2 if they are the same
int first_majorizes_second(const std::vector<int> v1, const std::vector<int> v2) {
    if (v1.size() > v2.size()) 
        return 1;

    if (v2.size() > v1.size()) 
        return 0;

    for (int i = 0; i < std::min(v1.size(), v2.size()); i++) {
        if (v1[i] > v2[i]) 
            return 1;

        if (v1[i] < v2[i]) 
            return 0;
    }

    return 2;
}

void dfs(const Vertex& v_i, const Vertex& special_vertex, std::set<Vertex>& seen, const Graph& graph, std::vector<Vertex>& newCompleteGraph) {
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

std::vector<std::vector<Vertex> > find_adjacent_complete_graphs(const Vertex& special_vertex, const Graph& graph) {
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

    Edge edges_array[edge_list.size()];
    std::copy(edge_list.begin(), edge_list.end(), edges_array);
    return Graph(edges_array, edges_array + sizeof(edges_array)/sizeof(Edge), n);
}

std::vector<Vertex> find_special_vertices_in_coloring(const Graph& coloring_graph) {
    std::vector<std::vector<int> > adjacent_complete_graph_sizes_from_vertex;
    for (const auto& vertex : boost::make_iterator_range(vertices(coloring_graph))) {
        const auto& adjacent_complete_graphs = find_adjacent_complete_graphs(vertex, coloring_graph);
        std::vector<int> sizes_of_adjacent_complete_graphs;
        for (const auto& adjacent_complete_graph : adjacent_complete_graphs) {
            sizes_of_adjacent_complete_graphs.push_back(adjacent_complete_graph.size());
        }

        adjacent_complete_graph_sizes_from_vertex.push_back(sizes_of_adjacent_complete_graphs);
        // sort the vector that was just added to adjacent_complete_graph_sizes_from_vertex in non-increasing order
        sort(adjacent_complete_graph_sizes_from_vertex.back().begin(), adjacent_complete_graph_sizes_from_vertex.back().end(), std::greater<int>());
    }

    std::vector<unsigned long> special_vertices;
    std::vector<int> best_adjacent_complete_graph_sizes = {-1};
    for (const auto& vertex : boost::make_iterator_range(vertices(coloring_graph))) {
        const auto& current_adjacent_complete_graph_sizes = adjacent_complete_graph_sizes_from_vertex[vertex];
        int current_majorizes_best = first_majorizes_second(current_adjacent_complete_graph_sizes, best_adjacent_complete_graph_sizes);
        if (current_majorizes_best == 1) {
            best_adjacent_complete_graph_sizes = current_adjacent_complete_graph_sizes;
            special_vertices = {vertex};
        } else if (current_majorizes_best == 2) {
            special_vertices.push_back(vertex);
        }
    }

    return special_vertices;
}
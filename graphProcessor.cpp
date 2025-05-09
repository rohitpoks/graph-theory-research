#include "gtr.h"
#include "graphProcessor.h"

// perform a random walk and return # of steps until a special vertex is reached
int random_walk_with_bfs(const Graph& graph, std::vector<int>& special_vertices_vector, std::vector<int>& n_complete_non_special_vertices_vector) { 
    std::set<Vertex> special_vertices = std::set<Vertex>(special_vertices_vector.begin(), special_vertices_vector.end());
    std::set<Vertex> n_complete_non_special_vertices = std::set<Vertex>(n_complete_non_special_vertices_vector.begin(), n_complete_non_special_vertices_vector.end());
    int n = num_vertices(graph);
    int walk_length = 0;
    int current_vertex = rand() % n;
    while (!special_vertices.count(current_vertex) && !n_complete_non_special_vertices.count(current_vertex)) {
        assert(current_vertex >= 0 && current_vertex < n);
        auto [neighbors_begin, neighbors_end] = boost::adjacent_vertices(current_vertex, graph);
        int number_of_neighbors = std::distance(neighbors_begin, neighbors_end);
        neighbors_begin += (rand() % number_of_neighbors);
        current_vertex = *neighbors_begin;
        walk_length++;
    }

    if (special_vertices.count(current_vertex)) return walk_length;
    int dist_from_blue = 0;
    int num_visited_vertices = 0;
    // start bfs from blue vertex
    queue<int> q;
    std::set<int> seen;
    q.push(current_vertex);
    seen.insert(current_vertex);
    while (!q.empty()) {
        int size = q.size();
        while (size--) {
            int next = q.front();
            q.pop();
            num_visited_vertices++;
            if (special_vertices.count(next)) {
                return walk_length + num_visited_vertices;
            }

            boost::graph_traits<Graph>::adjacency_iterator it, end;
            std::vector<int> adj;
            for (tie(it, end) = adjacent_vertices(next, graph); it != end; ++it) {
                int neighbor = *it;
                adj.push_back(neighbor);
            }

            // TODO: optimize this
            auto rd = std::random_device {}; 
            auto rng = std::default_random_engine { rd() };
            std::shuffle(std::begin(adj), std::end(adj), rng);

            for (int neighbor : adj) {
                if (seen.count(neighbor)) continue;
                seen.insert(neighbor);
                q.push(neighbor);
            }
        }

        dist_from_blue++;
    }

    // code should never reach this point!
    // this might imply there are no red vertices in the graph :(
    assert(1 != 1);
    return walk_length + num_visited_vertices;
}


long double simulate_random_walk_with_bfs(const Graph& graph, int number_of_simulations, std::vector<int>& special_vertices_vector, std::vector<int>& n_complete_non_special_vertices_vector) {
    srand(time(0));
    long double total = 0;
    for (int i = 0; i < number_of_simulations; i++) {
        int next_walk_length = random_walk_with_bfs(graph, special_vertices_vector, n_complete_non_special_vertices_vector);
        total += next_walk_length;
    }

    return total / (long double) number_of_simulations;
}


// simulate a random walk for given number of simulations
long double simulate_random_walk(const Graph& graph, int number_of_simulations, std::vector<int>& special_vertices_vector) {
    srand(time(0));
    // std::vector<Vertex> special_vertices_vector = find_special_vertices_in_coloring(graph);
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

void find_n_complete_non_reconstructible_non_special_vertices(const Graph& coloring_graph, std::vector<int>& special_vertices, std::vector<int>& reconstructible_vertices, int num_vertices_in_original_graph, std::vector<int>& result) {
    std::vector<std::vector<int> > adjacent_complete_graph_sizes_from_vertex;
    std::set<int> special_vertices_set = std::set<int>(special_vertices.begin(), special_vertices.end());
    std::set<int> reconstructible_vertices_set = std::set<int>(reconstructible_vertices.begin(), reconstructible_vertices.end());
    for (const auto& vertex : boost::make_iterator_range(vertices(coloring_graph))) {
        const auto& adjacent_complete_graphs = find_adjacent_complete_graphs(vertex, coloring_graph);
        if (adjacent_complete_graphs.size() == num_vertices_in_original_graph) {
            if (special_vertices_set.count(vertex) || reconstructible_vertices_set.count(vertex)) continue;
            result.push_back(vertex);
        }
    }
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
    std::vector<int> best_adjacent_complete_graph_sizes = {};
    for (const auto& vertex : boost::make_iterator_range(vertices(coloring_graph))) {
        const auto& current_adjacent_complete_graph_sizes = adjacent_complete_graph_sizes_from_vertex[vertex];
        int current_majorizes_best = first_majorizes_second(current_adjacent_complete_graph_sizes, best_adjacent_complete_graph_sizes);
        if (current_majorizes_best == 1) {
            // found a more special vertex
            best_adjacent_complete_graph_sizes = current_adjacent_complete_graph_sizes;
            special_vertices = {vertex};
        } else if (current_majorizes_best == 2) {
            // found a vertex that is as good as the best one so far
            special_vertices.push_back(vertex);
        }
    }

    return special_vertices;
}   
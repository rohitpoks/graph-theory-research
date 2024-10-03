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
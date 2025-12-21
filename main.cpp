#include <iostream>
#include <queue>
#include <vector>
#include <climits>
#include <algorithm>
#include "tests.hpp"
#include "graphs.hpp"

using namespace std;

/*
Finds shortest path from node s to node t in a graph g and it's length using Dijkstra's algorithm
@param g Graph structure, must have get_n(), get_neighbours(u) and get_distance(u, v) methods
@param s Start node index in g
@param t Finish node index in g
@return pair<length, path> - length of path and sequence of indices in path from start to finish stored in vector 
*/
template<typename Graph>
pair<long long, vector<int>> dijkstra(const Graph& g, int s, int t) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
    vector<long long> dist(g.get_n(), INT_MAX);
    vector<int> prev(g.get_n());

    dist[s] = 0;

    Q.push({0, s});

    while (!Q.empty()) {
        auto [d, u] = Q.top();
        Q.pop();

        if (u == t) break;                              // shortest path to t was found
        
        if (dist[u] < d) continue;                      // skip old values of d for u

        for (int v : g.get_neighbours(u)) {
            long long new_dist = dist[u] + g.get_distance(u, v);
            if (new_dist < dist[v]) {
                prev[v] = u;
                dist[v] = new_dist;
                Q.push({new_dist, v});                  // can't quickly remove elements from pq => just adding new ones and ignoring old ones
            }
        }        
    }

    // recover path from prevs 
    int v = t;
    vector<int> path;
    path.push_back(t);
    while (v != s) {
        path.push_back(prev[v]);
        v = prev[v];
    }
    reverse(path.begin(), path.end());

    return {dist[t], path};
}


int main() {
    test1();
    test2();
    test_no_walls();
    test_vertical_walls_maze_to_file(20, 20, "maze.txt");
    test_vertical_walls_maze_with_diags_to_file(20, 20, "maze_diags.txt");
}
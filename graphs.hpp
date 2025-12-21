#pragma once
#include <vector>
#include <unordered_set>
#include <climits>
#include <cmath>

using namespace std;

/*
Default realisation of graph structure using adjacency matrix
*/
struct dense_graph {
    /*
    @param adj_matrix Adjacency matrix
    */
    dense_graph(const vector<vector<int>>& adj_matrix) : adjacency_matrix(adj_matrix) {}
    /*
    Get indices of nodes that are avaiable to go to from node v
    */
    vector<int> get_neighbours(int v) const {
        vector<int> neighbours;
        for (int j = 0; j < adjacency_matrix[0].size(); j++) {
            if (adjacency_matrix[v][j] != 0) neighbours.push_back(j);
        }
        return neighbours;
    }
    /*
    Get distance between u and v
    */
    int get_distance(int u, int v) const {
        return adjacency_matrix[u][v];
    }
    /*
    Get number of nodes
    */
    int get_n() const {
        return adjacency_matrix.size();
    }
    vector<vector<int>> adjacency_matrix;
};

/*
Special graph structure for caertesian field with fast get_neighbours and low memory usage
*/
struct cartesian_rectangular_field {
    /*
    @param field Matrix filled with 0s and 1s
    */
    cartesian_rectangular_field(vector<vector<int>>& field) {
        nx = field[0].size();
        ny = field.size();
        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx; j++) {
                if (field[i][j] != 0) {
                    walls.insert(i * nx + j);
                }
            }
        }
    }
    
    /*
    @param nx Field size for x coordinate
    @param ny Field size for y coordinate
    @param walls_x Vector with walls coordinates by x
    @param walls_y Vector with walls coordinates by y
    */
    cartesian_rectangular_field(int nx, int ny, vector<int>& walls_x, vector<int>& walls_y) : nx(nx), ny(ny) {
        for (int i = 0; i < walls_x.size(); i++) {
            walls.insert(walls_y[i] * nx + walls_x[i]);
        }
    }
    /*
    Get indices of nodes that are avaiable to go to from node v
    */
    vector<int> get_neighbours(int v) const {
        vector<int> neighbours;
        if ((v / nx) + 1 < ny) neighbours.push_back(((v / nx) + 1) * nx + (v % nx));
        if ((v / nx) - 1 >= 0) neighbours.push_back(((v / nx) - 1) * nx + (v % nx));
        if ((v % nx) + 1 < nx) neighbours.push_back((v / nx) * nx + (v % nx + 1));
        if ((v % nx) - 1 >= 0) neighbours.push_back((v / nx) * nx + (v % nx - 1));
        return neighbours;
    }
    /*
    Get distance between u and v
    */
    int get_distance(int u, int v) const {
        return (walls.find(v) != walls.end()) ? INT_MAX : 1;
    }
    /*
    Get number of nodes
    */
    int get_n() const {
        return nx * ny;
    }

    // v = i * nx + j

    int nx, ny;
    unordered_set<int> walls;
};

/*
Special graph structure for caertesian field (diagonal movement is possible) with fast get_neighbours and low memory usage
*/
struct cartesian_rectangular_field_with_diagonals {
    /*
    @param field Matrix filled with 0s and 1s
    */
    cartesian_rectangular_field_with_diagonals(vector<vector<int>>& field) {
        nx = field[0].size();
        ny = field.size();
        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx; j++) {
                if (field[i][j] != 0) {
                    walls.insert(i * nx + j);
                }
            }
        }
    }

    /*
    @param nx Field size for x coordinate
    @param ny Field size for y coordinate
    @param walls_x Vector with walls coordinates by x
    @param walls_y Vector with walls coordinates by y
    */
    cartesian_rectangular_field_with_diagonals(int nx, int ny, vector<int>& walls_x, vector<int>& walls_y) : nx(nx), ny(ny) {
        for (int i = 0; i < walls_x.size(); i++) {
            walls.insert(walls_y[i] * nx + walls_x[i]);
        }
    }
    /*
    Get indices of nodes that are avaiable to go to from node v
    */
    vector<int> get_neighbours(int v) const {
        vector<int> neighbours;
        // v = i * nx + j
        if ((v / nx) + 1 < ny) neighbours.push_back(((v / nx) + 1) * nx + (v % nx));
        if ((v / nx) + 1 < ny && (v % nx) + 1 < nx) neighbours.push_back(((v / nx) + 1) * nx + (v % nx + 1));
        if ((v / nx) + 1 < ny && (v % nx) - 1 >= 0) neighbours.push_back(((v / nx) + 1) * nx + (v % nx - 1));
        if ((v / nx) - 1 >= 0) neighbours.push_back(((v / nx) - 1) * nx + (v % nx));
        if ((v / nx) - 1 >= 0 && (v % nx) + 1 < nx) neighbours.push_back(((v / nx) - 1) * nx + (v % nx + 1));
        if ((v / nx) - 1 >= 0 && (v % nx) - 1 >= 0) neighbours.push_back(((v / nx) - 1) * nx + (v % nx - 1));
        if ((v % nx) + 1 < nx) neighbours.push_back((v / nx) * nx + (v % nx + 1));
        if ((v % nx) - 1 >= 0) neighbours.push_back((v / nx) * nx + (v % nx - 1));
        return neighbours;
    }
    /*
    Get distance between u and v
    */
    int get_distance(int u, int v) const {
        if ((v / nx) + 1 == (u / nx)) return (walls.find(v) != walls.end()) ? INT_MAX : 1;
        if ((v / nx) + 1 == (u / nx) && (v % nx) + 1 == (u % nx)) return (walls.find(v) != walls.end()) ? INT_MAX : std::sqrt(2);
        if ((v / nx) + 1 == (u / nx) && (v % nx) - 1 == (u % nx)) return (walls.find(v) != walls.end()) ? INT_MAX : std::sqrt(2);
        if ((v / nx) - 1 == (u / nx)) return (walls.find(v) != walls.end()) ? INT_MAX : 1;
        if ((v / nx) - 1 == (u / nx) && (v % nx) + 1 == (u % nx)) return (walls.find(v) != walls.end()) ? INT_MAX : std::sqrt(2);
        if ((v / nx) - 1 == (u / nx) && (v % nx) - 1 == (u % nx)) return (walls.find(v) != walls.end()) ? INT_MAX : std::sqrt(2);
        if ((v % nx) + 1 == (u / nx)) return (walls.find(v) != walls.end()) ? INT_MAX : 1;
        if ((v % nx) - 1 == (u / nx)) return (walls.find(v) != walls.end()) ? INT_MAX : 1;
        return INT_MAX;
    }
    /*
    Get number of nodes
    */
    int get_n() const {
        return nx * ny;
    }

    int nx, ny;
    unordered_set<int> walls;
};


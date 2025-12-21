#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include "graphs.hpp"
#include <chrono>

using namespace std;

template<typename Graph>
pair<long long, vector<int>> dijkstra(const Graph& g, int s, int t);

void test1() {
    cout << "=== Test 1 ===" << endl;
    //   0 --10-- 1 --20-- 2
    //   |        |        |
    //   30      25       15
    //   |        |        |
    //   3 --50-- 4 --35-- 5
    vector<vector<int>> adj = {
        {0, 10, 0, 30, 0, 0},
        {10, 0, 20, 0, 25, 0},
        {0, 20, 0, 0, 0, 15},
        {30, 0, 0, 0, 50, 0},
        {0, 25, 0, 50, 0, 35},
        {0, 0, 15, 0, 35, 0}
    };
    
    dense_graph g(adj);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = dijkstra(g, 0, 5);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "time: " << duration.count() << " ms" << std::endl;
    
    cout << "Distance from 0 to 5: " << result.first << endl;
    cout << "Path: ";
    for (int v : result.second) {
        cout << v << " ";
    }
    cout << endl;
    // Best path: 0->1->2->5 (10+20+15=45)
}

void test2() {
    cout << "=== Test 2 ===" << endl;
    //   0 --100-- 1
    //   |       /
    //   1    50
    //   |  /
    //    2
    vector<vector<int>> adj = {
        {0, 100, 1},
        {100, 0, 50},
        {1, 50, 0}
    };
    
    dense_graph g(adj);
    auto result = dijkstra(g, 0, 1);
    
    cout << "Distance from 0 to 1: " << result.first << endl;
    cout << "Path: ";
    for (int v : result.second) {
        cout << v << " ";
    }
    cout << endl;
    // Best path: 0->2->1 (1+50=51)
}

void test_no_walls() {
    cout << "=== Test \"No walls\" ===" << endl;
    
    int nx = 5, ny = 5;
    vector<int> walls_x, walls_y; // vectors are empty, no walls
    
    cartesian_rectangular_field field(nx, ny, walls_x, walls_y);
    
    int start = 0; // (0,0)
    int end = ny * nx - 1; // (4,4)
    
    auto result = dijkstra(field, start, end);
    
    cout << "Distance: " << result.first << endl;
    cout << "Path: ";
    for (int node : result.second) {
        int y = node / nx;
        int x = node % nx;
        cout << "(" << x << "," << y << ") ";
    }
    cout << endl;
}

void test_vertical_walls_maze_to_file(int size_x = 20, int size_y = 20, const string& filename = "maze_output.txt") {
    cout << "=== Test \"Maze\" ===" << endl;
    vector<vector<int>> field(size_y, vector<int>(size_x, 0));
    
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            if (j % 3 == 0 && ((i % 4 != 0 || j == 0) || i == 0) ) {
                field[i][j] = 1;
            }
        }
    }
    
    field[4][12] = 1;

    field[0][0] = 0;
    field[size_y-1][size_x-1] = 0;
    
    cartesian_rectangular_field maze(field);
    
    ofstream outfile(filename);
    
    pair<pair<int, int>, pair<int, int>> route= {{0, 0}, {size_x-1, size_y-1}};
    
    int start = route.first.second * size_x + route.first.first;
    int end = route.second.second * size_x + route.second.first;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = dijkstra(maze, start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "time: " << duration.count() << " ms" << std::endl;

    for (int k = 0; k < result.second.size(); k++) {
        int node = result.second[k];
        int y = node / size_x;
        int x = node % size_x;
        outfile << "(" << x << "," << y << ")";
        if (k < result.second.size() - 1) {
            outfile << " ";
        }
    }
    outfile << endl;
    
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            outfile << field[i][j];
            if (j < size_x - 1) {
                outfile << " ";
            }
        }
        outfile << endl;
    }
    cout << "Path and field written to file: " << filename << endl;
    cout << "Path length: " << result.second.size() << " points" << endl;
    
    outfile.close();
}

void test_vertical_walls_maze_with_diags_to_file(int size_x = 20, int size_y = 20, const string& filename = "maze_output.txt") {
    cout << "=== Test \"Maze with diagonal paths\" ===" << endl;
    vector<vector<int>> field(size_y, vector<int>(size_x, 0));
    
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            if (j % 3 == 0 && ((i % 4 != 0 || j == 0) || i == 0) ) {
                field[i][j] = 1;
            }
        }
    }
    
    field[4][12] = 1;

    field[0][0] = 0;
    field[size_y-1][size_x-1] = 0;
    
    cartesian_rectangular_field_with_diagonals maze(field);
    
    ofstream outfile(filename);
    
    pair<pair<int, int>, pair<int, int>> route= {{0, 0}, {size_x-1, size_y-1}};
    
    int start = route.first.second * size_x + route.first.first;
    int end = route.second.second * size_x + route.second.first;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = dijkstra(maze, start, end);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "time: " << duration.count() << " ms" << std::endl;

    for (int k = 0; k < result.second.size(); k++) {
        int node = result.second[k];
        int y = node / size_x;
        int x = node % size_x;
        outfile << "(" << x << "," << y << ")";
        if (k < result.second.size() - 1) {
            outfile << " ";
        }
    }
    outfile << endl;
    
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            outfile << field[i][j];
            if (j < size_x - 1) {
                outfile << " ";
            }
        }
        outfile << endl;
    }
    cout << "Path and field written to file: " << filename << endl;
    cout << "Path length: " << result.second.size() << " points" << endl;
    
    outfile.close();
}

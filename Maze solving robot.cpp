#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <chrono>

class Maze {
public:
    Maze(int size) : size(size), maze(size, std::vector<int>(size, 1)) {}

    void generate() {
        std::stack<std::pair<int, int>> stack;
        std::set<std::pair<int, int>> visited;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 3);

        stack.push({0, 0});
        visited.insert({0, 0});

        while (!stack.empty()) {
            int x = stack.top().first;
            int y = stack.top().second;
            stack.pop();

            std::vector<std::pair<int, int>> neighbors = {{x+2, y}, {x-2, y}, {x, y+2}, {x, y-2}};
            std::vector<std::pair<int, int>> unvisited_neighbors;

            for (const auto& neighbor : neighbors) {
                int nx = neighbor.first;
                int ny = neighbor.second;

                if (nx >= 0 && nx < size && ny >= 0 && ny < size && visited.find({nx, ny}) == visited.end()) {
                    unvisited_neighbors.push_back({nx, ny});
                }
            }

            if (!unvisited_neighbors.empty()) {
                int random_index = dis(gen) % unvisited_neighbors.size();
                int nx = unvisited_neighbors[random_index].first;
                int ny = unvisited_neighbors[random_index].second;

                maze[ny][nx] = 0;  // Carve a path
                stack.push({nx, ny});
                visited.insert({nx, ny});
            }
        }
    }

    std::vector<std::pair<int, int>> solve() {
        std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // Right, Down, Left, Up
        int direction_index = 0;
        int x = 0, y = 0;
        std::vector<std::pair<int, int>> path = {{x, y}};

        while (x != size - 1 || y != size - 1) {
            int left_index = (direction_index + 3) % 4;
            int nx = x + directions[left_index].first;
            int ny = y + directions[left_index].second;

            if (nx >= 0 && nx < size && ny >= 0 && ny < size && maze[ny][nx] == 0) {
                direction_index = left_index;
                x = nx;
                y = ny;
                path.push_back({x, y});
                continue;
            }

            nx = x + directions[direction_index].first;
            ny = y + directions[direction_index].second;

            if (nx >= 0 && nx < size && ny >= 0 && ny < size && maze[ny][nx] == 0) {
                x = nx;
                y = ny;
                path.push_back({x, y});
            }
        }

        return path;
    }

    void visualize() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << (maze[i][j] == 1 ? "██" : "  ");
            }
            std::cout << std::endl;
        }
    }

private:
    int size;
    std::vector<std::vector<int>> maze;
};

int main() {
    int maze_size = 21;  // Adjust the maze size here
    Maze maze(maze_size);

    auto start = std::chrono::steady_clock::now();
    maze.generate();
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Maze generation time: " << elapsed.count() << " ms" << std::endl;

    maze.visualize();

    start = std::chrono::steady_clock::now();
    std::vector<std::pair<int, int>> path = maze.solve();
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Maze solving time: " << elapsed.count() << " ms" << std::endl;

    std::cout << "Path: ";
    for (const auto& p : path) {
        std::cout << "(" << p.first << ", " << p.second << ") ";
    }
    std::cout << std::endl;

    return 0;
}


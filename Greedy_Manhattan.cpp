#include <iostream> 
#include <vector> 
#include <queue> 
#include <algorithm> 
#include <unordered_set> 
 
using namespace std; 
 
// Size of the grid puzzle 
const int GRID_SIZE = 3; 
 
// Structure to represent a state of the grid puzzle 
struct GridState { 
    vector<vector<int>> grid; 
    int heuristicValue;  // Heuristic value based on Manhattan distance 
 
    // Constructor 
    GridState(const vector<vector<int>>& puzzle) : grid(puzzle) { 
        heuristicValue = computeHeuristic(); 
    } 
 
    // Calculate the Manhattan distance heuristic 
    int computeHeuristic() const { 
        int distance = 0; 
        for (int i = 0; i < GRID_SIZE; ++i) { 
            for (int j = 0; j < GRID_SIZE; ++j) { 
                if (grid[i][j] != 0) { 
                    int targetRow = (grid[i][j] - 1) / GRID_SIZE; 
                    int targetCol = (grid[i][j] - 1) % GRID_SIZE; 
                    distance += abs(i - targetRow) + abs(j - targetCol); 
                } 
            } 
        } 
        return distance; 
    } 
 
    // Check if the current state is the goal state 
    bool isGoalState() const { 
        int targetValue = 0; 
        for (int i = 0; i < GRID_SIZE; ++i) { 
            for (int j = 0; j < GRID_SIZE; ++j) { 
                if (grid[i][j] != targetValue) { 
                    return false; 
                } 
                ++targetValue; 
            } 
        } 
        return true; 
    } 
 
    // Check if two grid states are equal 
    bool operator==(const GridState& other) const { 
        return grid == other.grid; 
    } 
}; 
 
// Hash function for GridState (used for unordered_set) 
struct GridStateHash { 
    size_t operator()(const GridState& state) const { 
        size_t hashValue = 0; 
        for (const auto& row : state.grid) { 
            for (int val : row) { 
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2); 
            } 
        } 
        return hashValue; 
    } 
}; 
 
// Comparison function for priority_queue 
struct CompareGridState { 
    bool operator()(const GridState& lhs, const GridState& rhs) const { 
        return lhs.heuristicValue > rhs.heuristicValue; 
    } 
}; 
 
// Function to display the grid state 
void displayGridState(const GridState& state) { 
    for (const auto& row : state.grid) { 
        for (int val : row) { 
            cout << val << " "; 
        } 
        cout << endl; 
    } 
    cout << "Heuristic Value: " << state.heuristicValue << endl; 
    cout << "-----------------" << endl; 
} 
 
// Function to perform the greedy search 
void performGreedySearch(const GridState& initial) { 
    priority_queue<GridState, vector<GridState>, CompareGridState> priorityQueue; 
    unordered_set<GridState, GridStateHash> visited; 
 
    priorityQueue.push(initial); 
 
    while (!priorityQueue.empty()) { 
        GridState current = priorityQueue.top(); 
        priorityQueue.pop(); 
 
        if (current.isGoalState()) { 
            cout << "Goal state reached!" << endl; 
            displayGridState(current); 
            return; 
        } 
 
        if (visited.find(current) == visited.end()) { 
            visited.insert(current); 
            displayGridState(current); 
 
            // Generate possible next states (left, right, up, down moves) 
            vector<int> moves = {-1, 0, 1}; 
 
            for (int dx : moves) { 
                for (int dy : moves) { 
                    if (abs(dx) + abs(dy) == 1) { 
                        int newX = 0, newY = 0; 
 
                        // Find the position of the empty space (0 value) 
                        for (int i = 0; i < GRID_SIZE; ++i) { 
                            for (int j = 0; j < GRID_SIZE; ++j) { 
                                if (current.grid[i][j] == 0) { 
                                    newX = i + dx; 
                                    newY = j + dy; 
                                    break; 
                                } 
                            } 
                        } 
 
                        // Check if the new position is within bounds 
                        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) { 
                            // Create a new state by swapping the empty space and the adjacent tile 
                            vector<vector<int>> newGrid = current.grid; 
                            swap(newGrid[newX][newY], newGrid[newX - dx][newY - dy]); 
 
                            GridState nextState(newGrid); 
 
                            // Add the new state to the priority queue 
                            priorityQueue.push(nextState); 
                        } 
                    } 
                } 
            } 
        } 
    } 
 
    cout << "Goal state not reachable!" << endl; 
} 
 
int main() { 
    // Initial grid state 
    vector<vector<int>> initialGrid = { 
        {8, 0, 6}, 
        {5, 4, 7}, 
        {2, 3, 1} 
    }; 
 
    GridState initialGridState(initialGrid); 
 
    cout << "Initial state:" << endl; 
    displayGridState(initialGridState); 
 
    cout << "Starting greedy search with Manhattan distance heuristic..." << endl; 
    performGreedySearch(initialGridState); 
 
    return 0; 
} 


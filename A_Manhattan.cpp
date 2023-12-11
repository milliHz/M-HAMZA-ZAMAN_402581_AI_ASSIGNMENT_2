#include <iostream> 
#include <vector> 
#include <queue> 
#include <unordered_set> 
 
using namespace std; 
 
struct GameTile { 
    vector<vector<int>> layout; 
    int totalCost; 
    int heuristicValue; 
    int movesTaken; 
    const GameTile* prevTile; 
 
    GameTile(const vector<vector<int>>& arrangement, int cost, int heuristic, int moves, const GameTile* previous) 
        : layout(arrangement), totalCost(cost), heuristicValue(heuristic), movesTaken(moves), prevTile(previous) {} 
 
    bool operator==(const GameTile& other) const { 
        return layout == other.layout; 
    } 
}; 
 
struct GameTileHash { 
    size_t operator()(const GameTile& tile) const { 
        size_t hash = 0; 
        for (const auto& row : tile.layout) { 
            for (int num : row) { 
                hash ^= hash << 6 ^ hash >> 2 ^ size_t(num) + 0x9e3779b9 + (hash << 14) + (hash >> 7); 
            } 
        } 
        return hash; 
    } 
}; 
 
struct GameTileComparator { 
    bool operator()(const GameTile& a, const GameTile& b) const { 
        return a.totalCost + a.heuristicValue > b.totalCost + b.heuristicValue; 
    } 
}; 
 
void displayBoard(const vector<vector<int>>& arrangement) { 
    for (const auto& row : arrangement) { 
        for (int num : row) { 
            cout << num << " "; 
        } 
        cout << endl; 
    } 
    cout << endl; 
} 
 
pair<int, int> locateNumber(const vector<vector<int>>& arrangement, int number) { 
    for (int i = 0; i < 3; ++i) { 
        for (int j = 0; j < 3; ++j) { 
            if (arrangement[i][j] == number) { 
                return {i, j}; 
            } 
        } 
    } 
    return {-1, -1}; 
} 
 
int computeManhattan(const GameTile& tile) { 
    int distance = 0; 
    for (int num = 1; num <= 8; ++num) { 
        pair<int, int> currentPos = locateNumber(tile.layout, num); 
        pair<int, int> goalPos = {(num - 1) / 3, (num - 1) % 3}; 
        distance += abs(currentPos.first - goalPos.first) + abs(currentPos.second - goalPos.second); 
    } 
    return distance; 
} 
 
bool isValid(int i, int j) { 
    return i >= 0 && i < 3 && j >= 0 && j < 3; 
} 
 
vector<GameTile> generateTiles(const GameTile& tile); 
 
void executeAStar(const vector<vector<int>>& initialLayout); 
 
int main() { 
    vector<vector<int>> startLayout = {{1, 2, 3}, {0, 4, 6}, {7, 5, 8}}; 
 
    cout << "Initial state:" << endl; 
    displayBoard(startLayout); 
 
    executeAStar(startLayout); 
 
    return 0; 
} 
 
vector<GameTile> generateTiles(const GameTile& tile) { 
    vector<GameTile> tiles; 
    pair<int, int> emptyPosition = locateNumber(tile.layout, 0); 
 
    const int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; 
 
    for (const auto& move : moves) { 
        int newI = emptyPosition.first + move[0]; 
        int newJ = emptyPosition.second + move[1]; 
 
        if (isValid(newI, newJ)) { 
            vector<vector<int>> newLayout = tile.layout; 
            swap(newLayout[emptyPosition.first][emptyPosition.second], newLayout[newI][newJ]); 
 
            int newCost = tile.totalCost + 1; 
            int newHeuristic = computeManhattan({newLayout, 0, 0, 0, nullptr}); 
            int newMoves = tile.movesTaken + 1; 
 
            tiles.emplace_back(newLayout, newCost, newHeuristic, newMoves, &tile); 
        } 
    } 
 
    return tiles; 
} 
 
void executeAStar(const vector<vector<int>>& initialLayout) { 
    GameTile initialState{initialLayout, 0, computeManhattan({initialLayout, 0, 0, 0, nullptr}), 0, nullptr}; 
 
    priority_queue<GameTile, vector<GameTile>, GameTileComparator> openTiles; 
 
    unordered_set<GameTile, GameTileHash> closedTiles; 
 
    openTiles.push(initialState); 
 
    while (!openTiles.empty()) { 
        GameTile currentTile = openTiles.top(); 
        openTiles.pop(); 
 
        if (currentTile.heuristicValue == 0) { 
            cout << "Goal state reached in " << currentTile.movesTaken << " moves." << endl; 
            cout << "Solution:" << endl; 
 
            while (currentTile.prevTile != nullptr) { 
                displayBoard(currentTile.layout); 
                currentTile = *currentTile.prevTile; 
            } 
            displayBoard(initialState.layout); 
            return; 
        } 
 
        closedTiles.insert(currentTile); 
 
        vector<GameTile> successors = generateTiles(currentTile); 
 
        for (const GameTile& successor : successors) { 
            if (closedTiles.find(successor) == closedTiles.end()) { 
                openTiles.push(successor); 
            } 
        } 
    } 
 
    cout << "Goal state not reachable." << endl; 
} 
 


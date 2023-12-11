#include <bits/stdc++.h> 
#include <chrono> 
 
using namespace std; 
using namespace chrono; 
 
#define SIZE_OF_MATRIX 3 
int total_moves = 0; 
int rows[] = {1, 0, -1, 0}; 
int columns[] = {0, -1, 0, 1}; 
 
int target_matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX] = { 
    {0, 1, 2}, 
    {3, 4, 5}, 
    {6, 7, 8} 
}; 
 
struct BoardNode { 
    BoardNode *parent; 
    int matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX]; 
    int position_row, position_col; 
    int cost; 
    int level; 
    int heuristic; 
}; 
 
int displayMatrix(int matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX]) { 
    for (int i = 0; i < SIZE_OF_MATRIX; i++) { 
        for (int j = 0; j < SIZE_OF_MATRIX; j++) 
            printf("%d ", matrix[i][j]); 
        printf("\n"); 
    } 
    return 0; 
} 
 
BoardNode *generateNode(int matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX], int x, int y, int new_x, int new_y, int level, BoardNode *parent) { 
    BoardNode *node = new BoardNode; 
    node->parent = parent; 
 
    for (int i = 0; i < SIZE_OF_MATRIX; i++) { 
        for (int j = 0; j < SIZE_OF_MATRIX; j++) { 
            node->matrix[i][j] = matrix[i][j]; 
        } 
    } 
 
    int temp = node->matrix[x][y]; 
    node->matrix[x][y] = node->matrix[new_x][new_y]; 
    node->matrix[new_x][new_y] = temp; 
    node->cost = INT_MAX; 
    node->level = level; 
    node->position_row = new_x; 
    node->position_col = new_y; 
    return node; 
} 
 
int calculateCost(int matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX], int goal[SIZE_OF_MATRIX][SIZE_OF_MATRIX]) { 
    int count = 0; 
    for (int i = 0; i < SIZE_OF_MATRIX; i++) 
        for (int j = 0; j < SIZE_OF_MATRIX; j++) 
            if (matrix[i][j] != goal[i][j]) 
                count++; 
    return count; 
} 
 
int calculateHeuristic(int matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX], int goal[SIZE_OF_MATRIX][SIZE_OF_MATRIX]) { 
    int heuristic = 0; 
    for (int i = 0; i < SIZE_OF_MATRIX; i++) 
        for (int j = 0; j < SIZE_OF_MATRIX; j++) 
            if (matrix[i][j] != goal[i][j]) 
                heuristic++; 
    return heuristic; 
} 
 
int isWithinBounds(int x, int y) { 
    return (x >= 0 && x < SIZE_OF_MATRIX && y >= 0 && y < SIZE_OF_MATRIX); 
} 
 
void showBoard(BoardNode *root) { 
    if (root == NULL) 
        return; 
    showBoard(root->parent); 
    displayMatrix(root->matrix); 
    printf("\n"); 
} 
 
struct CompareBoardNodes { 
    bool operator()(const BoardNode *lhs, const BoardNode *rhs) const { 
        return lhs->heuristic > rhs->heuristic; 
    } 
}; 
 
// Function to convert the puzzle matrix into a string 
std::string matrixToString(int matrix[SIZE_OF_MATRIX][SIZE_OF_MATRIX]) { 
    std::ostringstream oss; 
    for (int i = 0; i < SIZE_OF_MATRIX; i++) { 
        for (int j = 0; j < SIZE_OF_MATRIX; j++) { 
            oss << matrix[i][j] << " "; 
        } 
    } 
    return oss.str(); 
} 
 
void solvePuzzle(int initial[SIZE_OF_MATRIX][SIZE_OF_MATRIX], int x, int y, int goal[SIZE_OF_MATRIX][SIZE_OF_MATRIX]) { 
    auto start_time = high_resolution_clock::now(); 
 
    priority_queue<BoardNode *, vector<BoardNode *>, CompareBoardNodes> pq; 
 
    unordered_set<string> visited; 
 
    BoardNode *root = generateNode(initial, x, y, x, y, 0, NULL); 
    root->cost = calculateCost(initial, goal); 
    root->heuristic = calculateHeuristic(initial, goal); 
    pq.push(root); 
 
    while (!pq.empty()) { 
        BoardNode *min = pq.top(); 
        pq.pop(); 
 
        if (min->cost == 0) { 
            auto end_time = high_resolution_clock::now(); 
            auto duration = duration_cast<milliseconds>(end_time - start_time); 
 
            cout << "Reached the goal state in " << total_moves << " moves.\n"; 
            cout << "Time taken: " << duration.count() << " milliseconds\n"; 
            showBoard(min); 
            return; 
        } 
 
        for (int i = 0; i < 4; i++) { 
            if (isWithinBounds(min->position_row + rows[i], min->position_col + columns[i])) { 
                BoardNode *child = generateNode(min->matrix, min->position_row, min->position_col, min->position_row + rows[i], min->position_col + columns[i], min->level + 1, min); 
                child->cost = calculateCost(child->matrix, goal); 
                child->heuristic = calculateHeuristic(child->matrix, goal); 
 
                if (visited.find(matrixToString(child->matrix)) == visited.end()) { 
                    pq.push(child); 
                    visited.insert(matrixToString(child->matrix)); 
                    total_moves++; 
                } 
            } 
        } 
    } 
} 
 
int main() { 
    int initial[SIZE_OF_MATRIX][SIZE_OF_MATRIX]; 
 
    cout << "\n\t\t----------------------------------------------------------------------------\n"; 
    cout << " Enter the initial state of the puzzle in this format \n"; 
    cout << "*** 2 3 1 5 6 0 8 4 7 ***\n>> "; 
 
    for (int i = 0; i < SIZE_OF_MATRIX; i++) 
        for (int j = 0; j < SIZE_OF_MATRIX; j++) 
            cin >> initial[i][j]; 
 
    cout << "The entered initial puzzle is:  \n>> "; 
    displayMatrix(initial); 
    cout << "\n\t\t----------------------------------------------------------------------------\n"; 
    cout << "Solving the Puzzle  \n>> "; 
 
    int start_row = 1, start_col = 2; 
 
    solvePuzzle(initial, start_row, start_col, target_matrix); 
 
    return 0; 
} 


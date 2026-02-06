#include "LifeBase.h"

// Constructor
LifeBase::LifeBase() {
    grid = nullptr;
    nextGrid = nullptr;
    rows = 0;
    cols = 0;
}

// Destructor
LifeBase::~LifeBase() {
    // TODO: Free the memory allocated for 'grid' and 'nextGrid'.
    // Reminder: You must delete each row array individually before deleting the main array.
    if (grid != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] grid[i];
        }
        delete[] grid;
    }
    if (nextGrid != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] nextGrid[i];
        }
        delete[] nextGrid;
    }
}

void LifeBase::initializeFromFile(const std::string& filename) {
    // TODO:
    // 1. Open the file.
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    // Free existing memory if it was already allocated (prevents memory leak if called multiple times)
    if (grid != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
    if (nextGrid != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] nextGrid[i];
        }
        delete[] nextGrid;
        nextGrid = nullptr;
    }
    
    // 2. Read lines, ignoring those starting with '#' (comments).
    std::string line;

    // 3. Read the first two non-comment numbers as 'rows' and 'cols'.
    while (std::getline(file, line)) {
        if (line[0] != '#') {
            rows = std::stoi(line);
            std::getline(file, line); // read the next line
            cols = std::stoi(line);
            break;
        }
    }
    // 4. Allocate memory for 'grid' and 'nextGrid' based on these dimensions.
    grid = new int*[rows]; // grid is m(rows) int pointers(int*), each pointer is an array

    // 5. Read the grid data ('.' or '-' for dead, 'X' for alive) and populate 'grid'.
    for (int i = 0; i < rows; i++) {
        std::getline(file, line);
        grid[i] = new int[cols];

        for (int j = 0; j < cols; j++) {
            grid[i][j] = (line[j]=='.' || line[j]=='-') ? 0 : 1;
        }
    }
    //    - Initialize 'nextGrid' to all zeros.
    nextGrid = new int*[rows];
    for (int i = 0; i < rows; i++) {
        nextGrid[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            nextGrid[i][j] = 0;
        }
    }
    //    - Live cells should start with age 1.

}

// For Autograder: DO NOT EDIT
void LifeBase::printBoard() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] > 0) {
                std::cout << "X";
            } else {
                std::cout << "-";
            }

            if (j < cols - 1) {
                std::cout << " "; 
            }
        }
        std::cout << std::endl;
    }
}

// For Autograder: DO NOT EDIT
void LifeBase::printBoardWithAge() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] > 0) {
                std::cout << grid[i][j];
            } else {
                std::cout << '-';
            }

            if (j < cols - 1) {
                std::cout << " "; 
            }
        }
        std::cout << std::endl;
    }
}

void LifeBase::swapGrids() {
    // TODO: next generation becomes the current generation
    // Swap the pointers - this is much more efficient than copying all the data
    int** temp = grid;
    grid = nextGrid;
    nextGrid = temp;
    //zero out the nextGrid
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            nextGrid[i][j] = 0;
        }
    }
}
void LifeBase::update() {
    // default behavior, or empty
}
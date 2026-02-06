#include "Simulation.h"

void Simulation::update() {
    // TODO: Implement the 4 rules of the simulation.
    // 1. Iterate through every cell (r, c) in 'grid'.
    // 2. Count neighbors using countNeighbors(r, c).
    // 3. Apply rules:
    //    - 0-1 neighbors: Die (set nextGrid[r][c] to 0)
    //    - 2 neighbors: Stabilize (copy grid[r][c] to nextGrid[r][c])
    //    - 3 neighbors: Birth/Survive (set nextGrid[r][c] to grid[r][c] + 1)
    //    - 4+ neighbors: Die (set nextGrid[r][c] to 0)
    // 4. Call swapGrids() at the end.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int neighbors = countNeighbors(i, j);
            if (neighbors < 2 || neighbors > 3) {// underpopulation or overpopulation
                nextGrid[i][j] = 0;
            } else if (grid[i][j] > 0 && (neighbors == 2 || neighbors == 3)) {// live cell with 2 or 3 neighbors survives
                nextGrid[i][j] = grid[i][j]+1;
            } else if (grid[i][j]==0 && neighbors == 3) {// dead cell with 3 neighbors becomes alive
                nextGrid[i][j] = 1;
            }
        }
    }
    swapGrids();
}

int Simulation::countNeighbors(int r, int c) {
    int count = 0;
    // TODO: Check the 8 surrounding neighbors.
    
    // we'll do this column by column
    if (c>0){// if c>0, then there is a left column
        if (r>0 && grid[r-1][c-1]>0) count++; 
        if (grid[r][c-1]>0) count++;
        if (r<rows-1 && grid[r+1][c-1]>0) count++;
    }
    // if r>0, then there is a top row
    if (r>0 && grid[r-1][c]>0) count++;
    // if r<rows-1, then there is a bottom row
    if (r<rows-1 && grid[r+1][c]>0) count++;
    
    if (c<cols-1){// if c<cols-1, then there is a right column
        if (r>0 && grid[r-1][c+1]>0) count++;
        if (grid[r][c+1]>0) count++;
        if (r<rows-1 && grid[r+1][c+1]>0) count++;
    }   
    return count;
}
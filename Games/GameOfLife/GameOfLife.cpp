// Imports
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#define ALIVE 'O'
#define DEAD  ' '
#define BOUND 5

/**
 * @brief Clear the screen with a 60ms timeout.
 */
void clearScreen() {
    // Implement a clear delay
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

    system("cls");
}

/**
 * @brief Display the grid in the command line. The printing method
 * builds the string buffer, which is dumped to the console. When printing
 * the board, we remove the first/last 5 rows and columns
 *
 * @param grid state of the game to print
 */
void printBoard(std::vector<std::vector<char>> grid) {
    // Create the buffer
    std::string buffer;
    buffer.reserve(grid.size() * grid[0].size() * 2);

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            // Do not display board ends
            if (((i >= BOUND) && (i <= (grid.size()-1)-BOUND)) &&
                ((j >= BOUND) && (j <= (grid[i].size()-1)-BOUND))) {

                buffer += grid[i][j];
            }
        }

        // Only newline when grid is printed
        if ((i >= BOUND) && (i <= (grid.size()-1)-BOUND)) {
            buffer += '\n';
        }
    }

    std::cout << buffer;
}

/**
 * @brief Update the board state given the Game of Life rules.
 *
 * @param grid current state of the game
 * @return new state of the game
 */
std::vector<std::vector<char>> updateBoard(std::vector<std::vector<char>> grid) {
    // Game of life rules
    // 1. Any LIVE cell with less than 2 neighbors dies
    // 2. Any LIVE cell with two or three neighbors lives to the next generation
    // 3. Any LIVE cell with more than 3 neighbors dies
    // 4. Any DEAD cell with exactly 3 live neighbors becomes a live cell

    auto newGrid = grid;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            // Central grid processing
            if (i != 0 && i != (grid.size()-1) && j != 0 && j != (grid[i].size()-1)) {
                int neighbors = 0;

                neighbors += (grid[i-1][j-1] == ALIVE) ? 1 : 0;
                neighbors += (grid[i-1][j] == ALIVE) ? 1 : 0;
                neighbors += (grid[i-1][j+1] == ALIVE) ? 1 : 0;
                neighbors += (grid[i][j-1] == ALIVE) ? 1 : 0;
                neighbors += (grid[i][j+1] == ALIVE) ? 1 : 0;
                neighbors += (grid[i+1][j-1] == ALIVE) ? 1 : 0;
                neighbors += (grid[i+1][j] == ALIVE) ? 1 : 0;
                neighbors += (grid[i+1][j+1] == ALIVE) ? 1 : 0;

                // Process alive cell rules
                if (grid[i][j] == ALIVE) {
                    if (neighbors < 2 || neighbors > 3) {
                        newGrid[i][j] = DEAD;
                    }
                }
                // Process dead cell rules
                else {
                    if (neighbors == 3) {
                        newGrid[i][j] = ALIVE;
                    }
                }
            }
        }
    }

    return newGrid;
}

/**
 * @brief Plays the Game of Life simulation.
 *
 * @param argc number of CMD args
 * @param argv array of CMD args
 * @return return code
 */
int main (int argc, char *argv[]) {
    srand(time(0));

    // Chance initial cell is alive
    const double ALIVE_THRESHOLD = 0.5;

    // Default grid size
    int W = 150; // Width
    int L = 50;  // Length

    // Update the grid size if params are provided
    if (argc == 3) {
        L = std::stoi(argv[1]);
        W = std::stoi(argv[2]);
    }

    // Initialize the game board
    std::vector<std::vector<char>> grid(W, std::vector<char>(L));

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < L; j++) {
            // Determine if the initial cell is alive
            grid[i][j] = ((((double)rand()) / RAND_MAX) <= ALIVE_THRESHOLD) ? ALIVE : DEAD;
        }
    }

    // Continue processing the game.
    // CNTRL-C will cancel the game in CMD
    while (true) {
        // Start by printing the board
        printBoard(grid);

        // Update grid state
        grid = updateBoard(grid);

        clearScreen();
    }

    return 0;
}
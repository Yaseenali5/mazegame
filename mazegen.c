#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100  // Define maximum size of the maze

// Structs
typedef struct {
    char grid[MAX_SIZE][MAX_SIZE];  // 2D array for the maze structure
    int visited[MAX_SIZE][MAX_SIZE];  // 2D array to track visited cells
    int width, height;  // Dimensions of the maze
} Maze;

// Initialize Maze with unvisited walls 
void initializeMaze(Maze *maze, int width, int height) {
    maze->width = width;
    maze->height = height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze->grid[i][j] = '#';  // Fill with walls
            maze->visited[i][j] = 0;  // Mark cells as unvisited
        }
    }
}

// Recursive Backtracking carve function
void carveMaze(Maze *maze, int x, int y) {
    int dx[4] = {1, -1, 0, 0};  // Direction arrays for moving horizontally
    int dy[4] = {0, 0, 1, -1};  // Direction arrays for moving vertically
    int dir[4] = {0, 1, 2, 3};  // Array to hold direction indices

    // Shuffle directions to add randomness and variety
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = dir[i];
        dir[i] = dir[j];
        dir[j] = temp;
    }

    maze->visited[y][x] = 1;  // Mark the current cell as visited
    maze->grid[y][x] = ' ';  // Carve a path at the current cell

    // Recursively carve paths in the maze
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[dir[i]] * 2;
        int ny = y + dy[dir[i]] * 2;

        // Check bounds and if the cell is unvisited
        if (nx >= 0 && nx < maze->width && ny >= 0 && ny < maze->height && maze->visited[ny][nx] == 0) {
            maze->grid[ny - dy[dir[i]]][nx - dx[dir[i]]] = ' ';  // Carve path between cells
            carveMaze(maze, nx, ny);
        }
    }
}

// Function Placing E and S at the start of the maze
void placeStartAndExit(Maze *maze) {
    // Randomly place E and S in open spaces only
    int sx = rand() % maze->width;
    int sy = rand() % maze->height;
    while (maze->grid[sy][sx] != ' ') {
        sx = rand() % maze->width;
        sy = rand() % maze->height;
    }
    maze->grid[sy][sx] = 'S';

    // Randomly place the exit as long as it is not the start location
    int ex = rand() % maze->width;
    int ey = rand() % maze->height;
    while ((maze->grid[ey][ex] != ' ') || (sx == ex && sy == ey)) {
        ex = rand() % maze->width;
        ey = rand() % maze->height;
    }
    maze->grid[ey][ex] = 'E';
}

// Function to print the maze to the console
void printMaze(const Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            putchar(maze->grid[i][j]);
        }
        putchar('\n');
    }
}

// Main
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <width> <height>\n", argv[0]);
        return 1;  // Return error if command-line arguments are incorrect
    }

    int width = atoi(argv[2]);  // Convert width from string to int
    int height = atoi(argv[3]);  // Convert height from string to int
    if (width < 5 || width > MAX_SIZE || height < 5 || height > MAX_SIZE) {
        fprintf(stderr, "Error: Width and height must be between 5 and %d.\n", MAX_SIZE);
        return 1;  // Validate dimensions
    }

    Maze maze;
    initializeMaze(&maze, width, height);  // Initialize the maze
    srand(time(NULL));  // Seed random number generator
    carveMaze(&maze, 1, 1);  // Begin carving maze
    placeStartAndExit(&maze);  // Place start and exit

    // Open a file to save the maze
    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        perror("File openning error");
        return 1;
    }
    for (int i = 0; i < maze.height; i++) {
        fwrite(maze.grid[i], 1, maze.width, file);  // Write each row of the maze to the file
        fputc('\n', file);  // Add a newline after each row
    }
    fclose(file);  // Close the file

    return 0;
}
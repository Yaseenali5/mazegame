// Headers 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maze constants for maze dimensions
#define MAX_SIZE 100
#define MIN_SIZE 5

// Structs to contain the maze grid, dimensions, player position, map visibility and exit count
typedef struct {
    char maze[MAX_SIZE][MAX_SIZE]; // 2D array to store maze characters
    int width, height;             // Maze Dimensions
    int player_x, player_y;        // Current position of the player in the maze
    int showMap;                   // Flag to toggle map visibility
    int exitCount;                 // Counter for the number of exits in the maze
} Maze;

// Function prototypes
int loadMaze(Maze *maze, const char *filename);
void printMaze(const Maze *maze);
void movePlayer(Maze *maze, char direction);
int validateMazeDimensions(int width, int height);
void findStartPosition(Maze *maze);
int checkForExit(const Maze *maze);
void handleMapCommand(Maze *maze); 

// Function to load a maze from a given file
int loadMaze(Maze *maze, const char *filename) {
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (file == NULL) {
        perror("Error opening file");  // Print error
        return 2;
    }

    int firstChar = fgetc(file); // Read the first character to check for an empty file
    if (firstChar == EOF) {
        printf("Error: File is empty\n"); // Print error
        fclose(file);
        return 2;
    }
    rewind(file); // Resets file pointer for parsing

    char line[MAX_SIZE + 1]; // Buffer to hold each line from the file
    int width = 0, height = 0; // Variables to store maze dimensions
    maze->exitCount = 0; // Starts exit count for later function

    // Reads each line from the file and process it
    while (fgets(line, sizeof(line), file) != NULL) {
        if (height == 0) {
            width = strlen(line) - 1; // Determines maze width
            if (line[width] == '\n') line[width] = '\0'; // Remove newline character
        }
        strncpy(maze->maze[height], line, width); // Copy the line into the maze structure
        for (int i = 0; i < width; i++) {
            char ch = line[i];
            if (ch == 'E') maze->exitCount++; // Count exits as a condition to validate exit count
            if ((ch >= 'A' && ch <= 'Z') && (ch != 'S' && ch != 'E')) { // Check for invalid characters
                printf("Error: Invalid maze character '%c'.\n", ch);
                fclose(file);
                return 3;
            }
        }
        height++; // Increment the height after processing each line
        if (height > MAX_SIZE) {
            printf("Error: Maze exceeds maximum height\n");
            fclose(file);
            return 3; 
        }
    }
    maze->width = width;
    maze->height = height;

    fclose(file); // Close the file after reading

    if (!validateMazeDimensions(width, height) || maze->exitCount != 1) {
        printf("Error: Invalid maze dimensions or incorrect number of exits.\n");
        return 3;  
    }

    findStartPosition(maze); // Locate the starting position in the maze
    return 0; 
}

// Function to display the maze on the console with the playeer on it
void printMaze(const Maze *maze) {
    printf("\n"); 
    // Logic to go through maze array and find player position
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (i == maze->player_y && j == maze->player_x) {
                printf("X");  // Print player position at X
            } else {
                if (maze->maze[i][j] == 'S' && !(i == maze->player_y && j == maze->player_x)) {
                    printf("S"); // Print S instead with priority over X if at the same location
                } else {
                    printf("%c", maze->maze[i][j]);  // Print maze character
                }
            }
        }
        printf("\n"); 
    }
    printf("\n"); 
}

// Function to move the player in the maze based on input direction
void movePlayer(Maze *maze, char direction) {
    int newX = maze->player_x; // New X coordinate after move
    int newY = maze->player_y; // New Y coordinate after move

    // Updates coordinates based on direction
    switch (direction) {
        case 'W': case 'w': newY -= 1; break; // Up
        case 'S': case 's': newY += 1; break; // Down
        case 'A': case 'a': newX -= 1; break; // Left
        case 'D': case 'd': newX += 1; break; // Right
        default:
            printf("Invalid movement command.\n"); // Print Error for invalid command
            return; 
    }

    // Check for wall and collisions with error message
    if (newX < 0 || newX >= maze->width || newY < 0 || newY >= maze->height) {
        printf("You can't move outside the maze\n");
        return; 
    } else if (maze->maze[newY][newX] == '#') {
        printf("You can't walk through walls\n");
        return; 
    }

    // Updates player position
    maze->player_x = newX;
    maze->player_y = newY;
}

// Function to validate the dimensions of the maze against earlier constants
int validateMazeDimensions(int width, int height) {
    if (width < MIN_SIZE || width > MAX_SIZE || height < MIN_SIZE || height > MAX_SIZE) {
        printf("Invalid maze dimensions: Width and height must be between %d and %d.\n", MIN_SIZE, MAX_SIZE);
        return 0;  
    }
    return 1;  
}

// Function to find the starting position of the player in the maze
void findStartPosition(Maze *maze) {
    int startCount = 0;  // Counter for start positions found
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (maze->maze[i][j] == 'S') {
                maze->player_x = j;  // Set player x-coordinate
                maze->player_y = i;  // Set player y-coordinate
                startCount++;  // Increments start position counter
            }
        }
    }
    if (startCount == 0) {
        printf("Error: Start position 'S' not found in the maze\n");
        exit(EXIT_FAILURE);  // Exit with error if no start found
    } else if (startCount > 1) {
        printf("Error: Multiple start positions 'S' found in the maze\n");
        exit(EXIT_FAILURE);  // Exit with error if multiple starts found
    }
}

// Function to check if the player has reached the exit
int checkForExit(const Maze *maze) {
    return (maze->maze[maze->player_y][maze->player_x] == 'E');  // Return true if player is on the exit
}

// Function to toggle the visibility of the map
void handleMapCommand(Maze *maze) {
    maze->showMap = !maze->showMap;  // Toggle the showMap
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <maze_file>\n", argv[0]);  // Prompt correct usage if wrong number of arguments
        return 1;  
    }

    Maze maze = {0};  // Starts the maze structure
    int result = loadMaze(&maze, argv[1]);  // Load the maze from the file
    if (result != 0) {
        return result; 
    }

    int gameRunning = 1;  // Game control flag
    char command;  // Variable to store inputs
    while (gameRunning) {
        if (maze.showMap) {
            printMaze(&maze);  // Print the maze if map visibility is enabled
        }

        printf("Enter command WASD to move, M to show/hide map: ");
        scanf(" %c", &command);  // Read player command

        switch (command) {
            case 'W': case 'w':
            case 'A': case 'a':
            case 'S': case 's':
            case 'D': case 'd':
                movePlayer(&maze, command);  // Move player based on command
                break;
            case 'M': case 'm':
                handleMapCommand(&maze);  // Toggle map visibility
                break;
            default:
                printf("Invalid command. Please enter WASD\n");  // Handle invalid commands
        }

        if (checkForExit(&maze)) {
            printf("Congratulations! You've found the exit!\n");  // Notify player of winning
            gameRunning = 0;  // End game after finding exit
        }
    }

    return 0;  
}
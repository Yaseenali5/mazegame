# Maze Game and Maze Generator

This repository contains a maze game and a maze generator written in C. The maze game allows players to navigate through a maze loaded from a file, while the maze generator creates random mazes and saves them to a file.

## Table of Contents

- [Project Description](#project-description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Project Description

### Maze Game

The maze game loads a maze from a file and allows the player to navigate through it using keyboard commands. The goal is to find the exit ('E') starting from the start position ('S').

### Maze Generator

The maze generator creates random mazes using recursive backtracking. It ensures the maze has a start ('S') and an exit ('E') position, and saves the generated maze to a specified file.

## Features

### Maze Game
- Load maze from a file.
- Navigate the maze using WASD keys.
- Toggle the visibility of the maze map.
- Check for walls and ensure valid movements.
- Single exit point ensures a clear objective.

### Maze Generator
- Generate random mazes with customizable dimensions.
- Ensure valid paths between the start and exit positions.
- Save generated mazes to a file.

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/your-username/maze-game-and-generator.git
    cd maze-game-and-generator
    ```

2. Compile the maze game and maze generator:
    ```sh
    gcc -o maze_game maze.c
    gcc -o maze_generator maze_generator.c
    ```

## Usage

### Maze Generator

Generate a new maze with specified dimensions and save it to a file:

```sh
./maze_generator <filename> <width> <height>

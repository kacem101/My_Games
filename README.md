# My Games

A collection of games created in C++ using the Raylib library. This repository serves as a portfolio for my small game development projects.

## Games Included

This repository currently contains the following games:

- **Connect 4:** A classic two-player strategy game where players drop colored discs into a grid.
- **Tetris:** A tile-matching puzzle video game where players must complete lines by manipulating falling geometric shapes.
- **15-Puzzle:** A classic sliding tile puzzle game where the goal is to arrange numbered tiles in numerical order.

## How to Build and Run the Games

To build and run these games, you will need to have a C++ compiler and the Raylib library set up on your system.

### Dependencies

- **Raylib:** A simple and easy-to-use library to enjoy videogames programming.

### Building on Windows (with MinGW-w64)

1.  **Install MinGW-w64:** If you don't have it, download and install MinGW-w64 from a reliable source like `msys2.org`.
2.  **Install Raylib:** Download the pre-compiled Raylib binaries for MinGW from the official Raylib website. Place the contents in a known directory (e.g., `C:\raylib`).
3.  **Compile the Game:** Navigate to the specific game's directory in your terminal (e.g., `cd 15-puzzle`). Use the following command to compile your code:

    ```bash
    g++ your_game_file.cpp -o your_game_name.exe -Ipath/to/raylib/include -Lpath/to/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static
    ```

    - **Note:** Replace `your_game_file.cpp` and `your_game_name.exe` with your actual file names.
    - Replace `path/to/raylib/include` and `path/to/raylib/lib` with the actual paths on your system.
    - For example: `g++ main.cpp -o game.exe -IC:\raylib\include -LC:\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -static`

### Building on Linux (with GCC)

1.  **Install Dependencies:** Open a terminal and run the following command to install the necessary packages:

    ```bash
    sudo apt-get install build-essential libraylib-dev
    ```

2.  **Compile the Game:** Navigate to the game's directory and use a similar command to compile:

    ```bash
    g++ your_game_file.cpp -o your_game_name -lraylib
    ```

### Adding More Games

Feel free to add more of your game projects to this repository. Please create a new sub-directory for each game project and update this `README.md` file with its name and a brief description.

## License

This project is open-source and available under the MIT License.

## Contact

- **Your Name Here**

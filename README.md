# SmeltCoal Console Game

A 2D grid-based console game built entirely in C++. The objective of the game is to navigate the map, collect raw materials, and successfully smelt them into refined bronze using the required amount of coal. 

## Tech Stack
* **Language:** C++
* **IDE:** Code::Blocks

## How to Play
1. Compile and run the game via the command line or by opening the `.cbp` file in Code::Blocks.
2. The game requires command line arguments to set up the environment:
   `[ExecutableName] <TotalRows> <TotalCols> <Number of Ores> <Amount of coal> <PlayerRow> <PlayerCol>`
3. **Controls:**
   * `W` - Move Up
   * `S` - Move Down
   * `A` - Move Left
   * `D` - Move Right
   * `Q` - Quit Game
4. **Symbols:**
   * `P` - Player
   * `@` - Tin Ore
   * `#` - Copper Ore
   * `O` - Mining Equipment
   * `S` - Smelter

## Setup & Compilation
To run this project locally, clone the repository and compile the source files:
```bash
git clone [https://github.com/Gubevu245/smeltcoal-console-game.git](https://github.com/Gubevu245/smeltcoal-console-game.git)
g++ main.cpp SmeltSpace.cpp -o SmeltGame
./SmeltGame 10 10 6 3 0 0

# Minesweeper
This is a minesweeper game You can play on terminal.
It is written in 3 different languages: C#, Python and JavaScript.

- [Minesweeper](#minesweeper)
    - [Installation](#installation)
    - [Requirements](#requirements)
    - [How to play](#how-to-play)
        - [Start a new game](#start-a-new-game)
        - [Play the game](#play-the-game)
        - [End of the game](#end-of-the-game)
    - [How it works](#how-it-works)
        - [Difficulty](#difficulty)
        - [Board](#board)
        - [Revealing cells](#revealing-cells)
        - [Win or lose](#win-or-lose)
    - [Languages](#languages)
        - [C#](#c)
        - [Python](#python)
        - [JavaScript](#javascript)

## Installation
In progress...

## Requirements
You need a terminal that supports ANSI escape codes and colours.

## How to play
### Start a new game
On start screen, you can choose the difficulty of the game. There are 3 difficulties: Easy, Medium and Hard. Choose one of them and start the game.
### Play the game
You can see grid of cells. All cells are hidden.
You can reveal the cell by moving your cursor on the cell and pressing enter.
To move the cursor Up, Down, Left, and Right arrow keys are used.
When You reveal a cell, it will either be empty or will show You a number. This number is the number of mines around the cell.
If You reveal a cell that has no mines around, it will reveal all nearby cells.
You can flag a cell by pressing `f` key. Flag prevents you from revealing the cell and marks a spot with a mine. You can unflag the cell by pressing `f` key again.
### End of the game
If You reveal a cell that has a mine, You will lose the game.
If You reveal all cells that have no mines correctly, You will win the game.

## How it works
### Difficulty
On start screen, You can choose the difficulty of the game.
There are 3 difficulties: Easy, Medium and Hard.
| Difficulty | Width | Height | Mines |
|------------|-------|--------|-------|
| Easy       | 9     | 9      | 10    |
| Medium     | 16    | 16     | 40    |
| Hard       | 30    | 16     | 99    |

You can also choose custom difficulty. You can choose the width, height and number of mines.
### Board
Board is a 2D array that contains cells. Each cell can be empty or have a mine.
While creating the board, mines are placed randomly.
After placing the mines, numbers are calculated for each cell. This number is the number of mines around the cell.
First move is always safe. Mines are set after the first move to prevent the first opened field to be a mine.
### Revealing cells
When a cell is revealed, it will show a number. This number is the number of mines around the cell.
If the cell has no mines around, meaning it is empty, it will reveal all of the cells around it.
### Win or lose
If you reveal a cell that has a mine, game will over and game will show you all of the mines.
If you reveal all cells that have no mines, you will be presented with a win screen.
You can choose to play again or exit the game on win or lose screen.

## Languages
<!---
### C
C is simple procedural programming language and it has very limited features. This make this version the game simple and straightforward. But it has its own disadvantages.
The code will have a lot of boilerplate. In C#, Python and JavaScript, you can easyly create and call toString method for a class. While in C, you have to write a separate function and call it each time you want to print a struct.--->
### C#
C# is an object-oriented programming language, meaning this version of the Minesweeper game will use classes and polymorphism.
If we do opt for using multiple classes instead, they will have different Print and Reveal methods. `Cell` class will have characteristics of tiles/cells, allowing each tile to be empty, a number or a mine tile. `Board` class will be the visual representation of the game screen. `StartScreen` class will be a welcome screen and will allow the player to select a difficulty.
### JavaScript
JavaScript is multi-paradigm programming language. Minesweeper can be written in functional programming style or object oriented programming style.
JavaScript's biggest advantage is that it has easy-to-use package system. We can simply install a package like `blessed (A high-level terminal interface library for node.js.)` to create terminal games.
### Python
Python is a high-level programming language. It known for its simplicity and batteries-included philosophy. So our Python version of Minesweeper should be simple and easy to understand.

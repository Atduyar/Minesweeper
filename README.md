# Minesweeper
This is a bast minesweeper game you can play on terminal.
It is written in 4 different languages: C, C#, Python and JavaScript.

- [Minesweeper](#minesweeper)
    - [Installation](#installation)
    - [Rquirements](#requirements)
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
        - [C](#c)
        - [C#](#c)
        - [Python](#python)
        - [JavaScript](#javascript)

## Installation
On prosses...
## Rquirements
You need terminal that supports ANSI escape codes and colors.

## How to play
### Start a new game
On start screen, you can choose the difficulty of the game. There are 3 difficulties: Easy, Medium and Hard. Choose one of them and start the game.
### Play the game
You can see grid of cells. All of cells are hidden.
You can reveal the cell by moving your cursor on the cell and press enter.
Up Down Left Right keys are used to move the cursor.
When you reveal a cell, it will show you a number. This number is the number of mines around the cell.
If you reveal a cell that has no mines around, it will reveal all of the cells around it.
You can flag a cell by pressing `f` key. Flag prevents you to reveal the cell. You can unflag the cell by pressing `f` key again.
### End of the game
If you reveal a cell that has a mine, you will lose the game.
If you reveal all of the cells that has no mine, you will win the game.

## How it works
### Difficulty
On start screen, you can choose the difficulty of the game.
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
First move is always safe. Board is created after the first move to prevent the first move to be a mine.
### Revealing cells
When a cell is revealed, it will show a number. This number is the number of mines around the cell.
If the cell has no mines around, it will reveal all of the cells around it.
### Win or lose
If you reveal a cell that has a mine, game will over and game will show you all of the mines.
If you reveal all of the cells that has no mine, you will presented with a win screen.
You can choose to play again or exit the game on win or lose screen.

## Languages
### C
C is simple procedural programming language and it has very limited features. This make this version the game simple and straightforward. But it has its own disadvantages.
The code will have a lot of boilerplate. In C#, Python and JavaScript, you can easyly create and call toString method for a class. While in C, you have to write a separate function and call it each time you want to print a struct.
### C#
C# is a object oriented programming language. So this version of Minesweeper game will use classes and polymorphism.
For example, there is a `Cell` class that is a base class for `EmptyCell` and `MineCell` classes.
This classes have different Print and Reveal methods. `EmptyCell` class will reveal all of the cells around it if it has no mines around. `MineCell` class will end the game.
### JavaScript
JavaScript is multi-paradigm programming language. Minesweeper can be written in functional programming style or object oriented programming style.
JavaScript's biggest advantage is it has easy to use package system. We can simply install a package like `blessed (A high-level terminal interface library for node.js.)` to create terminal games.
### Python
Python is a high-level programming language. And it known for its simplicity and batteries-included philosophy. So our Python version of Minesweeper will be simple and easy to understand.

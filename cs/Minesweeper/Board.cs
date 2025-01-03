using Spectre.Console;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Minesweeper
{

    //CLASS BOARD ----------------------------------------------------------------------------------------------------------------------
    public class Board
    {
        private DifficultySettings difficultySettings;
        private int openedCellCount;
        private bool isFirstMove;
        private Cell[,] cells;
        private (int X, int Y) cursor; //tuple
        private string gameStatus;


        public Board(DifficultySettings settings)
        {
            this.difficultySettings = settings;
            this.openedCellCount = 0;
            this.isFirstMove = true;
            this.gameStatus = "playing";
            this.cursor.X = 0;
            this.cursor.Y = 0;
            this.cells = new Cell[settings.width, settings.height];
            for (int x = 0; x < difficultySettings.width; x++)
            {
                for (int y = 0; y < difficultySettings.height; y++)
                {
                    this.cells[x, y] = new Cell();
                }
            }

            for (int i = 0; i < difficultySettings.mines; i++)
            {
                this.findRandomNonMineCell().setMine();
            }
            this.calculateNumbers();
        }
        Cell findRandomNonMineCell()
        {
            while (true)
            {
                Random random = new Random();
                int rndX = random.Next(0, difficultySettings.width);
                int rndY = random.Next(0, difficultySettings.height);
                if (!(cells[rndX, rndY].isMine))
                {
                    return cells[rndX, rndY];
                }
            }
        }
        //TODO: make void settings, bool isMine
        void calculateNumbers()
        {
            for (int x = 0; x < difficultySettings.width; x++)
            {
                for (int y = 0; y < difficultySettings.height; y++)
                {
                    if (cells[x, y].isMine)
                    {
                        continue;
                    }
                    int mineCount = 0;
                    //relative x, y:
                    for (int rx = Math.Max(0, x - 1); rx <= Math.Min(x + 1, difficultySettings.width - 1); rx++)
                    {
                        for (int ry = Math.Max(0, y - 1); ry <= Math.Min(y + 1, difficultySettings.height - 1); ry++)
                        {
                            if (cells[rx, ry].isMine)
                            {
                                mineCount++;
                            }
                        }
                    }
                    this.cells[x, y].setNumber(mineCount);
                }
            }
        }

        public void render()
        {
            Console.Clear();
            for (int y = 0; y < difficultySettings.height; y++)
            {
                for (int x = 0; x < difficultySettings.width; x++)
                {
                    if (x == cursor.X && y == cursor.Y)
                    {
                        var text = this.cells[x, y].ToString();
                        var temp = Console.BackgroundColor;
                        Console.BackgroundColor = Console.ForegroundColor;
                        Console.ForegroundColor = temp;
                        Console.Write(text);
                    }
                    else
                    {
                        Console.Write(this.cells[x, y].ToString());
                    }
                }
                Console.WriteLine();
            }
        }

        public void toggleFlag()
        {
            var cell = this.cells[this.cursor.X, this.cursor.Y];
            if (cell.isOpen)
            {
                return;
            }
            cell.isFlagged = !cell.isFlagged;
        }

        public void openCell()
        {
            openCell(this.cursor.X, this.cursor.X)
        }

        public void openCell(int x, int y)
        {
            if (this.gameStatus != "playing")
            {
                return;
            }
            var cell = this.cells[x, y];
            if (cell.isFlagged)
            {
                return;
            }
            if (cell.isOpen)
            {
                if (cell.number == 0)
                {
                    return;
                }

                int flagCount = 0;
                for (int rx = Math.Max(0, x - 1); rx < Math.Min(x + 1, this.difficultySettings.width - 1); x++)
                {
                    for (int ry = Math.Max(0, y - 1); ry < Math.Min(y + 1, this.difficultySettings.height - 1); y++)
                    {
                        if (this.cells[rx, ry].isFlagged)
                        {
                            flagCount++;
                        }
                    }
                }

                if (flagCount != cell.number)
                {
                    return;
                }

                for (int rx = Math.Max(0, x - 1); rx < Math.Min(x + 1, this.difficultySettings.width - 1); rx++)
                {
                    for (int ry = Math.Max(0, y - 1); ry < Math.Min(y + 1, this.difficultySettings.height - 1); ry++)
                    {
                        if (!this.cells[rx, ry].isFlagged && !this.cells[rx, ry].isOpen)
                        {
                            this.openCell(rx, ry);
                        }
                    }
                }

                if (this.isFirstMove)
                {
                    this.isFirstMove = false;
                    if (cell.isMine)
                    {
                        cell.isMine = false;
                        this.findRandomNonMineCell().setMine();
                        this.calculateNumbers();
                    }
                }
                else if (cell.number==0)
                {
                    for (int rx = Math.Max(0, x - 1); rx < Math.Min(x + 1, this.difficultySettings.width - 1); rx++)
                    {
                        for (int ry = Math.Max(0, y - 1); ry < Math.Min(y + 1, this.difficultySettings.height - 1); ry++)
                        {
                            if (this.cells[rx, ry].isOpen)
                            {
                                continue;
                            }
                            this.openCell(rx, ry);
                        }
                    }
                }
                if (this.openedCellCount==this.difficultySettings.width * this.difficultySettings.height-this.difficultySettings.mines)
                {
                    this.gameStatus = "victory";
                }




            }
        }

        public void moveCursorUp()
        {
            this.cursor.Y = Math.Max(0, this.cursor.Y - 1);
        }

        public void moveCursorDown()
        {
            this.cursor.Y = Math.Min(this.difficultySettings.height - 1, this.cursor.Y + 1);
        }

        public void moveCursorLeft()
        {
            this.cursor.X = Math.Max(0, this.cursor.X - 1);
        }

        public void moveCursorRight()
        {
            this.cursor.X = Math.Min(this.difficultySettings.width - 1, this.cursor.X + 1);
        }





        //HOME SCREEN
        //Static texts:
        private readonly String WelcomeMessage = "MINESWEEPER";
        private readonly String Rules = "In this game you have to find all mines by flagging them using 'F' key on the selected spot. " +
            "Find mines by looking at the numbers that appear on the open fields. To open a field press ENTER key. " +
            "For each number a mine is hiding somewhere in the surrounding 3x3 area of that number. " +
            "The number shows how much mines are around. If you open a mine, you lose. Good luck! \n \n" +
            "First select a difficulty. " +
            "For each difficulty the board and the number of mines will be bigger. " +
            "Select by entering the following keys and pressing enter: ";
        private readonly String SelectionEasy = "EASY - 1 (9X9, 10 MINES)";
        private readonly String SelectionNormal = "NORMAL - 2 (16X16, 40 MINES)";
        private readonly String SelectionHard = "HARD - 3 (30X16, 99 MINES)";



    }
    //END OF BOARD----------------------------------------------------------------------




    public enum Difficulty
    {
        Easy, Medium, Hard, Custom
    }

    //CLASS DIFFICULTYSELECTION----------------------------------------------------------------------------------------------------
    public class DifficultySettings
    {

        public Difficulty difficulty;
        public int width;
        public int height;
        public int mines;
        public DifficultySettings(int width, int height, int mines)
        {
            this.difficulty = Difficulty.Custom;
            this.width = width;
            this.height = height;
            this.mines = mines;
        }
        public DifficultySettings(Difficulty difficulty)
        {
            this.difficulty = difficulty;

            switch (difficulty)
            {
                case Difficulty.Easy:
                    this.width = 9;
                    this.height = 9;
                    this.mines = 10;
                    break;
                case Difficulty.Medium:
                    this.width = 16;
                    this.height = 16;
                    this.mines = 40;
                    break;
                case Difficulty.Hard:
                    this.width = 30;
                    this.height = 16;
                    this.mines = 99;
                    break;
                default:
                    throw new ArgumentException("Invalid input");
                    break;
            }
        }
        public readonly String WrongDifficultyInputTxt = "You entered an invalid difficulty level! Please try again";
    }

    //CLASS CELL ----------------------------------------------------------------------------------------------------------------------
    public class Cell
    {
        public int number { get; set; }
        public bool isMine { get; set; }
        public bool isFlagged { get; set; }
        public bool isOpen { get; set; }



        public Cell(int number, bool isMine)
        {
            this.number = number;
            this.isMine = isMine;
            this.isFlagged = false;
            this.isOpen = false;
        }

        public Cell() : this(-1, false)
        {

        }

        public override string ToString()
        {
            Console.BackgroundColor = ConsoleColor.Black;
            if (this.isFlagged)
            {
                Console.ForegroundColor = ConsoleColor.Green;
                return " F ";
            }
            else if (!this.isOpen)
            {
                Console.ForegroundColor = ConsoleColor.White;
                return " # ";
            }
            else
            {
                if (this.isMine)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    return " B ";
                }
                else
                {
                    switch (this.number)
                    {
                        case 0:
                            Console.ForegroundColor = ConsoleColor.Black;
                            break;
                        case 1:
                            Console.ForegroundColor = ConsoleColor.Gray;
                            break;
                        case 2:
                            Console.ForegroundColor = ConsoleColor.Blue;
                            break;
                        case 3:
                            Console.ForegroundColor = ConsoleColor.Magenta;
                            break;
                        default:
                            Console.ForegroundColor = ConsoleColor.Red;
                            break;
                    }
                    return this.number.ToString();
                }
            }
        }

        public void setMine()
        {
            this.isMine = true;
            this.number = -1;
        }

        public void setNumber(int number)
        {
            this.number = number;
            this.isMine = false;
        }
    }
}

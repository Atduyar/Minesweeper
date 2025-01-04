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
	public enum GameStatus
	{
		Playing,
		GameOver,
		Victory
	}

	public class Board
	{
		public DifficultySettings Settings { get => difficultySettings; }
		public int OpenedCellCount { get => openedCellCount; }
		public int CellMustBeOpened { get => difficultySettings.width * difficultySettings.height - difficultySettings.mines; }
		public GameStatus Status { get => gameStatus; }

		private DifficultySettings difficultySettings;
		private int openedCellCount;
		private bool isFirstMove;
		private Cell[,] cells;
		private (int X, int Y) cursor; //tuple
		private GameStatus gameStatus;

		public Board(DifficultySettings settings)
		{
			this.difficultySettings = settings;
			this.openedCellCount = 0;
			this.isFirstMove = true;
			this.gameStatus = GameStatus.Playing;
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
				Console.ResetColor();
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
			this.openCell(this.cursor.X, this.cursor.Y);
		}

		public void openCell(int x, int y)
		{
			if (this.gameStatus != GameStatus.Playing)
			{
				return;
			}
			var cell = this.cells[x, y];
			if (cell.isFlagged)
			{
				return;
			}

			// smartReveal
			if (cell.isOpen)
			{
				if (cell.number == 0)
				{
					return;
				}

				int flagCount = 0;
				for (int rx = Math.Max(0, x - 1); rx <= Math.Min(x + 1, this.difficultySettings.width - 1); rx++)
				{
					for (int ry = Math.Max(0, y - 1); ry <= Math.Min(y + 1, this.difficultySettings.height - 1); ry++)
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

				for (int rx = Math.Max(0, x - 1); rx <= Math.Min(x + 1, this.difficultySettings.width - 1); rx++)
				{
					for (int ry = Math.Max(0, y - 1); ry <= Math.Min(y + 1, this.difficultySettings.height - 1); ry++)
					{
						if (!this.cells[rx, ry].isFlagged && !this.cells[rx, ry].isOpen)
						{
							this.openCell(rx, ry);
						}
					}
				}
				return;
			}

			if (this.isFirstMove)
			{
				this.isFirstMove = false;
				if (cell.isMine)
				{
					this.findRandomNonMineCell().setMine();
					cell.isMine = false;
					this.calculateNumbers();
				}
			}

			// mine is opininggg
			cell.isOpen = true;

			if (cell.isMine)
			{
				this.gameStatus = GameStatus.GameOver;
				revealAllBomb();
				return;
			}

			// autoReveal
			if (cell.number == 0)
			{
				for (int rx = Math.Max(0, x - 1); rx <= Math.Min(x + 1, this.difficultySettings.width - 1); rx++)
				{
					for (int ry = Math.Max(0, y - 1); ry <= Math.Min(y + 1, this.difficultySettings.height - 1); ry++)
					{
						if (this.cells[rx, ry].isOpen)
						{
							continue;
						}
						this.openCell(rx, ry);
					}
				}
			}
			this.openedCellCount++;
			if (this.openedCellCount == this.difficultySettings.width * this.difficultySettings.height - this.difficultySettings.mines)
			{
				this.gameStatus = GameStatus.Victory;
			}
		}

		private void revealAllBomb()
		{
			for (int x = 0; x < this.difficultySettings.width; x++)
			{
				for (int y = 0; y < this.difficultySettings.height; y++)
				{
					var cell = this.cells[x, y];
					if (cell.isMine)
					{
						cell.isOpen = true;
						cell.isFlagged = false;
					}
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

	}

	public enum Difficulty
	{
		Easy, Medium, Hard, Custom
	}

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
			}
		}
		public readonly String WrongDifficultyInputTxt = "You entered an invalid difficulty level! Please try again";
	}

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
			Console.ResetColor();
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
							Console.ForegroundColor = ConsoleColor.Gray;
							return "   ";
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
					return " " + this.number + " ";
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

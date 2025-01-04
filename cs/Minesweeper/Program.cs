using Minesweeper;

namespace Tables;

public static class Program
{
	static void cancelKeyPress_Handler(object sender, ConsoleCancelEventArgs args)
	{
		Console.CursorVisible = true;
		Environment.Exit(0);
	}

	static String Instructions = " Select difficulty by entering a number: \n";
	static String SelectionEasy = "\t1 - Easy \t(9x9,   10 mines)";
	static String SelectionNormal = "\t2 - Normal \t(16x16, 40 mines)";
	static String SelectionHard = "\t3 - Hard \t(30x16, 99 mines)";
	static String SelectionExit = "\tq - Exit";
	public static void Main()
	{
		Console.Clear();
		Console.CursorVisible = false;
		Console.CancelKeyPress += new ConsoleCancelEventHandler(cancelKeyPress_Handler);

		Board b = new Board(new DifficultySettings(Difficulty.Easy));
		string bigTitle = @"  __  __ _             _____                                  
 |  \/  (_)           / ____|                                 
 | \  / |_ _ __   ___| (_____      _____  ___ _ __   ___ _ __ 
 | |\/| | | '_ \ / _ \\___ \ \ /\ / / _ \/ _ \ '_ \ / _ \ '__|
 | |  | | | | | |  __/____) \ V  V /  __/  __/ |_) |  __/ |   
 |_|  |_|_|_| |_|\___|_____/ \_/\_/ \___|\___| .__/ \___|_|   
                                             | |              
                                             |_|              ";
		bool isMenu = true;

		while (true)
		{
			// Clear the console
			Console.Clear();

			// Menu Screen
			if (isMenu)
			{
				Console.ForegroundColor = ConsoleColor.Cyan;
				Console.WriteLine(bigTitle);
				Console.ResetColor();
				Console.WriteLine(Instructions + "\n");
				Console.WriteLine(SelectionEasy + "\n" + SelectionNormal + "\n" + SelectionHard + "\n");
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine(SelectionExit);

				var inputKey1 = Console.ReadKey();
				DifficultySettings settings = new DifficultySettings(Difficulty.Easy);
				switch (inputKey1.Key)
				{
					case ConsoleKey.NumPad1:
					case ConsoleKey.D1:
						settings = new DifficultySettings(Difficulty.Easy);
						isMenu = false;
						break;
					case ConsoleKey.NumPad2:
					case ConsoleKey.D2:
						settings = new DifficultySettings(Difficulty.Medium);
						isMenu = false;
						break;
					case ConsoleKey.NumPad3:
					case ConsoleKey.D3:
						settings = new DifficultySettings(Difficulty.Hard);
						isMenu = false;
						break;
					case ConsoleKey.Escape:
					case ConsoleKey.Q:
						Console.CursorVisible = true;
						return;// Exit
					default:
						Console.WriteLine("Wrong input");
						continue;
				}
				b = new Board(settings);
				continue;
			}


			// Game Screen
			Console.ForegroundColor = ConsoleColor.Red;
			WriteCenteredLine("Minesweeper");
			Console.ResetColor();

			switch (b.Status)
			{
				case GameStatus.GameOver:
					Console.BackgroundColor = ConsoleColor.DarkRed;
					Console.ForegroundColor = ConsoleColor.White;
					WriteCenteredLine("You Lost! Press R to restart or Q to exit");
					Console.ResetColor();
					break;
				case GameStatus.Victory:
					Console.BackgroundColor = ConsoleColor.Green;
					Console.ForegroundColor = ConsoleColor.White;
					WriteCenteredLine("You Won! Press R to restart or Q to exit");
					Console.ResetColor();
					break;
				case GameStatus.Playing:
					Console.BackgroundColor = ConsoleColor.Gray;
					Console.ForegroundColor = ConsoleColor.Black;
					WriteCenteredLine("Status: " + b.OpenedCellCount + "/" + b.CellMustBeOpened + " cells opened");
					Console.ResetColor();
					break;
			}

			// Render the board
			b.render();
			Console.BackgroundColor = ConsoleColor.Gray;
			Console.ForegroundColor = ConsoleColor.Black;
			switch (b.Status)
			{
				case GameStatus.GameOver:
					Console.BackgroundColor = ConsoleColor.DarkRed;
					Console.ForegroundColor = ConsoleColor.White;
					break;
				case GameStatus.Victory:
					Console.BackgroundColor = ConsoleColor.Green;
					Console.ForegroundColor = ConsoleColor.White;
					break;
				case GameStatus.Playing:
					Console.BackgroundColor = ConsoleColor.Gray;
					Console.ForegroundColor = ConsoleColor.Black;
					break;
			}
			WriteCenteredLine(" [Arrow keys] Move  [Space] Open  [F] Flag  [R] Restart  [Q] Quit ");
			Console.ResetColor();

			// Input
			var inputKey2 = Console.ReadKey();
			switch (inputKey2.Key)
			{
				case ConsoleKey.F:
					b.toggleFlag();
					break;
				case ConsoleKey.Enter:
				case ConsoleKey.Spacebar:
					b.openCell();
					break;
				case ConsoleKey.UpArrow:
					b.moveCursorUp();
					break;
				case ConsoleKey.DownArrow:
					b.moveCursorDown();
					break;
				case ConsoleKey.LeftArrow:
					b.moveCursorLeft();
					break;
				case ConsoleKey.RightArrow:
					b.moveCursorRight();
					break;
				case ConsoleKey.R:
					b = new Board(b.Settings);
					break;
				case ConsoleKey.Escape:
				case ConsoleKey.Q:
					isMenu = true;
					break;
			}
			// Status Line
		}
	}

	private static void WriteCenteredLine(string textToEnter)
	{
		Console.Write(String.Format("{0," + ((Console.WindowWidth / 2) + (textToEnter.Length / 2)) + "}", textToEnter));
		Console.WriteLine(String.Format("{0," + ((Console.WindowWidth / 2) - (textToEnter.Length / 2)) + "}", " "));
	}
}

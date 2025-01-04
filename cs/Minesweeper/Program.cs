using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using Minesweeper;
using Spectre.Console;

namespace Tables;

public static class Program
{
    public static void Main()
    {
        //DifficultySettings settings = new DifficultySettings(Difficulty.Easy);
        //Board b = new Board(settings);
        string bigTitle = "MINESWEEPER";
        bool isMenu = true;
        String Instructions = "Select difficulty by entering a button: ";
        String SelectionEasy = "EASY - 1 (9X9, 10 MINES)";
        String SelectionNormal = "NORMAL - 2 (16X16, 40 MINES)";
        String SelectionHard = "HARD - 3 (30X16, 99 MINES)";




        while (true)
        {
            Console.Clear();
            Console.WriteLine(bigTitle);
            Console.WriteLine(Instructions + "\n" + SelectionEasy + "\n" + SelectionNormal + "\n" + SelectionHard);

            if (isMenu)
            {
                var inputKey1 = Console.ReadKey();
                DifficultySettings settings = new DifficultySettings(Difficulty.Easy);
                switch (inputKey1.Key)
                {
                    case ConsoleKey.NumPad1:
                        settings = new DifficultySettings(Difficulty.Easy);
                        isMenu = false;
                        break;
                    case ConsoleKey.NumPad2:
                        settings = new DifficultySettings(Difficulty.Medium);
                        isMenu = false;
                        break;
                    case ConsoleKey.NumPad3:
                        settings = new DifficultySettings(Difficulty.Hard);
                        isMenu = false;
                        break;
                    case ConsoleKey.Escape:
                        Environment.Exit(0);
                        break;
                    default:
                        Console.WriteLine("Wrong input");
                        return;
                }
                //TODO:
                Board b = new Board(settings);

            }

            var inputKey2= Console.ReadKey();
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
                    Console.Clear();
                    //home recall menu TODO
                    break;
            }


            b.render();
        }
        






    }

}
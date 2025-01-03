using Minesweeper;
using Spectre.Console;

namespace Tables;

public static class Program
{
    public static void Main()
    {
        DifficultySettings settings = new DifficultySettings(Difficulty.Easy);
        Board b = new Board(settings);

        while (true)
        {
            Console.Clear();
            Console.WriteLine("intro blabla");
            //home call TODO


            var inputKey= Console.ReadKey();
            switch (inputKey.Key)
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
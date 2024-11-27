#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "libmine/include/libmine.h"
#define LOG_USE_COLOR
#define ATD_LOGGER_IMPLEMENTATION
#include "logger.h"

size_t cur_x = 0;
size_t cur_y = 0;

bool update(MinesweeperGame** g){
	MinesweeperGame* game = *g;
	switch (getch()) {
		case KEY_UP: {
			cur_y--;
		} break;
		case KEY_DOWN: {
			cur_y++;
		} break;
		case KEY_LEFT: {
			cur_x--;
		} break;
		case KEY_RIGHT: {
			cur_x++;
		} break;
		case 'F':
		case 'f': {
			toggleFlag(game, cur_x, cur_y);
		} break;
		case 'R':
		case 'r': {
			*g = refreshGame(game);
		} break;
		case '\n':
		case KEY_ENTER: {
			if (game->status.state == WAITING_FIRST_MOVE || game->status.state == PLAYING) {
				reveal(game, cur_x, cur_y);
				if (game->status.state == YOU_WIN || game->status.state == YOU_GAMEOVER) {
					revealAllMines(game);
				}
			}
		} break;
		case KEY_EXIT:
		case 'q': {
			return false;
		} break;
	}
	return true;
}

void render(MinesweeperGame* game){
	// TODO: print status
	attron(COLOR_PAIR(1));
	printw("Ahmet Tarik Duyar \n");
	for (size_t y = 0; y < game->board->height; y++) {
		for (size_t x = 0; x < game->board->width; x++) {
			Cell* c = &GET_CELL(game->board, x, y);
			int col = COLOR_PAIR(-1);
			switch (c->status) {
				case OPENED: {
					if (c->value == 'B')
						col = COLOR_PAIR(4) | A_BOLD;
					else if(c->value-'0' < 4)
						col = COLOR_PAIR(c->value-'0' + 5);
					else
						col = COLOR_PAIR(9);
				} break;
				case CLOSE: {
					col = COLOR_PAIR(0);
				} break;
				case FLAGGED: {
					col = COLOR_PAIR(3);
				} break;
			}
			if (x == cur_x && y == cur_y) {
				col = COLOR_PAIR(2) | A_BOLD;
			}
			attron(col);
			switch (c->status) {
				case OPENED: {
					printw(" %c ", c->value);
				} break;
				case CLOSE: {
					printw(" # ");
				} break;
				case FLAGGED: {
					printw(" F ");
				} break;
			}
			attroff(col);
		}
		printw("\n");
	}
	printw("---------STATUS-----------\n");
	printw("Flagged: %d\n", game->status.flagedCellCount);
	printw("Revealed: %d\n", game->status.revealedCellCount);
	printw("Time: %ld\n", time(NULL) - game->status.startTime);
	printw("State: %s\n", game->status.state == YOU_WIN ? "YOU WIN" : game->status.state == YOU_GAMEOVER ? "YOU GAMEOVER" : "PLAYING");
	printw("---------SETTINGS---------\n");
	printw("Width: %zu, Height: %zu, Mines: %u\n", game->settings.width, game->settings.height, game->settings.mines);
}

#define GRAY_0 8
#define GRAY_1 9
#define GRAY_2 10
#define GRAY_3 11
#define GRAY_4 12
int main(void) {
	srand(time(NULL));

	log_add_fp(fopen("debug.log", "a"), LOG_DEBUG);
	log_set_quiet(true);
	log_debug("Main called.");

	// GameSettings settings = { .difficulty=CUSTOM, .width=10, .height=10, .mines=100 };
	GameSettings settings = { .difficulty=CUSTOM, .width=10, .height=10, .mines=98 };
	// GameSettings settings = { .difficulty= EASY };
	// GameSettings settings = { .difficulty= MEDIUM };
	// GameSettings settings = { .difficulty= HARD };
	MinesweeperGame* game = newGame(settings);

	initscr();
	if (!has_colors()) {
        endwin();
        printf("Your terminal does not support colors\n");
        return 1;
    }
    if (!can_change_color()) {
        endwin();
        printf("Your terminal does not support color modifications\n");
        return 1;
    }
    start_color();
	use_default_colors();
    init_color(GRAY_0, 50, 50, 50);
    init_color(GRAY_1, 200, 200, 200);
    init_color(GRAY_2, 200, 200, 500);
    init_color(GRAY_3, 500, 200, 300);
    init_color(GRAY_4, 700, 200, 200);
    init_pair(1, -1, -1);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // cursor
    init_pair(3, COLOR_GREEN, -1); // flag
    init_pair(4, COLOR_RED, -1); // BOMB
    init_pair(5, GRAY_0, -1);
    init_pair(6, GRAY_1, -1);
    init_pair(7, GRAY_2, -1);
    init_pair(8, GRAY_3, -1);
    init_pair(9, GRAY_4, -1);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0); 
	bool isRuning = true;
	while (isRuning) {
		clear();
		render(game);
		refresh();
		isRuning = update(&game);
	}
	freeGame(game);
	endwin();
	return 0;
}

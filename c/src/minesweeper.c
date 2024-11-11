#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "libmine/include/libmine.h"

int cur_x = 0, cur_y =0;

bool update(MinesweeperGame* game){
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
		case '\n':
		case KEY_ENTER: {
			reveal(game, cur_x, cur_y);
		} break;
		case KEY_EXIT:
		case 'q': {
			return false;
		} break;
	}
	return true;
}

void render(MinesweeperGame* game){
	// TODO: print grid
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
					else
						col = COLOR_PAIR(c->value-'0' + 5);
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
					printw(" X ");
				} break;
				case FLAGGED: {
					printw(" F ");
				} break;
			}
			attroff(col);
		}
		printw("\n");
	}

}

#define GRAY_0 8
#define GRAY_1 9
#define GRAY_2 10
#define GRAY_3 11
#define GRAY_4 12
#define GRAY_5 13
#define GRAY_6 14
#define GRAY_7 15
#define GRAY_8 16
#define GRAY_9 17
int main(void) {
	srand(time(NULL));

	GameSettings settings = { .difficulty= EASY };
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
    init_color(GRAY_3, 700, 200, 200);
    init_color(GRAY_4, 800, 200, 200);
    // init_color(GRAY_5, 250, 250, 250);
    // init_color(GRAY_6, 250, 250, 250);
    // init_color(GRAY_7, 250, 250, 250);
    // init_color(GRAY_8, 250, 250, 250);
    // init_color(GRAY_9, 250, 250, 250);
    init_pair(1, -1, -1);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // cursor
    init_pair(3, COLOR_RED, -1); // flag
    init_pair(4, COLOR_BLUE, -1); // BOMB
    init_pair(5, GRAY_0, -1);
    init_pair(6, GRAY_1, -1);
    init_pair(7, GRAY_2, -1);
    init_pair(8, GRAY_3, -1);
    init_pair(9, GRAY_4, -1);
    // init_pair(10, GRAY_5, -1);
    // init_pair(11, GRAY_6, -1);
    // init_pair(12, GRAY_7, -1);
    // init_pair(13, GRAY_8, -1);
    // init_pair(14, GRAY_9, -1);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0); 
	bool isRuning = true;
	while (isRuning) {
		clear();
		render(game);
		refresh();
		isRuning = update(game);
	}
	endwin();
	return 0;
}

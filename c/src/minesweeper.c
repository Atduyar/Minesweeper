#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
	int x;
	int y;
} cursor;

typedef enum {
	EASY,
	MEDIUM,
	HARD,
	CUSTOM,
} difficulty;

typedef enum {
	OPENED = 0,
	CLOSE,
	FLAGGED,
} cellStatus;

typedef struct {
	char value; // '0'-'8' or 'B'.
	cellStatus status;
} cell;
//maybe it is better to use struct since you can check if specific field was open or not
// I was thinking something like typedef struct cell which has status (open / flagged / ...) and isBomb flag + if not bomb flag for how many bombs around so you can precompute it during compile time

typedef struct {
	cursor cur;
	size_t height;
	size_t width;
	difficulty dif;
	cell grid[];
} board;

void generateBoard(board* b) {
	int cellCount = b->width * b->height;
	int bombCount = cellCount/10;

	for (int i = 0; i< b->height; i++) {
		
	}

}

board* newBoard(size_t size, difficulty dif) {
	board* b = malloc(sizeof(board) + sizeof(cell)*size*size);
	b->height = size;
	b->width = size;
	b->dif = dif;

	return b;
}

cursor cur;

void print_cur(){
	mvprintw(cur.y*3+0, cur.x*6, "# # # ");
	mvprintw(cur.y*3+1, cur.x*6, "#   # ");
	mvprintw(cur.y*3+2, cur.x*6, "# # #");
}

bool update(){
	switch (getch()) {
		case KEY_UP: {
			cur.y -= 1;
		} break;
		case KEY_DOWN: {
			cur.y += 1;
		} break;
		case KEY_LEFT: {
			cur.x -= 1;
		} break;
		case KEY_RIGHT: {
			cur.x += 1;
		} break;
		case 'q': {
			return false;
		} break;
	}
	return true;
}

void render(){
	// TODO: print grid
	print_cur();
	// TODO: print status
}

int main(void) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0); 
	bool isRuning = true;
	while (isRuning) {
		clear();
		render();
		refresh();
		isRuning = update();
	}
	endwin();
	return 0;
}

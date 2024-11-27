#pragma once
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GET_CELL(b, x, y) (b->grid[(y) * b->width + (x)])

typedef enum {
	CLOSE = 0,
	OPENED,
	FLAGGED,
} CellStatus;

typedef struct {
	char value; // '0'-'8' or 'B'.
	CellStatus status;
} Cell;

typedef enum {
	EASY,
	MEDIUM,
	HARD,
	CUSTOM,
} Difficulty;

typedef struct {
	size_t height;
	size_t width;
	Cell grid[];
} Board;

typedef enum {
	WAITING_FIRST_MOVE,
	PLAYING,
	YOU_WIN,
	YOU_GAMEOVER,
} GameState;

typedef struct {
	Difficulty difficulty;
	size_t width;
	size_t height;
	uint32_t mines;
	bool smartReval;
} GameSettings;

typedef struct {
	Board* board;
	GameSettings settings;
	struct {
		time_t startTime;
		uint32_t revealedCellCount;
		uint32_t flagedCellCount;
		GameState state;
	} status;
} MinesweeperGame;

MinesweeperGame* newGame(GameSettings settings);
MinesweeperGame* refreshGame(MinesweeperGame* game);
void freeGame(MinesweeperGame* game);

void reveal(MinesweeperGame* game, int x, int y);
void revealAllMines(MinesweeperGame* game);

void placeFlag(MinesweeperGame* game, int x, int y);
void unplaceFlag(MinesweeperGame* game, int x, int y);
void toggleFlag(MinesweeperGame* game, int x, int y);

Cell* getCell(Board* board, size_t x, size_t y);


#pragma once
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MIN_MINESWEEPER_HEIGHT 5 
#define MIN_MINESWEEPER_WIDTH 5 
#define MIN_MINESWEEPER_MINE_COUNT 3 

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

void reveal(MinesweeperGame* game, uint32_t x, uint32_t y);
void revealAllMines(MinesweeperGame* game);

void placeFlag(MinesweeperGame* game, uint32_t x, uint32_t y);
void unplaceFlag(MinesweeperGame* game, uint32_t x, uint32_t y);
void toggleFlag(MinesweeperGame* game, uint32_t x, uint32_t y);

Cell* getCell(Board* board, uint32_t x, uint32_t y);


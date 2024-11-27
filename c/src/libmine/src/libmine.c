#include "../include/libmine.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

static int32_t minI32(int32_t a, int32_t b) {
	return a < b ? a : b;
}
static int32_t maxI32(int32_t a, int32_t b) {
	return a > b ? a : b;
}

Cell* getCell(Board* board, size_t x, size_t y) {
	if (x < 0 || y < 0 || x >= board->width || y >= board->height) {
		return NULL;
	}
	return &board->grid[y * board->width + x];
}

static void setupSettings(GameSettings* settings) {
	switch (settings->difficulty) {
		case EASY: {
			settings->width = 9;
			settings->height = 9;
			settings->mines = 10;
		} break;
		case MEDIUM: {
			settings->width = 16;
			settings->height = 16;
			settings->mines = 40;
		} break;
		case HARD: {
			settings->width = 30;
			settings->height = 16;
			settings->mines = 99;
		} break;
		case CUSTOM: {
			if (settings->width < 9) {
				settings->width = 9;
			}
			if (settings->height < 9) {
				settings->height = 9;
			}
			if (settings->mines < 10) {
				settings->mines = 10;
			}
		} break;
	}
}

MinesweeperGame* newGame(GameSettings settings) {
	log_debug("New Game created.");
	setupSettings(&settings);
	MinesweeperGame* game = malloc(sizeof(MinesweeperGame));
	game->settings = settings;
	game->status.startTime = time(NULL);
	game->status.state = WAITING_FIRST_MOVE;
	game->status.flagedCellCount = 0;
	game->status.revealedCellCount = 0;
	// The board will generated when the first move made.
	game->board = malloc(sizeof(Board) + settings.width * settings.height * sizeof(Cell));
	memset(game->board, 0, sizeof(Board) + settings.width * settings.height * sizeof(Cell));
	game->board->height = settings.height;
	game->board->width = settings.width;

	return game;
}

void freeGame(MinesweeperGame* game) {
	log_debug("Game freed.");
	free(game->board);
	free(game);
}

MinesweeperGame* refreshGame(MinesweeperGame* game) {
	MinesweeperGame* ng = newGame(game->settings);
	freeGame(game);
	return ng;
}

static uint32_t countAdjacentMines(MinesweeperGame* game, int x, int y) {
	uint32_t mineCount = 0;
	const size_t max_x = minI32(x+1, game->board->width-1);
	const size_t max_y = minI32(y+1, game->board->height-1);

	for (size_t ry = maxI32(y-1, 0); ry <= max_y; ry++) {
		for (size_t rx = maxI32(x-1, 0); rx <= max_x; rx++) {
			Cell* c = getCell(game->board, rx, ry);
			if (c->value == 'B')
				mineCount++;
		}
	}
	return mineCount;
}

static void logBoard(MinesweeperGame* game) {
	for (size_t y = 0; y < game->board->height; y++) {
		char* buf = malloc(sizeof(char) * game->board->width*2+1);
		buf[game->board->width*2] = '\0';
		for (size_t x = 0; x < game->board->width; x++) {
			Cell* c = &GET_CELL(game->board, x, y);
			buf[x*2] = c->value?c->value:'-';
			buf[x*2+1] = ' ';
		}
		log_debug("%s", buf);
	}
}

static void generateMines(MinesweeperGame* game, int x, int y) {
	log_debug("Generate Mines called. %d, %d", x, y);
	uint32_t avaibleCells = game->board->height * game->board->width - 1;
	uint32_t forbidenCell = y*game->board->width + x;

	// place Bombs
	// for(uint32_t remainingZeros = game->settings.mines; remainingZeros > 0 ; --remainingZeros) {
	for(uint32_t mineNeeded = game->settings.mines; mineNeeded > 0 ; --mineNeeded) {
		if (avaibleCells == 0) {
			log_debug("There are no avaible cells for new mine.");
			game->settings.mines -= mineNeeded; 
			log_debug("Mines are reduced to %d to make game playable.", game->settings.mines);
			break;
		}
		uint32_t random_cell_number = rand()%avaibleCells;

		// find random_cell_number th cell among non mined cells.
		for (size_t random_cell_pos = 0; random_cell_pos < game->board->width * game->board->height; random_cell_pos++) {
			if (GET_CELL(game->board, random_cell_pos%game->board->width, random_cell_pos/game->board->width).value == 'B') {
				continue;
			}
			if (random_cell_pos == forbidenCell) {
				continue;
			}
			if (random_cell_number == 0) {
				game->board->grid[random_cell_pos].value = 'B';
				avaibleCells--;
				break;
			}
			random_cell_number--;
		}
	}

	log_debug("Bomb Map:");
	logBoard(game);

	// place number
	for (size_t y = 0; y < game->board->height; y++) {
		for (size_t x = 0; x < game->board->width; x++) {
			Cell* c = &GET_CELL(game->board, x, y);
			if(c->value == 'B')
				continue;
			c->value = (uint8_t)'0' + countAdjacentMines(game, x, y);
		}
	}

	log_debug("Numbered Map:");
	logBoard(game);
}

void reveal(MinesweeperGame* game, int x, int y) {
	log_debug("Reveal called. %d, %d", x, y);
	if (game->status.state == WAITING_FIRST_MOVE) {
		generateMines(game, x, y);
		game->status.state = PLAYING;
	}
	Cell* cell = &GET_CELL(game->board, x, y);
	switch (cell->status) {
		case FLAGGED: {
			return;
		} break;
		case OPENED: {
			// TODO: Smart Reveal
			return;
		} break;
		case CLOSE: {
			cell->status = OPENED;
			if(cell->value == 'B') {
				log_debug("Game Over");
				game->status.state = YOU_GAMEOVER;
				return;
			}
			else if (cell->value == '0') {
				for (int32_t ry = -1; ry <= 1; ry++) {
					for (int32_t rx = -1; rx <= 1; rx++) {
						if (x+rx < 0 || x+rx >= (int32_t)game->board->width ||
							y+ry < 0 || y+ry >= (int32_t)game->board->height
						) {
							continue;
						}
						reveal(game, x+rx, y+ry);
					}
				}
			}
			game->status.revealedCellCount++;
			uint32_t neededCellCount = game->board->width * game->board->height - game->settings.mines;
			if (game->status.revealedCellCount == neededCellCount) {
				log_debug("You Win");
				game->status.state = YOU_WIN;
			}
		} break;
	}
}

void revealAllMines(MinesweeperGame* game) {
	log_debug("Reveal All Mines called.");
	for (size_t y = 0; y < game->board->height; y++) {
		for (size_t x = 0; x < game->board->width; x++) {
			Cell* c = &GET_CELL(game->board, x, y);
			if (c->value == 'B') {
				c->status = OPENED;
			}
		}
	}
}

void placeFlag(MinesweeperGame* game, int x, int y) {
	// FIX: Bounce check
	Cell* cell = &GET_CELL(game->board, x, y);
	switch (cell->status) {
		case FLAGGED: {
			return;
		} break;
		case OPENED: {
			return;
		} break;
		case CLOSE: {
			cell->status = FLAGGED;
			game->status.flagedCellCount++;
		} break;
	}
}
void unplaceFlag(MinesweeperGame* game, int x, int y) {
	// FIX: Bounce check
	Cell* cell = &GET_CELL(game->board, x, y);
	switch (cell->status) {
		case CLOSE: {
			return;
		} break;
		case OPENED: {
			return;
		} break;
		case FLAGGED: {
			cell->status = CLOSE;
			game->status.flagedCellCount--;
		} break;
	}
}

void toggleFlag(MinesweeperGame* game, int x, int y) {
	// FIX: Bounce check
	Cell* cell = &GET_CELL(game->board, x, y);
	switch (cell->status) {
		case OPENED: {
			return;
		} break;
		case CLOSE: {
			placeFlag(game, x, y);
		} break;
		case FLAGGED: {
			unplaceFlag(game, x, y);
		} break;
	}
}


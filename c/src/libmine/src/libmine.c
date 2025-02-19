#include "../include/libmine.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "logger.h"

static int32_t minI32(int32_t a, int32_t b) {
	return a < b ? a : b;
}
static int32_t maxI32(int32_t a, int32_t b) {
	return a > b ? a : b;
}

Cell* getCell(Board* board, uint32_t x, uint32_t y) {
	if (x >= board->width || y >= board->height) {
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
			if (settings->width < MIN_MINESWEEPER_WIDTH) {
				settings->width = MIN_MINESWEEPER_WIDTH;
			}
			if (settings->height < MIN_MINESWEEPER_HEIGHT) {
				settings->height = MIN_MINESWEEPER_HEIGHT;
			}
			if (settings->mines < MIN_MINESWEEPER_MINE_COUNT) {
				settings->mines = MIN_MINESWEEPER_MINE_COUNT;
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
	const uint32_t max_x = minI32(x+1, game->board->width-1);
	const uint32_t max_y = minI32(y+1, game->board->height-1);

	for (uint32_t ry = maxI32(y-1, 0); ry <= max_y; ry++) {
		for (uint32_t rx = maxI32(x-1, 0); rx <= max_x; rx++) {
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
			Cell* c = getCell(game->board, x, y);
			buf[x*2] = c->value?c->value:'-';
			buf[x*2+1] = ' ';
		}
		log_debug("%s", buf);
	}
}

static void generateMines(MinesweeperGame* game, uint32_t x, uint32_t y) {
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
			if (getCell(game->board, random_cell_pos%game->board->width, random_cell_pos/game->board->width)->value == 'B') {
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
			Cell* c = getCell(game->board, x, y);
			if(c->value == 'B')
				continue;
			c->value = (uint8_t)'0' + countAdjacentMines(game, x, y);
		}
	}

	log_debug("Numbered Map:");
	logBoard(game);
}

static void revealAllMines(MinesweeperGame* game) {
	log_debug("All Mines Revealed.");
	for (size_t y = 0; y < game->board->height; y++) {
		for (size_t x = 0; x < game->board->width; x++) {
			Cell* c = getCell(game->board, x, y);
			if (c->value == 'B') {
				c->status = OPENED;
			}
		}
	}
}

static void autoReveal(MinesweeperGame* game, uint32_t x, uint32_t y) {
	Cell* cell = getCell(game->board, x, y);
	if (!cell) {
		return;
	}
	if (cell->value != '0') {
		return;
	}
	const size_t max_x = minI32(x+1, game->board->width-1);
	const size_t max_y = minI32(y+1, game->board->height-1);

	for (size_t ry = maxI32(y-1, 0); ry <= max_y; ry++) {
		for (size_t rx = maxI32(x-1, 0); rx <= max_x; rx++) {
			if (rx >= game->board->width ||
				ry >= game->board->height
			) {
				continue;
			}
			reveal(game, rx, ry);
		}
	}
}

void smartReveal(MinesweeperGame* game, uint32_t x, uint32_t y) {
	Cell* centerCell = getCell(game->board, x, y);
	if (!centerCell || centerCell->status != OPENED) {
		return;
	}
	uint32_t flaggedCount = 0;
	const uint32_t max_x = minI32(x+1, game->board->width-1);
	const uint32_t max_y = minI32(y+1, game->board->height-1);

	for (uint32_t ry = maxI32(y-1, 0); ry <= max_y; ry++) {
		for (uint32_t rx = maxI32(x-1, 0); rx <= max_x; rx++) {
			Cell* c = getCell(game->board, rx, ry);
			if (c->status == FLAGGED)
				flaggedCount++;
		}
	}
	if ((char)('0' + flaggedCount) != centerCell->value) {
		log_debug("There are not enough flaged cell for smart reveal.");
		return;
	}

	for (uint32_t ry = maxI32(y-1, 0); ry <= max_y; ry++) {
		for (uint32_t rx = maxI32(x-1, 0); rx <= max_x; rx++) {
			Cell* c = getCell(game->board, rx, ry);
			if (c->status == CLOSE){
				reveal(game, rx, ry);
			}
		}
	}
}

void reveal(MinesweeperGame* game, uint32_t x, uint32_t y) {
	Cell* cell = getCell(game->board, x, y);
	if (!cell) {
		return;
	}
	if (game->status.state == WAITING_FIRST_MOVE) {
		generateMines(game, x, y);
		game->status.state = PLAYING;
	}
	switch (cell->status) {
		case FLAGGED: {
			return;
		} break;
		case OPENED: {
			if (game->settings.smartReval) {
				smartReveal(game, x, y);
			}
			return;
		} break;
		case CLOSE: {
			log_debug("Reveal called. %d, %d", x, y);
			cell->status = OPENED;
			if(cell->value == 'B') {
				log_debug("Game Over");
				game->status.state = YOU_GAMEOVER;
				revealAllMines(game);
				return;
			}
			else if (cell->value == '0') {
				log_debug("AutoReveal Started.");
				autoReveal(game, x, y);
				log_debug("AutoReveal Ended.");
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

void placeFlag(MinesweeperGame* game, uint32_t x, uint32_t y) {
	Cell* cell = getCell(game->board, x, y);
	if(!cell){
		return;
	}
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
void unplaceFlag(MinesweeperGame* game, uint32_t x, uint32_t y) {
	Cell* cell = getCell(game->board, x, y);
	if(!cell){
		return;
	}
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

void toggleFlag(MinesweeperGame* game, uint32_t x, uint32_t y) {
	Cell* cell = getCell(game->board, x, y);
	if(!cell){
		return;
	}
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


#include "../include/libmine.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
	setupSettings(&settings);
	MinesweeperGame* game = malloc(sizeof(MinesweeperGame));
	game->settings = settings;
	game->status.startTime = time(NULL);
	game->status.state = WAITING_FIRST_MOVE;
	game->status.flagedCellCount = 0;
	game->status.revealedCellCount = 0;
	// The board will generated when the first move made.
	game->board = malloc(sizeof(Board) + settings.width * settings.height * sizeof(Cell));
	game->board->height = settings.height;
	game->board->width = settings.width;

	return game;
}

void freeGame(MinesweeperGame* game) {
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
	for (int8_t ry = -1; ry <= 1; ry++) {
		for (int8_t rx = -1; rx <= 1; rx++) {
			Cell* c = &GET_CELL(game->board, x+rx, y+ry);
			if (c->value == 'B')
				mineCount++;
		}
	}
	return mineCount;
}

static void generateMines(MinesweeperGame* game, int x, int y) {
	uint32_t avaibleCells = game->board->height * game->board->width - 1;
	uint32_t forbidenCell = y*game->board->width + game->board->height;

	// place Bombs
	for(uint32_t remainingZeros = game->settings.mines; remainingZeros > 0 ; --remainingZeros) {
		uint32_t rcell = rand()%avaibleCells;
		printf("LOG: rand: %d", rcell);
		// find rcell th cell among non mined cells.
		size_t pos = 0;
		while(rcell > 0) {
			if (game->board->grid[pos].value != 'B' && pos != forbidenCell) {
				rcell--;
			}
			pos++;
		}
		game->board->grid[pos].value = 'B';
		avaibleCells--;
	}
	// place number
	for (size_t y = 0; y < game->board->height; y++) {
		for (size_t x = 0; x < game->board->width; x++) {
			Cell* c = &GET_CELL(game->board, x, y);
			if(c->value == 'B')
				continue;
			c->value = (uint8_t)'0' + countAdjacentMines(game, x, y);
		}
	}
}

void reveal(MinesweeperGame* game, int x, int y) {
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
				game->status.state = YOU_GAMEOVER;
				return;
			}
			else if (cell->value == '0') {
				for (int32_t ry = -1; ry <= 1; ry++) {
					for (int32_t rx = -1; rx <= 1; rx++) {
						if (x+rx < 0 || x+rx >= game->board->width ||
							y+ry < 0 || y+ry >= game->board->height
						) {
							continue;
						}
						reveal(game, x+rx, y+ry);
					}
				}
			}
			game->status.revealedCellCount++;
			uint32_t remeningCell = game->board->width * game->board->height - game->settings.mines;
			if (remeningCell == 0) {
				game->status.state = YOU_WIN;
			}
		} break;
	}
}

void revealAllMines(MinesweeperGame* game) {
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


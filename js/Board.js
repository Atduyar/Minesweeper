import blessed from 'blessed';
import { Cell } from './Cell.js';

export class Board {
	constructor(settings) {
		this.cursor = [0, 0];
		this.settings = settings;
		this.gameStatus = 'playing';
		this.openedCellsCount = 0;
		this.isFirstMove = true;
		this.cells = Array.from({ length: settings.height }, () =>
			Array.from({ length: settings.width }, () => new Cell(false, 0))
		);

		// Place mines
		for (let i = 0; i < settings.mine; i++) {
			this.findRandomNonMineCell().setMine();
		}

		// Calculate numbers
		this.calculateNumbers();
	}

	findRandomNonMineCell() {
		while (true) {
			const randX = Math.floor(Math.random() * this.settings.width);
			const randY = Math.floor(Math.random() * this.settings.height);
			if (!this.cells[randY][randX].isMine) {
				return this.cells[randY][randX];
			}
		}
	}

	calculateNumbers() {
		for (let y = 0; y < this.settings.height; y++) {
			for (let x = 0; x < this.settings.width; x++) {
				if (this.cells[y][x].isMine) continue;
				let mineCount = 0;
				for (let ry = Math.max(0, y - 1); ry <= Math.min(y + 1, this.settings.height - 1); ry++) {
					for (let rx = Math.max(0, x - 1); rx <= Math.min(x + 1, this.settings.width - 1); rx++) {
						if (this.cells[ry][rx].isMine) mineCount++;
					}
				}
				this.cells[y][x].setNumber(mineCount);
			}
		}
	}

	render(boardBox) {
		boardBox.children.forEach(child => boardBox.remove(child)); // Clear existing elements
		for (let y = 0; y < this.settings.height; y++) {
			let row = '';
			for (let x = 0; x < this.settings.width; x++) {
				if (x === this.cursor[0] && y === this.cursor[1]) {
					row += `{inverse}${this.cells[y][x]}{/inverse}`;
				} else {
					row += this.cells[y][x];
				}
			}
			boardBox.append(blessed.text({
				top: y, // Position each row at a unique vertical position
				left: 0,
				content: row,
				tags: true,
			}));
		}
		// boardBox.render(); // Refresh the screen
	}

	toggleFlag() {
		const cell = this.cells[this.cursor[1]][this.cursor[0]];
		if (cell.isOpened) return;
		cell.isFlagged = !cell.isFlagged;
	}

	openCell(x, y) {
		if (x === undefined || y === undefined) {
			x = this.cursor[0];
			y = this.cursor[1];
		}
		if (this.gameStatus !== 'playing') return;
		const cell = this.cells[y][x];
		if (cell.isFlagged) {
			return;
		}
		if (cell.isOpened) {
			if (cell.number === 0) {
				return;
			}
			let flaggedCount = 0;
			for (let ry = Math.max(0, y - 1); ry <= Math.min(y + 1, this.settings.height - 1); ry++) {
				for (let rx = Math.max(0, x - 1); rx <= Math.min(x + 1, this.settings.width - 1); rx++) {
					if (this.cells[ry][rx].isFlagged) {
						flaggedCount++;
					}
				}
			}
			if (flaggedCount !== cell.number) {
				return;
			}
			for (let ry = Math.max(0, y - 1); ry <= Math.min(y + 1, this.settings.height - 1); ry++) {
				for (let rx = Math.max(0, x - 1); rx <= Math.min(x + 1, this.settings.width - 1); rx++) {
					if (!this.cells[ry][rx].isFlagged && !this.cells[ry][rx].isOpened) {
						this.openCell(rx, ry);
					}
				}
			}
			return;
		}
		if (this.isFirstMove) {
			this.isFirstMove = false;
			if (cell.isMine) {
				cell.isMine = false;
				this.findRandomNonMineCell().setMine();
				this.calculateNumbers();
			}
		}
		cell.isOpened = true;
		this.openedCellsCount++;
		if (cell.isMine) {
			this.gameStatus = 'lose';
			for (let y = 0; y < this.settings.height; y++) {
				for (let x = 0; x < this.settings.width; x++) {
					if (this.cells[y][x].isMine) {
						this.cells[y][x].isOpened = true;
					}
				}
			}
			return;
		} else if (cell.number === 0) {
			for (let ry = Math.max(0, y - 1); ry <= Math.min(y + 1, this.settings.height - 1); ry++) {
				for (let rx = Math.max(0, x - 1); rx <= Math.min(x + 1, this.settings.width - 1); rx++) {
					if (this.cells[ry][rx].isOpened) {
						continue;
					}
					this.openCell(rx, ry);
				}
			}
		}
		if (this.openedCellsCount === this.settings.width * this.settings.height - this.settings.mine) {
			this.gameStatus = 'win';
		}
	}

	moveCursorUp() {
		this.cursor[1] = Math.max(0, this.cursor[1] - 1);
	}

	moveCursorDown() {
		this.cursor[1] = Math.min(this.settings.height - 1, this.cursor[1] + 1);
	}

	moveCursorLeft() {
		this.cursor[0] = Math.max(0, this.cursor[0] - 1);
	}

	moveCursorRight() {
		this.cursor[0] = Math.min(this.settings.width - 1, this.cursor[0] + 1);
	}
}

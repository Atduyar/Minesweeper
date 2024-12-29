import blessed from 'blessed';
import { GameSettings } from './GameSettings.js';
import { Board } from './Board.js';

export class GameScreen {
	constructor(mainScreen) {
		this.mainScreen = mainScreen;
		this.screen = blessed.box({
			top: 'center',
			left: 'center',
			width: '100%',
			height: '100%',
			tags: true,
			border: {
				type: 'line',
			},
			style: {
				fg: 'white',
				border: {
					fg: '#f0f0f0',
				},
			},
		});
		this.screenBoard = this.newGameBoard();

		this.screenStatus = blessed.box({
			parent: this.screen,
			left: 0,
			bottom: 0,
			width: '100%-2',
			shrink: true,
			content: 'Status: Playing',
			height: 1,
			tags: true,
			style: {
				fg: 'black',
				bold: true,
				bg: 'white',
			},
		});
		this.board = null;

		this.screen.key(['escape', 'q'], () => {
			this.screen.detach();
			this.mainScreen.menuScreen.attach();
		});

		this.screen.key(['r'], () => {
			this.newGame(this.board.settings);
		});

		this.screen.key(['up', 'k'], () => {
			this.board.moveCursorUp();
			this.render()
		});

		this.screen.key(['down', 'j'], () => {
			this.board.moveCursorDown();
			this.render()
		});

		this.screen.key(['left', 'h'], () => {
			this.board.moveCursorLeft();
			this.render()
		});

		this.screen.key(['right', 'l'], () => {
			this.board.moveCursorRight();
			this.render()
		});

		this.screen.key(['space', 'enter'], () => {
			this.board.openCell();
			this.render()
		});

		this.screen.key(['f'], () => {
			this.board.toggleFlag();
			this.render()
		});
	}

	render() {
		// this.board.render(this.screen);
		this.board.render(this.screenBoard);
		this.updateStatus();
		this.mainScreen.render()
	}

	updateStatus() {
		let status = 'Status: ';
		let openCellStat = `(${this.board.openedCellsCount}/${this.board.settings.width * this.board.settings.height - this.board.settings.mine})`;
		switch (this.board.gameStatus) {
			case 'playing':
				status += 'Playing ' + openCellStat;
				this.screen.style.border.fg = 'white';
				this.screenStatus.style.bg = 'white';
				break;
			case 'win':
				status += 'You Win';
				this.screen.style.border.fg = 'green';
				this.screenStatus.style.bg = 'green';
				break;
			case 'lose':
				status += 'You Lose';
				this.screen.style.border.fg = 'red';
				this.screenStatus.style.bg = 'red';
				break;
		}
		let tip = "";
		if (this.board.gameStatus === 'playing') {
			tip = ' [Arrow keys] Move  [Space] Open  [F] Flag  [R] Restart {red-bg} [Q] Quit {/red-bg}';
		}
		else if (this.board.gameStatus === 'win') {
			tip = ' [R] Restart  {red-bg} [Q] Quit {/red-bg}';
		}
		else {
			tip = '{white-bg} [R] Restart  [Q] Quit {/white-bg}';
		}
		this.screenStatus.setContent(status + '{|}' + tip);
	}

	newGameBoard() {
		return blessed.box({
			parent: this.screen,
			top: 0,
			left: 0,
			width: '100%-2',
			// left: 'center',
			// top: 'center',
			width: 'shrink',
			height: 'shrink',
			tags: true,
			border: {
				type: 'line',
			},
			style: {
				fg: 'white',
				border: {
					fg: '#f0f0f0',
				},
			},
		});

	}

	newGame(settings) {
		this.board = new Board(settings);
		// this.screenBoard.set('width', this.board.settings.width * 4);
		// this.screenBoard.set('height', this.board.settings.height * 2);
		this.screen.remove(this.screenBoard);
		this.screenBoard = this.newGameBoard();
		this.render();
	}

	attach() {
		this.mainScreen.append(this.screen);
		this.screen.focus();
		this.render();
	}
}

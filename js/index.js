import blessed from 'blessed';
import { GameSettings } from './GameSettings.js';
import { Board } from './Board.js';

const screen = blessed.screen({
	smartCSR: true,
	title: 'Minesweeper',
});

const settings = new GameSettings('easy');
const board = new Board(settings);
board.render(screen);

screen.key(['escape', 'q'], () => {
	screen.destroy();
	process.exit(0);
});

screen.key(['up', 'k'], () => {
	board.moveCursorUp();
	board.render(screen);
});

screen.key(['down', 'j'], () => {
	board.moveCursorDown();
	board.render(screen);
});

screen.key(['left', 'h'], () => {
	board.moveCursorLeft();
	board.render(screen);
});

screen.key(['right', 'l'], () => {
	board.moveCursorRight();
	board.render(screen);
});

screen.key(['space', 'enter'], () => {
	board.openCell();
	board.render(screen);
});

screen.key(['f'], () => {
	board.toggleFlag();
	board.render(screen);
});

import blessed from 'blessed';
import { MenuScreen } from './MenuScreen.js';
import { GameScreen } from './GameScreen.js';

const screen = blessed.screen({
	smartCSR: true,
	title: 'Minesweeper',
});

screen.menuScreen = new MenuScreen(screen);
screen.gameScreen = new GameScreen(screen);
screen.menuScreen.attach();
screen.render();

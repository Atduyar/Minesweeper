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

// let pro = blessed.prompt({
// 	parent: screen,
// 	border: 'line',
// 	width: '50%',
// 	height: '30%',
// 	top: 'center',
// 	left: 'center',
// 	label: ' Enter a Number ',
// 	tags: true,
// 	keys: true,
// 	vi: true,
// });
// pro.input('Enter your name:', '', (err, value) => {
// 	if (!err) {
// 		console.log(value);
// 	}
// });

// screen.append(pro);
// screen.append(box);

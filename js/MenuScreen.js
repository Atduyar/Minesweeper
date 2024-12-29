import blessed from 'blessed';
import { GameSettings } from './GameSettings.js';

export class MenuScreen {
	static bigTitle = `  __  __ _             _____                                  
 |  \\/  (_)           / ____|                                 
 | \\  / |_ _ __   ___| (_____      _____  ___ _ __   ___ _ __ 
 | |\\/| | | '_ \\ / _ \\\\___ \\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|
 | |  | | | | | |  __/____) \\ V  V /  __/  __/ |_) |  __/ |   
 |_|  |_|_|_| |_|\\___|_____/ \\_/\\_/ \\___|\\___| .__/ \\___|_|   
                                             | |              
                                             |_|              `

	constructor(mainScreen) {
		this.mainScreen = mainScreen;
		this.screen = blessed.box({
			top: 'center',
			left: 'center',
			width: '100%',
			height: '100%',
			content: 'Press any key to start',
			tags: true,
			style: {
				fg: 'white',
				border: {
					fg: '#f0f0f0',
				},
			},
		})

		this.screen.append(blessed.text({
			content: MenuScreen.bigTitle,
		}));

		this.screenForm = blessed.form({
			parent: this.screen,
			keys: true,
			width: 30,
			height: 4,
			top: 9,
			left: 2,
			content: 'Choose Difficulty: ',
		});

		this.screen.on('focus', () => {
			this.screenForm.focus();
		});

		this.btns = {};
		this.btns["easy"] = this.createButton('easy', 2);
		this.btns["medium"] = this.createButton('medium', 4);
		this.btns["hard"] = this.createButton('hard', 6);
		this.btns["custom"] = this.createButton('custom', 8);
		this.btns["exit"] = this.createButton('exit', 11, true);

		this.screenForm.key(['q'], this.createBtnEventListner('exit'));
		this.screenForm.key(['1'], this.createBtnEventListner('easy'));
		this.screenForm.key(['2'], this.createBtnEventListner('medium'));
		this.screenForm.key(['3'], this.createBtnEventListner('hard'));
		this.screenForm.key(['4'], this.createBtnEventListner('custom'));

		Object.values(this.btns).forEach(btn => {
			btn.key(['q'], this.createBtnEventListner('exit'));
			btn.key(['1'], this.createBtnEventListner('easy'));
			btn.key(['2'], this.createBtnEventListner('medium'));
			btn.key(['3'], this.createBtnEventListner('hard'));
			btn.key(['4'], this.createBtnEventListner('custom'));

			btn.on('press', (data) => {
				this.screenForm.submit();
			});
		});

		this.screenForm.on('submit', (data) => {
			this.createBtnEventListner(Object.keys(data)[0])();
		});

		this.customForm = blessed.prompt({
			parent: this.screen,
			focused: true,
			border: 'line',
			width: '50%',
			height: '30%',
			top: 'center',
			left: 'center',
			label: ' Custom Game Settings ',
			tags: true,
			keys: true,
		});
	}

	createButton(name, top, exit = false) {
		return blessed.button({
			parent: this.screenForm,
			mouse: true,
			keys: true,
			shrink: true,
			padding: {
				left: 1,
				right: 1
			},
			left: 4,
			top: top,
			shrink: true,
			name: name,
			content: '* ' + name.charAt(0).toUpperCase() + name.slice(1),
			style: exit ?
				{
					bg: 'red',
					fg: 'black',
					focus: {
						bg: 'magenta'
					},
					hover: {
						bg: 'magenta'
					}
				}
				: {
					bg: 'white',
					fg: 'black',
					focus: {
						bg: 'cyan'
					},
					hover: {
						bg: 'cyan'
					}
				}
		});
	}

	createBtnEventListner(name) {
		if (name === 'exit') {
			return () => {
				this.mainScreen.destroy();
				process.exit(0);
			}
		}
		if (name === 'custom') {
			return () => {
				this.customForm.readInput('Enter width:', '', (err, value) => {
					if (err || !value) {
						this.mainScreen.render();
						return;
					}
					let width = parseInt(value);
					if (width < 3 || isNaN(width)) {
						this.mainScreen.render();
						return;
					}

					this.customForm.readInput('Enter height:', '', (err, value) => {
						if (err) {
							throw new Error('Invalid height' + err);
							return;
						}
						let height = parseInt(value);
						if (height < 3 || isNaN(height)) {
							this.mainScreen.render();
							return;
						}

						this.customForm.readInput('Enter mines:', '', (err, value) => {
							if (err) {
								return;
							}
							let mines = parseInt(value);
							if (mines < 1 || mines >= width * height || isNaN(mines)) {
								this.mainScreen.render();
								return;
							}

							try {
								let gs = new GameSettings('custom', width, height, mines);
								this.screen.detach();
								this.mainScreen.gameScreen.newGame(gs);
								this.mainScreen.gameScreen.attach();
							}
							catch (e) {
								throw new Error('Invalid custom game settings: ' + e);
								return;
							}
						});

					});
				});
			}
		}
		return () => {
			// throw new Error('Invalid button name: ' + JSON.stringify(data));
			this.screen.detach();
			this.mainScreen.gameScreen.newGame(new GameSettings(name));
			this.mainScreen.gameScreen.attach();
		}
	}

	attach() {
		this.mainScreen.append(this.screen);
		this.screenForm.focus();
		this.mainScreen.render();
	}
}

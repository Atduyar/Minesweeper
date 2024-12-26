export class GameSettings {
	constructor(difficulty, width = null, height = null, mine = null) {
		switch (difficulty) {
			case 'easy':
				this.width = 9;
				this.height = 9;
				this.mine = 10;
				break;
			case 'medium':
				this.width = 16;
				this.height = 16;
				this.mine = 40;
				break;
			case 'hard':
				this.width = 30;
				this.height = 16;
				this.mine = 99;
				break;
			case 'custom':
				this.width = width;
				this.height = height;
				this.mine = mine;
				break;
			default:
				throw new Error('Invalid difficulty setting');
		}
	}
}

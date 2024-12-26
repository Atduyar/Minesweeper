export class Cell {
	constructor(isMine = false, number = 0) {
		this.isMine = isMine;
		this.number = number;
		this.isFlagged = false;
		this.isOpened = false;
	}

	toString() {
		if (this.isFlagged) {
			return `{green-fg} F {/green-fg}`;
		} else if (!this.isOpened) {
			return `{white-fg} # {/white-fg}`;
		} else if (this.isMine) {
			return `{red-fg} B {/red-fg}`;
		} else {
			switch (this.number) {
				case 0:
					return '   ';
				case 1:
					return `{black-fg} ${this.number} {/black-fg}`;
				case 2:
					return `{blue-fg} ${this.number} {/blue-fg}`;
				case 3:
					return `{magenta-fg} ${this.number} {/magenta-fg}`;
				default:
					return `{bright-red-fg} ${this.number} {/bright-red-fg}`;
			}
		}
	}

	setMine() {
		this.isMine = true;
		this.number = null;
	}

	setNumber(num) {
		this.isMine = false;
		this.number = num;
	}
}

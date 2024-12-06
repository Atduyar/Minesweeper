from blessed import Terminal
import random

class GameSettings:
    def __init__(self,difficulty,width=None,height=None,mine=None):
        match difficulty:
            case "easy":
                self.width=9
                self.height=9
                self.mine=10
            case "medium":
                self.width=16
                self.height=16
                self.mine=40
            case "hard":
                self.width=30
                self.height=16
                self.mine=99
            case "custom":
                self.height=height
                self.width=width
                self.mine=mine

class Cell:
    def __init__(self,isMine,number=None):
        self.isMine=isMine
        self.number=number
        self.isFlagged=False
        self.isOpened=False
    def __str__(self):
        if self.isFlagged:
            return term.bright_green+term.bold+ ' F '+term.normal
        elif self.isOpened==False:
            return ' # '
        else:
            if self.isMine:
                return term.red+term.bold+" B " + term.normal
            else:
                match self.number:
                    case 0:
                        return "   "
                        #return term.black+" "+ str(self.number) + " "+term.normal
                    case 1:
                        return term.black+" "+ str(self.number) + " "+term.normal
                    case 2:
                        return term.royalblue+" "+ str(self.number) + " "+term.normal
                    case 3:
                        return term.maroon+" "+ str(self.number) + " "+term.normal
                    case _:
                        return term.red+" "+ str(self.number) + " "+term.normal
    
    def setMine(self):
        self.isMine=True
        self.number=None

    def setNumber(self,num):
        self.isMine=False
        self.number=num

class Board:
    def __init__(self,settings):
        self.cursor = [0, 0]
        self.settings=settings
        self.gameStatus="playing"
        self.openedCellsCount=0
        self.isFirstMove=True
        self.cells = []
        # Creating cells
        for y in range(0,self.settings.height):
            self.cells.append([])
            for x in range(0,self.settings.width):
                self.cells[y].append(Cell(False, 0))
                #self.cells[y][x].isOpened=True
        #Creating mines
        for _ in range (0,self.settings.mine):
            self.FindRandomNonMineCell().setMine()

        #Creating numbers
        self.CalcNumbers()
    
    def FindRandomNonMineCell(self):
        while True:
                randX = random.randint(0,self.settings.width-1)
                randY = random.randint(0,self.settings.height-1)
                if self.cells[randY][randX].isMine:
                    continue
                else:
                    return self.cells[randY][randX]

    def CalcNumbers(self):
        for y in range(0,self.settings.height):
            for x in range(0,self.settings.width):
                if self.cells[y][x].isMine:
                    continue
                mineCount=0
                for ry in range(max(0,y-1),min(y+2,self.settings.height)):
                    for rx in range(max(0,x-1),min(x+2,self.settings.width)):
                        if self.cells[ry][rx].isMine:
                            mineCount+=1
                self.cells[y][x].setNumber(mineCount)

    def render(self):
        for y in range (0,self.settings.height):
            for x in range(0,self.settings.width):
                if x == self.cursor[0] and y == self.cursor[1]:
                    print(term.on_snow(str(self.cells[y][x])), end="")
                else:
                    print(self.cells[y][x], end="")
            print()
    
    def moveCursorUp(self):
        if self.cursor[1] > 0:
            self.cursor[1] -= 1
    def moveCursorDown(self):
        if self.cursor[1] < self.settings.height-1:
            self.cursor[1] += 1
    def moveCursorLeft(self):
        if self.cursor[0] > 0:
            self.cursor[0] -= 1
    def moveCursorRight(self):
        if self.cursor[0] < self.settings.width-1:
            self.cursor[0] += 1
    
    def toggleFlag(self):
        if self.gameStatus != "playing":
            return
        
        cell = self.cells[self.cursor[1]][self.cursor[0]]
        if not cell.isOpened:
            cell.isFlagged= not cell.isFlagged
    def reveal(self, x=None,y=None):
        if self.gameStatus != "playing":
            return
        
        if x == None or y == None:
            x = self.cursor[0]
            y = self.cursor[1]
        
        cell = self.cells[y][x]
            
        if cell.isFlagged:
            return
        if cell.isOpened:
            if cell.number != 0:
                flaggedCount = 0
                for ry in range(max(0,y-1),min(y+2,self.settings.height)):
                    for rx in range(max(0,x-1),min(x+2,self.settings.width)):
                        rCell = self.cells[ry][rx]
                        if rCell.isFlagged:
                            flaggedCount += 1
                if flaggedCount == cell.number:
                    for ry in range(max(0,y-1),min(y+2,self.settings.height)):
                        for rx in range(max(0,x-1),min(x+2,self.settings.width)):
                            rCell = self.cells[ry][rx]
                            if not rCell.isFlagged and not rCell.isOpened:
                                self.reveal(rx, ry)
            return
        cell.isOpened = True
        
        if cell.isMine:
            if self.isFirstMove:
                randCell = self.FindRandomNonMineCell()
                randCell.setMine()
                cell.isMine = False
                self.CalcNumbers()
            else:
                self.gameStatus="gameOver"
                for y in range(0,self.settings.height):
                    for x in range(0,self.settings.width):
                        cell = self.cells[y][x]
                        if cell.isMine:
                            cell.isOpened = True
                            cell.isFlagged = False
                return
        if cell.number == 0:
            for ry in range(max(0,y-1),min(y+2,self.settings.height)):
                for rx in range(max(0,x-1),min(x+2,self.settings.width)):
                    cell = self.cells[ry][rx]
                    self.reveal(rx, ry)
        
        self.isFirstMove = False
        self.openedCellsCount += 1
        if board.settings.height*board.settings.width - board.settings.mine == self.openedCellsCount:
            self.gameStatus = "gameWin"
        

term = Terminal()
isMenu = True
board=Board(GameSettings("hard"))
        
while True:
    # render
    if isMenu:
        print(term.home + term.clear)
        print(term.black_on_darkkhaki(term.center('Minesweeper')))
        print("""  __  __ _             _____                                  
 |  \/  (_)           / ____|                                 
 | \  / |_ _ __   ___| (_____      _____  ___ _ __   ___ _ __ 
 | |\/| | | '_ \ / _ \\___ \ \ /\ / / _ \/ _ \ '_ \ / _ \ '__|
 | |  | | | | | |  __/____) \ V  V /  __/  __/ |_) |  __/ |   
 |_|  |_|_|_| |_|\___|_____/ \_/\_/ \___|\___| .__/ \___|_|   
                                             | |              
                                             |_|              """)
        
        print()
        print(term.black_on_darkkhaki(term.center("Choose difficulty:")))
        print("\t1: easy")
        print("\t2: medium")
        print("\t3: hard")
        print()
        print("\tq: quit")
        with term.cbreak(), term.hidden_cursor():
            inp = term.inkey()
        match inp:
            case "1":
                board=Board(GameSettings("easy"))
                isMenu = False
            case "2":
                board=Board(GameSettings("medium"))
                isMenu = False
            case "3":
                board=Board(GameSettings("hard"))
                isMenu = False
            case "q":
                exit(0)
        continue
    
    print(term.home + term.clear)
    print(term.black_on_darkkhaki(term.center('Minesweeper')))

    match board.gameStatus:
        case "playing":
            print(term.black_on_yellow(term.center(f"{board.openedCellsCount}/{board.settings.height*board.settings.width-board.settings.mine}")))
        case "gameWin":
            print(term.black_on_green(term.center('You Win! Press "r" to replay')))
        case "gameOver":
            print(term.black_on_red(term.center('Game Over Press "r" to replay')))
    board.render()
        
    print(term.black_on_darkkhaki(term.center('Status')))
    print(end="", flush=True)
    # update
    with term.cbreak(), term.hidden_cursor():
        inp = term.inkey()
    if inp.is_sequence:
        match inp.name:
            case "KEY_UP":
                board.moveCursorUp()
            case "KEY_DOWN":
                board.moveCursorDown()
            case "KEY_LEFT":
                board.moveCursorLeft()
            case "KEY_RIGHT":
                board.moveCursorRight()
            case "KEY_ENTER":
                board.reveal()
    else:
        match inp:
            case "F" | "f":
                board.toggleFlag()
            case "r":
                board=Board(board.settings)
            case " ":
                board.reveal()
            case "q":
                isMenu = True

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
            return term.green+ ' F '+term.normal
        elif self.isOpened==False:
            return ' # '
        else:
            if self.isMine:
                return term.red+" B " + term.normal
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
    
    def setMine (self):
        self.isMine=True
        self.number=None

    def setNumber (self,num):
        self.isMine=False
        self.number=num

class Board:
    def __init__(self,settings):
        self.settings=settings
        self.cells=[]
        # Creating cells
        for y in range(0,self.settings.height):
            self.cells.append([])
            for x in range(0,self.settings.width):
                self.cells[y].append(Cell(False, 0))
                self.cells[y][x].isOpened=True
        #Creating mines
        for _ in range (0,self.settings.mine):
            while True:
                randX = random.randint(0,self.settings.width-1)
                randY = random.randint(0,self.settings.height-1)
                if self.cells[randY][randX].isMine:
                    continue
                else:
                    self.cells[randY][randX].setMine()
                    break
        #Creating numbers
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
                print(self.cells[y][x], end="")
            print()
        

term = Terminal()

print(term.home + term.clear )
print(term.black_on_darkkhaki(term.center('Minesweeper')))

board=Board(GameSettings("easy"))
board.render()

print(term.black_on_darkkhaki(term.center('Status')))

with term.cbreak(), term.hidden_cursor():
    inp = term.inkey()
print(term.move_down(2) + 'You pressed ' + term.bold(repr(inp)))

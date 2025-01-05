import pygame
from Cell import Cell
from FigureFabric import FigureFabric

class Application:
    def __init__(self, width=10, height=20, squareSide=30, shift=7, xLeft=50, yTop=50, screenWidth=1000, screenHeight=800, cellColor=(255, 255, 255), figureCellColor=(246, 33, 33)):
        self.width = width
        self.height = height
        self.squareSide = squareSide
        self.shift = shift
        self.xLeft = xLeft
        self.yTop = yTop
        self.screenWidth = screenWidth
        self.screenHeight = screenHeight  
        self.cellColor = cellColor
        self.FF = FigureFabric(0, 4)
        self.figureCellColor = figureCellColor
        
        self.screen = pygame.display.set_mode((self.screenWidth, self.screenHeight))
        self.screen.convert_alpha()
        self.fixedCells = []
        self.fallingCells = []
        self.cells = [[] for j in range(self.height)]
        self.surfaces = [[] for j in range(self.height)]

        x = self.xLeft
        for i in range(self.width):
            y = self.yTop
            for j in range(self.height):
                surface = pygame.Surface((self.squareSide-self.shift*2, self.squareSide-self.shift*2))
                surface.fill((255, 255, 255))
                surface.set_alpha(70)
                self.screen.blit(surface, (x+self.shift, y+self.shift))
                self.surfaces[j].append(surface)
                self.cells[j].append(Cell((x, y), self.squareSide, self.squareSide))
                y += self.squareSide
            x += self.squareSide

    def __drawGrid(self):
        for line in self.cells:
            for cell in line:
                cell.draw(self.screen)

    def __drawScorePanel(self):
        rect = pygame.Rect(self.squareSide*self.width+self.xLeft, self.yTop, self.squareSide*self.width, self.squareSide*self.height/2)
        pygame.draw.rect(self.screen, (255, 255, 255), rect, 2)

    def drawScreen(self):
        self.__drawGrid()
        self.__drawScorePanel()

    def createNewFigure(self):
        self.fallingCells = self.FF.next()
        
        for x, y, color in self.fallingCells:
            self.cells[y][x].on(self.screen, color)

    def __isEmptyBellow(self, x, y):
        y = y+1
        for cell in self.fixedCells:
            if cell[1] == y and cell[0] == x:
                return False
        return True
    
    def __offCells(self, cells):
        for cell in cells:
            self.cells[cell[1]][cell[0]].off(self.screen)
            x = self.cells[cell[1]][cell[0]].x
            y = self.cells[cell[1]][cell[0]].y
            s = self.cells[cell[1]][cell[0]].shift
            self.screen.blit(self.surfaces[cell[1]][cell[0]], (x+s, y+s))

    def __fall(self, cells):
        self.__offCells(cells)

        for cell in cells:
            cell[1] += 1
            self.cells[cell[1]][cell[0]].on(self.screen, self.figureCellColor)

    def __checkLineFill(self):
        for j, line in enumerate(self.cells):
            isLineFilled = True
            for cell in line:
                if not cell.glowing:
                    isLineFilled = False
                    break
            if isLineFilled:
                return (True, j)
        
        return (False, )
    
    def __isLineFalling(self, j, filledLines):
        if filledLines < j or j >= self.height:
            return False
        return True

    def fallUpdate(self):
        falling = True
        for (x, y, _) in self.fallingCells:
            if self.__isEmptyBellow(x, y):
                if y < self.height - 1:
                    continue
            falling = False
            break

        if not falling: 
            self.fixedCells += self.fallingCells
            self.fallingCells= []

            filledLines = self.__checkLineFill()
            print(filledLines)
            # if len(filledLines) != 0:
            #     for j in filledLines:
            #         for i, cell in enumerate(self.cells[j]):
            #             # print(i, j)
            #             # print(self.fixedCells)
            #             self.fixedCells.remove([i, j, self.figureCellColor])
            #             # cell.filled = False
            #             # cell.color = self.CELL_COLOR

            #             # rect = pygame.Rect(cell.x+cell.shift, cell.y+cell.shift, cell.width-2*cell.shift, cell.height-2*cell.shift)
            #             # pygame.draw.rect(self.screen, (0, 0, 0), rect)
            #             cell.off(self.screen)
            #             # surface = pygame.Surface((cell.width-cell.shift*2, cell.height-cell.shift*2))
            #             # surface.fill((255, 255, 255))
            #             # surface.set_alpha(70)
            #             self.screen.blit(self.surfaces[j][i], (cell.x+cell.shift, cell.y+cell.shift))

            #         # self.__fall([cell for cell in self.fixedCells if self.__isLineFalling(cell[1], filledLines) ])
            
            print(filledLines)
            if filledLines[0]:
                while filledLines[0]:
                    j = filledLines[1]
                    for i, cell in enumerate(self.cells[j]):
                        self.fixedCells.remove([i, j, self.figureCellColor])
                        cell.off(self.screen)
                        self.screen.blit(self.surfaces[j][i], (cell.x+cell.shift, cell.y+cell.shift))
                    # print([cell for cell in self.fixedCells if self.__isLineFalling(cell[1], filledLines[1]) ])
                    self.__fall([cell for cell in self.fixedCells if self.__isLineFalling(cell[1], filledLines[1]) ])
                    filledLines = self.__checkLineFill()

            self.createNewFigure()
        else:
            self.__fall(self.fallingCells)

    def __isEmptySide(self, x, y, direction):
        x = x + direction
        for cell in self.fixedCells:
            if cell[1] == y and cell[0] == x:
                return False
        return True
    
    def __shift(self, cells, direction):
        self.__offCells(cells)

        for cell in cells:
            cell[0] += direction
            self.cells[cell[1]][cell[0]].on(self.screen, self.figureCellColor)


    def shiftUpdate(self, direction):
        if direction == 0:
            return
        
        shifting = True
        for (x, y, _) in self.fallingCells:
            if self.__isEmptySide(x, y, direction):
                if direction == 1:
                    if x < self.width - 1:
                        continue
                elif direction == -1:
                    if x > 0:
                        continue
            shifting = False
            break

        
        if shifting:
            self.__shift(self.fallingCells, direction)
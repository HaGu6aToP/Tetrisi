import pygame
from Cell import Cell
from FigureFabric import FigureFabric

class Application:


    def __init__(self, WIDTH = 10, HEIGHT = 20, SQUARE_SIDE = 30, SHIFT = 7, X_LEFT = 50, Y_TOP = 50, SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 800, CELL_COLOR=(255, 255, 255)):
        self.WIDTH = WIDTH
        self.HEIGHT = HEIGHT
        self.SQUARE_SIDE = SQUARE_SIDE
        self.SHIFT = SHIFT
        self.X_LEFT = X_LEFT
        self.Y_TOP = Y_TOP
        self.SCREEN_WIDTH = SCREEN_WIDTH
        self.SCREEN_HEIGHT = SCREEN_HEIGHT  
        self.CELL_COLOR = CELL_COLOR
        self.FF = FigureFabric(0, 4)
        
        self.screen = pygame.display.set_mode((self.SCREEN_WIDTH, self.SCREEN_HEIGHT))
        self.screen.convert_alpha()
        self.fixedCells = []
        self.fallingCells = []
        self.cells = [[] for j in range(self.HEIGHT)]

        x = self.X_LEFT
        for i in range(self.WIDTH):
            y = self.Y_TOP
            for j in range(self.HEIGHT):
                surface = pygame.Surface((self.SQUARE_SIDE-self.SHIFT*2, self.SQUARE_SIDE-self.SHIFT*2))
                surface.fill((255, 255, 255))
                surface.set_alpha(70)
                self.screen.blit(surface, (x+self.SHIFT, y+self.SHIFT))
                self.cells[j].append(Cell(x, y, self.SQUARE_SIDE, self.SQUARE_SIDE, self.CELL_COLOR))
                y += self.SQUARE_SIDE
            x += self.SQUARE_SIDE


    def __drawGrid(self):
        for line in self.cells:
            for cell in line:
                cell.draw(self.screen)

    def __drawScorePanel(self):
        rect = pygame.Rect(self.SQUARE_SIDE*self.WIDTH+self.X_LEFT, self.Y_TOP, self.SQUARE_SIDE*self.WIDTH, self.SQUARE_SIDE*self.HEIGHT/2)
        pygame.draw.rect(self.screen, (255, 255, 255), rect, 1)

    def drawScreen(self):
        self.__drawGrid()
        self.__drawScorePanel()

    def createNewFigure(self):
        self.fallingCells = self.FF.next()
        self.__fill(self.fallingCells)

    def __fill(self, cells):
        for (x, y, color) in cells:
            self.cells[y][x].filled = True
            self.cells[y][x].color = color

    def __clear(self, cells):
        for (x, y, color) in cells:
            self.cells[y][x].filled = False
            self.cells[y][x].color = self.CELL_COLOR

    def __fall(self, cells):
        for cell in cells:
            self.cells[cell[1]][cell[0]].filled = False
            self.cells[cell[1]][cell[0]].color = self.CELL_COLOR

            x = self.cells[cell[1]][cell[0]].x
            y = self.cells[cell[1]][cell[0]].y
            w = self.cells[cell[1]][cell[0]].width
            h = self.cells[cell[1]][cell[0]].height
            s = self.cells[cell[1]][cell[0]].shift
            rect = pygame.Rect(x+s, y+s, w-2*s, h-2*s)
            pygame.draw.rect(self.screen, (0, 0, 0), rect)
            surface = pygame.Surface((self.SQUARE_SIDE-self.SHIFT*2, self.SQUARE_SIDE-self.SHIFT*2))
            surface.fill((255, 255, 255))
            surface.set_alpha(70)
            self.screen.blit(surface, (x+s, y+s))

            cell[1] += 1
            self.cells[cell[1]][cell[0]].filled = True
            self.cells[cell[1]][cell[0]].color = cell[2]

    def __isEmptyBellow(self, x, y):
        y = y+1
        for cell in self.fixedCells:
            if cell[1] == y and cell[0] == x:
                return False
        return True
    
    def fallUpdate(self):
        falling = True
        for (x, y, _) in self.fallingCells:
            if self.__isEmptyBellow(x, y):
                if y < self.HEIGHT - 1:
                    continue
            falling = False
            break

        if not falling:           
            self.fixedCells += self.fallingCells
            self.fallingCells= []
            self.createNewFigure()
        else:
            self.__fall(self.fallingCells)
        

        
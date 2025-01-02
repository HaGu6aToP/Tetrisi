import pygame
from Const import Const
from FigureFabric import FigureFabric

class MainScreen:
    def __init__(self) -> None:
        self.screen = pygame.display.set_mode((Const.SCREEN_WIDTH, Const.SCREEN_HEIGHT))
        self.blocks = []
        self.fallingBlocks = []
 
    def __drawCell(self, x, y):
        cell = pygame.Rect(x, y, Const.SQUARE_SIDE, Const.SQUARE_SIDE)
        pygame.draw.rect(self.screen, (255, 255, 255), cell, 2)

        rect = pygame.Surface((Const.SQUARE_SIDE-Const.SHIFT*2, Const.SQUARE_SIDE-Const.SHIFT*2))
        rect.fill((156, 156, 156))
        rect.set_alpha(3)
        self.screen.blit(rect, (x+Const.SHIFT, y+Const.SHIFT))

    def __drawGrid(self):
        x = Const.X_LEFT
        for i in range(Const.WIDTH):
            y = Const.Y_TOP
            for j in range(Const.HEIGHT):
                self.__drawCell(x, y)
                y += Const.SQUARE_SIDE
            x += Const.SQUARE_SIDE

    def __drawScorePanel(self):
        rect = pygame.Rect(Const.SQUARE_SIDE*Const.WIDTH+Const.X_LEFT, Const.Y_TOP, Const.SQUARE_SIDE*Const.WIDTH, Const.SQUARE_SIDE*Const.HEIGHT/2)
        pygame.draw.rect(self.screen, (255, 255, 255), rect, 1)

    def draw(self):
        self.__drawGrid()
        self.__drawScorePanel()

        for block in self.blocks:
            block.draw(self.screen)

        for block in self.fallingBlocks:
            block.draw(self.screen)

    def createNewFigure(self):
        # self.blocks += sorted(FigureFabric.next(), key=lambda x: x.y, reverse=True)
        self.fallingBlocks += FigureFabric.next()
        

    def  __isEmptyBellow(self, x, y):
        y = y+1
        for block in self.blocks:
            if block.y == y and block.x == x:
                return False
        return True
        
    def shift(self, s=0):
        for block in self.fallingBlocks:
            if s == -1:
                if block.x > 0:
                    block.shift(s)
            if s == 1:
                if block.x < Const.WIDTH - 1:
                    block.shift(s)

    def fall(self):
        falling = True
        for block in self.fallingBlocks:
            if self.__isEmptyBellow(block.x, block.y):
                if block.y < Const.HEIGHT - 1:
                    continue
            falling = False
            break

        if not falling:           
            self.blocks += self.fallingBlocks
            self.fallingBlocks = []
            self.createNewFigure()
        else:
            for block in self.fallingBlocks:
                block.fall()



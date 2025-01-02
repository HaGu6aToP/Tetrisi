import pygame
from Const import Const

class MainScreen:
    def __init__(self) -> None:
        self.screen = pygame.display.set_mode((Const.SCREEN_WIDTH, Const.SCREEN_HEIGHT))
 
    def __drawCell(self, x, y):
        cell = pygame.Rect(x, y, Const.SQUARE_SIDE, Const.SQUARE_SIDE)
        pygame.draw.rect(self.screen, (255, 255, 255), cell, 1)

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
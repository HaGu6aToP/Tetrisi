import pygame

class MainScreen:
    def __init__(self) -> None:
        self.screen = pygame.display.set_mode((1000, 800))
        self.width = 10
        self.height = 20
        self.squareSide = 30
        self.shift = 7
        self.xLeft = 50
        self.yTop = 50


    def __drawCell(self, x, y):
        cell = pygame.Rect(x, y, self.squareSide, self.squareSide)
        pygame.draw.rect(self.screen, (255, 255, 255), cell, 1)

        rect = pygame.Surface((self.squareSide-self.shift*2, self.squareSide-self.shift*2))
        rect.fill((156, 156, 156))
        rect.set_alpha(3)
        self.screen.blit(rect, (x+self.shift, y+self.shift))

    def __drawGrid(self):
        x = self.xLeft
        for i in range(self.width):
            y = self.yTop
            for j in range(self.height):
                self.__drawCell(x, y)
                y += self.squareSide
            x += self.squareSide

    def __drawScorePanel(self):
        rect = pygame.Rect(self.squareSide*self.width+self.xLeft, self.yTop, self.squareSide*self.width, self.squareSide*self.height/2)
        pygame.draw.rect(self.screen, (255, 255, 255), rect, 1)

    def drow(self):
        self.__drawGrid()
        self.__drawScorePanel()
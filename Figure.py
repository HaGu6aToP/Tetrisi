from Const import Const
import pygame

from scipy.stats import multinomial
from numpy import where

class Block:
    def __init__(self, x=0, y=0) -> None:
        self.x = x
        self.y = y
        self.color = (246, 254, 89)

    def fall(self):
        if self.y < Const.HEIGHT - 1:
            self.y += 1

    def draw(self, screen):
        x = Const.X_LEFT + self.x*Const.SQUARE_SIDE
        y = Const.Y_TOP + self.y*Const.SQUARE_SIDE
        rect = pygame.Rect(x, y, Const.SQUARE_SIDE, Const.SQUARE_SIDE)
        pygame.draw.rect(screen, self.color, rect, 1)
        
        rect.x += Const.SHIFT
        rect.y += Const.SHIFT
        rect.width -= 2*Const.SHIFT
        rect.height -= 2*Const.SHIFT
        pygame.draw.rect(screen, self.color, rect)

class Figure:
    p = [1/4 for i in range(4)]
    def __getFigure():
        t = multinomial.rvs(n=1, p=Figure.p, size=1)[0]
        return where(t==1)[0][0]
        
    def next():
        res = []
        i = Figure.__getFigure()

        if i == 0:
            #  x
            #  x
            #  x
            #  x
            res.append(Block(4, 0))
            res.append(Block(4, 1))
            res.append(Block(4, 2))
            res.append(Block(4, 3))
        elif i == 1:
            #  x x 
            #  x x
            res.append(Block(4, 0))
            res.append(Block(5, 0))
            res.append(Block(4, 1))
            res.append(Block(5, 1))
        elif i == 2:
            #  x
            #  x
            #  x x
            res.append(Block(4, 0))
            res.append(Block(4, 1))
            res.append(Block(4, 2))
            res.append(Block(5, 2))
        elif i == 3:
            #   x
            # x x x
            res.append(Block(5, 0))
            res.append(Block(5, 1))
            res.append(Block(4, 1))
            res.append(Block(6, 1))
        
        return res
            

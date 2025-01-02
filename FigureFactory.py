from Const import Const
import pygame

class Block:
    def __init__(self) -> None:
        self.x = 0
        self.y = 0
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
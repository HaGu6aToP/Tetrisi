import pygame

class Cell:

    def __init__(self, x, y, width, height, color=(255, 255, 255), shift=7):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.color = color
        self.shift = shift
        self.filled = False

    def draw(self, screen):
        rect = pygame.Rect(self.x, self.y, self.width, self.height)
        pygame.draw.rect(screen, self.color, rect, 2)

        if self.filled:
            rect.x += self.shift
            rect.y += self.shift
            rect.width -= 2*self.shift
            rect.height -= 2*self.shift
            pygame.draw.rect(screen, self.color, rect)
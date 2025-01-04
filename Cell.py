import pygame

class Cell:
    def __init__(self,
                 c, # left-top coord 
                 width,
                 height,
                 default_color=(255, 255, 255),
                 background_color=(0, 0, 0),
                 shift=7
                 ):
        self.x = c[0]
        self.y = c[1]
        self.width = width
        self.height = height
        self.default_color = default_color
        self.color = default_color
        self.background_color = background_color
        self.shift = shift
        self.glowing = False

    def draw(self, screen):
        rect = pygame.Rect(self.x, self.y, self.width, self.height)
        pygame.draw.rect(screen, self.color, rect, 2)

        if self.glowing:
            rect.x += self.shift
            rect.y += self.shift
            rect.width -= 2*self.shift
            rect.height -= 2*self.shift
            pygame.draw.rect(screen, self.color, rect)

    def on(self, screen, color):
        self.glowing = True
        self.color = color
        self.draw(screen)

    def off(self, screen):
        self.glowing = False
        self.color = self.default_color
        self.draw(screen)
        rect = pygame.Rect(self.x+self.shift, self.y+self.shift, self.width-2*self.shift, self.height-2*self.shift)
        pygame.draw.rect(screen, self.background_color, rect)
        
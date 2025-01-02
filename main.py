import pygame
from pygame.locals import * 
import sys
from MainScreen import MainScreen

from FigureFabric import Block, FigureFabric

pygame.init()

clock = pygame.time.Clock()

MS = MainScreen()
MS.draw()
MS.createNewFigure()

past = pygame.time.get_ticks()
tick = 1000

while True:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.quit()
                sys.exit()
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
    
    now = pygame.time.get_ticks()
    if now - past > tick:
        MS.update()
        past = now

    MS.draw()
    pygame.display.flip()

    


#
# clock = pygame.time.Clock()

# screen = pygame.display.set_mode((1000, 800))
# width = 6
# height = 17
# squareSide = 30
# shift = 7
# xLeft = 50
# yTop = 50

# #
# mainFIeld = pygame.Rect(xLeft, yTop, squareSide*width, squareSide*height)
# scoreField = pygame.Rect(squareSide*width+xLeft, yTop, squareSide*width, squareSide*height/2)

# #
# def drawCell(x, y):
#     cell = pygame.Rect(x, y, squareSide, squareSide)
#     pygame.draw.rect(screen, (255, 255, 255), cell, 1)

#     rect = pygame.Surface((squareSide-shift*2, squareSide-shift*2))
#     rect.fill((156, 156, 156))
#     rect.set_alpha(100)
#     screen.blit(rect, (x+shift, y+shift))

# def drawGrid():
#     x = xLeft
#     for i in range(width):
#         y = yTop
#         for j in range(height):
#             drawCell(x, y)
#             y += squareSide
#         x += squareSide


# #
# pygame.draw.rect(screen, (255, 255, 255), mainFIeld, 1)
# pygame.draw.rect(screen, (255, 255, 255), scoreField, 1)
# drawGrid()


# rect = pygame.Rect(xLeft, yTop, squareSide, squareSide)
# pygame.draw.rect(screen, (255, 0, 0), rect)

# keyPressed = False

# delta = 0

# velocity = 1

# while True:
#     now = pygame.time.get_ticks()
#     keyPressed = False
#     for event in pygame.event.get():
#         if event.type == pygame.KEYDOWN:
#             if event.key == pygame.K_ESCAPE:
#                 pygame.quit()
#                 sys.exit()
#             if event.key == pygame.K_LEFT:
#                 if not keyPressed:
#                     rect.x -= squareSide
#                     keyPressed = True
#             if event.key == pygame.K_RIGHT:
#                 if not keyPressed:
#                     rect.x += squareSide
#                     keyPressed = True
#             if event.key == pygame.K_DOWN:
#                 # velocity = 2*squareSide
#                 velocity = 5
        
#         if event.type == pygame.KEYUP:
#             if event.key == pygame.K_DOWN:
#                 # velocity = squareSide
#                 velocity = 1

#         if event.type == pygame.QUIT:
#             pygame.quit()
#             sys.exit()

#     pygame.display.flip()

#     if rect.y < yTop + (height - 1)*squareSide:
#         rect.y += squareSide

#     clock.tick(velocity)

#     screen.fill((0, 0, 0))
#     drawGrid()
#     pygame.draw.rect(screen, (255, 0, 0), rect)

#     delta = pygame.time.get_ticks() - now
#     # print(delta)


    
    
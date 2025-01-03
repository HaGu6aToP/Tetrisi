import pygame
from pygame.locals import * 
import sys
from Application import Application

pygame.init()
clock = pygame.time.Clock()

App = Application()
App.drawScreen()
App.createNewFigure()

pastF = pygame.time.get_ticks()
pastS = pygame.time.get_ticks()
fallTick = 1000
shiftTick = 80
shift = 0

while True:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.quit()
                sys.exit()
            if event.key == pygame.K_DOWN:
                fallTick = 50
            if event.key == pygame.K_LEFT:
                shift = -1
            if event.key == pygame.K_RIGHT:
                shift = 1
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_DOWN :
                fallTick = 1000
            if event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                shift = 0
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
    
    nowS = pygame.time.get_ticks()
    if nowS - pastS > shiftTick:
        App.shiftUpdate(shift)
        pastS = nowS

    nowF = pygame.time.get_ticks()
    if nowF - pastF > fallTick:
        App.fallUpdate()
        pastF = nowF
        
        # for block in MS.fallingBlocks:
        #     print(block.x, block.y)
        # print()
    
    App.drawScreen()
    pygame.display.flip()
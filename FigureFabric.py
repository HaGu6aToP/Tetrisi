from Cell import Cell
import pygame

from scipy.stats import multinomial
from numpy import where

class FigureFabric:
    def __init__(self, m, n, color=(246, 33, 33), p=[1/5 for i in range(5)], i=0):
        self.m = m
        self.n = n
        self.p = p
        self.i = i
        self.color = color

    def __getFigure(self):
        t = multinomial.rvs(n=1, p=self.p, size=1)[0]
        return where(t==1)[0][0]
        
    def next(self):
        res = []
        
        if self.i == 0:
            #  x
            #  x
            #  x
            #  x    
            res.append([self.n, self.m+3, self.color])
            res.append([self.n, self.m+2, self.color])
            res.append([self.n, self.m+1, self.color])
            res.append([self.n, self.m, self.color])
        elif self.i == 1:
            #  x x 
            #  x x
            res.append([self.n, self.m+1, self.color])
            res.append([self.n+1, self.m+1, self.color])
            res.append([self.n, self.m, self.color])
            res.append([self.n+1, self.m, self.color])
        elif self.i == 2:
            #  x
            #  x
            #  x x
            res.append([self.n, self.m+2, self.color])
            res.append([self.n+1, self.m+2, self.color])
            res.append([self.n, self.m+1, self.color])
            res.append([self.n, self.m, self.color])
        elif self.i == 3:
            #   x
            # x x x
            res.append([self.n, self.m+1, self.color])
            res.append([self.n-1, self.m+1, self.color])
            res.append([self.n+1, self.m+1, self.color])
            res.append([self.n, self.m, self.color])
        elif self.i == 4:
            #   x x
            # x x
            res.append([self.n, self.m+1, self.color])
            res.append([self.n+1, self.m+1, self.color])
            res.append([self.n+1, self.m, self.color])
            res.append([self.n+2, self.m, self.color])
        
        self.i = self.__getFigure()
        return res
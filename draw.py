# -*- coding: UTF-8 -*-
from matplotlib.pyplot import MultipleLocator
from matplotlib.ticker import FuncFormatter
import matplotlib.pyplot as plt
x = []
y = []
step = 1000000

def get_XY():
    infile='XY.txt'
    f=open(infile, 'r')
    sourceInLine=f.readlines()
    for line in sourceInLine:
        tmp = line.split(',')
        x.append(int(tmp[0]))
        y.append(int(tmp[1]))
    f.close()

fig, ax = plt.subplots(figsize=(8,4))

def to_percent(temp, position):
    return '%d'%(temp//step)

def draw():
    plt.xlabel('Number of Guesses (Millions)', fontsize=10)
    plt.ylabel('Passwords Cracked', fontsize=10)
    plt.plot(x,y)
    plt.gca().xaxis.set_major_formatter(FuncFormatter(to_percent))
    # plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))
    plt.show()

get_XY();
draw();

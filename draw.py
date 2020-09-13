# -*- coding: UTF-8 -*-
from matplotlib.pyplot import MultipleLocator
import matplotlib.pyplot as plt
x = []
y = []

def get_XY():
    infile='XY.txt'
    f=open(infile, 'r')
    sourceInLine=f.readlines()
    for line in sourceInLine:
        tmp = line.split(',')
        x.append(int(tmp[0]))
        y.append(int(tmp[1]))
    f.close()


def draw():
    fig, ax = plt.subplots(figsize=(8,4))
    len = 1000000
    ax.set_yticklabels([str(i) for i in range(-200,2000,200)], rotation=0)
    ax.set_xticklabels([str(i//len) for i in range(-len,10*len,len)], rotation=0)

    plt.xlabel('Number of Guesses (Millions)', fontsize=10)
    plt.ylabel('Passwords Cracked', fontsize=10)
    plt.plot(x,y)
    plt.show()

get_XY();
draw();

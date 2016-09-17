#-*-coding: utf-8-*-

import matplotlib.pyplot as plt
from pylab import *
from math import *
import random






file_name = "/home/alexia/dossiers_partages/Documents_LINUX/Stage/Stage_neuro/Code/V3/Interface/Interface/Results/learning.csv"


def read(file_name):

	f = open(file_name,"r")
	
	x = []
	y = []

	for line in f:
		line = line.strip()
		tmp = line.split(";")
		x.append(tmp[0])
		y.append(tmp[1])

	f.close()
	 
	return x, y


# table stores values from x and y to plot
x,y = read(file_name)




fig, ax = subplots()
ax.axvspan(0.4, 0.6, alpha=0.5, color='red')
ax.axvspan(0.4+2.5, 0.6+2.5, alpha=0.5, color='red')
ax.axvspan(0.4+2*2.5, 0.6+2*2.5, alpha=0.5, color='red')
ax.axvspan(0.4+3*2.5, 0.6+3*2.5, alpha=0.5, color='red')

ylim([-0.100,0.070])

figure(1)
plot(x, y)

savefig("/home/alexia/dossiers_partages/Documents_LINUX/Stage/Stage_neuro/Code/V3/Interface/Interface/Images/learning.png", format="png")




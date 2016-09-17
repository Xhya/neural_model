#-*-coding: utf-8-*-

import matplotlib.pyplot as plt
from pylab import *
from math import *
import random
import os


file_name = "Results/histogram.csv"

time = arange(0,10,0.001)
frequency = []


def read(file_name):

	f = open(file_name,"r")

	frequency_vect = []

	for line in f:
		line = line.strip()
		frequency_vect = line.split(";")

	f.close()


	return frequency_vect


# table stores values from x and y to plot
frequency = read(file_name)


for i in range(len(frequency)):
	frequency[i] = float(frequency[i]) * 1000


figure(1)

fig, ax = subplots()
ax.hist(frequency, 200)
ax.axvspan(400, 600, alpha=0.5, color='red')
ax.axvspan(400+2500, 600+2500, alpha=0.5, color='red')
ax.axvspan(400+2*2500, 600+2*2500, alpha=0.5, color='red')
ax.axvspan(400+3*2500, 600+3*2500, alpha=0.5, color='red')

savefig("Images/histogram.png", format="png")




def change_bin_width(frequency, bin_w):

	begin = 0
	end = bin_w

	first = False 
	key = 0

	for i in range(len(time)):
		if (first == False):
			key = time[i]
			first = True
		else:
			frequency[key] += frequency[i]
			frequency[i] = 0

		if (time[i] >= end):
			begin =	end
			end = end + bin_w
			first = False

	return frequency
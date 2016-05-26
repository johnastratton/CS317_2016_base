"""
Plots cells over time
Copyright (C) 2012 Ahmet Ay, Jack Holland, Adriana Sperlea

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import numpy
import scipy
import pylab as pl
import time
from time import clock
from scipy.stats.stats import pearsonr
import sys
import shared
import struct

def main():
	# check the given arguments
	if len(sys.argv) < 5:
		usage()
	else:
		f = shared.openFile(sys.argv[1], "r")
		directory = sys.argv[2]
		image_name = sys.argv[3]
		step_size = shared.toFlo(sys.argv[4])
	
	print 'Plotting all the cells from ' + sys.argv[1] + '...'
	# split the lines to get data
	data = [line.split() for line in f]
	max_time = len(data) - 1
	
	# calculate the tissue size
	cells_width = shared.toInt(data[0][0])
	cells_height = shared.toInt(data[0][1])
	total_cells = cells_width * cells_height + 1
	
	# create a matrix to store the concentration values we obtain from the file
	cons = numpy.zeros(shape = (max_time, total_cells))
	
	# put the concentration values from the file into the matrix
	for i in range(1, max_time + 1):
		cons[i - 1][0] = shared.toFlo(data[i][0]) * step_size
		for j in range(1, total_cells):
			cons[i - 1][j] = shared.toFlo(data[i][j])
	
	# close the file
	f.close()
	
	# plot colors
	colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
	color = 0
	
	for i in range(1, total_cells):
		start = 0
		
		# Adjust the plotting interval for each cell to account for different columns being staggered
		# as they enter the PSM at intervals of 6 minutes apart frome ach other
		while cons[start][i] == -1: # -1 stands for no data in the output file
			start += 1
		end = max_time - 1
		while cons[end][i] == -1:
			end -= 1;

		if (i % 4 == 0):
			pl.plot(cons[start:end, 0], cons[start:end, i], 'r')
		elif (i % 4 == 1):
			pl.plot(cons[start:end, 0], cons[start:end, i], 'g')
		elif (i % 4 == 2):
			pl.plot(cons[start:end, 0], cons[start:end, i], 'b')
		else:
			pl.plot(cons[start:end, 0], cons[start:end, i], 'c')
	
	pl.savefig(directory + "/" + image_name + ".png", format = "png")
	pl.close()
	print 'Done. Your plot is stored in ' + directory + "/" + image_name + ".png"

def usage():
	print "Usage: python plot-cells.py <file with concentration levels> <directory to store plots> <plot name> <step size>"
	exit(0)

main()


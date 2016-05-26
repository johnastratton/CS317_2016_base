"""
Plots each cell individually and as an average
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

def main():
	# check the given arguments
	if len(sys.argv) < 6:
		usage()
	elif len(sys.argv) == 7:
		if sys.argv[1] == "-c" or sys.argv[1] == "--no-color":
			shared.terminalRed = ""
			shared.terminalReset = ""
			filename = sys.argv[2]
			filename2 = sys.argv[3]
			directory = sys.argv[4]
			measuring = sys.argv[5]
			mutation = sys.argv[6]

		else:
			usage()
	else:
		filename = sys.argv[1]
		directory = sys.argv[2]
		measuring = sys.argv[3]
		mutation = sys.argv[4]
	
	# open the input file and ensure the directory exists
	f = shared.openFile(filename, "r")
	f2 = shared.openFile(filename2, "r")
	directory = shared.ensureDir(directory)
	
	# split the lines to get data
	data = [line.split() for line in f]
	file_len = len(data) - 1
	max_x = file_len
	f.close()
	

	data2 = [line.split() for line in f2]
	file_len2 = len(data2) - 1
	max_x2 = file_len2
	f2.close()

	if (max_x == max_x2):
		print "test"
	# number of columns we have in the files
	cn = shared.toInt(data[0][0]) * shared.toInt(data[0][1]) + 1
	cn2 = shared.toInt(data2[0][0]) * shared.toInt(data2[0][1]) + 1
	
	# create matrices to store the data we obtained from the files
	m2p=numpy.zeros(shape = (max_x,cn + cn2))
	
	# put the data coming from the files to the matrix
	for i in range(2, file_len):
		for j in range(0, cn+cn2):
			if (j <cn):
				m2p[i][j] = shared.toFlo(data[i][j])
				
			elif (j==cn):
				print data2[i][j-cn]
			else:
				m2p[i][j] = 2*shared.toFlo(data2[i][j-cn])
				
	
	# plot colors
	colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
	color = 0
	
	for i in range(1, cn+cn2):
		if (i % 4 == 0):
			pl.plot(m2p[0:max_x, 0], m2p[0:max_x, i], 'r')
		elif (i % 4 == 1):
			pl.plot(m2p[0:max_x, 0], m2p[0:max_x, i], 'g')
		elif (i % 4 == 2):
			pl.plot(m2p[0:max_x, 0], m2p[0:max_x, i], 'b')
		else:
			pl.plot(m2p[0:max_x, 0], m2p[0:max_x, i], 'c')
	
	pl.title(measuring + " " + mutation + " All Cells")
	pl.savefig(directory + "/" + mutation + "_all.png", format = "png")
	pl.close()
	
	# plot the data
	average = []
	for i in range(0, max_x):
		average.append(float(sum(m2p[i][1:])) / float(len(m2p[i][1:])))
	
	pl.plot(m2p[0:max_x, 0], average, colors[color])
	if color == len(colors) - 1:
		color = 0
	else:
		color += 1
	
	pl.title(measuring + " " + mutation + " Average")
	pl.savefig(directory + "/" + mutation + "_avg.png", format = "png")
	pl.close()

def usage():
	print "Usage: python plot.py [-c|--no-color] <file1 with concentration levels> <file2 with concentration levels> <directory to store plots> <what it's measuring> <mutation name>"
	exit(0)

main()


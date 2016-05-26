"""
Plots the synchronization between cells
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

import matplotlib.pyplot as plt
import pylab
import sys
import shared
import math

def main():
	# check the given arguments
	if len(sys.argv) < 6:
		usage()
	else:
		folder = sys.argv[1]
		parsets = shared.toInt(sys.argv[2])
		ofolder = sys.argv[3]
		image_name = sys.argv[4]
		excel_name = sys.argv[5]

	mutants = ["wildtype", "delta", "her1", "her7", "her7her13", "her13"]
	markers = ['o', '^', 's', '*', 'h', 'D']
	colors = ['k', 'b', 'g', 'r', 'c', 'm']
	num_mutants = 6
	
	# Create excel file in which the data used to create the plots will be stored
	excel_file = shared.openFile(ofolder + "/" + excel_name + "-sync.csv", "w")
	
	for index in range(num_mutants):

		mutant = mutants[index]
		marker = markers[index]
		color = colors[index]
			
		# open the first file to get the height, width and interval
		f = shared.openFile(folder + "/" + mutant + "/set_0_sync_mh1.feats", "r")

		# split the lines to get data
		data = [line.split(",") for line in f]
	
		# calculate the tissue size
		height = shared.toInt(data[0][0])
		interval = shared.toFlo(data[0][1])
		#split_time = shared.toFlo(data[0][2])
		width = len(data[1]) - 1

		indexes = [0 for i in range(width)]
		averages = [0 for i in range(width)]
		stderr = [0 for i in range(width)]
		
		for parset in range(parsets):
			f = shared.openFile(folder + "/" + mutant + "/set_" + str(parset) + "_sync_mh1.feats", "r")
	
			# split the lines to get data
			data = [line.split(",") for line in f]
		
			for col in range(width):
				for line in range(1, height + 1):
					averages[col] += shared.toFlo(data[line][col])
		
			f.close()
				
		for col in range(width):
			indexes[col] = (((interval / 2) * col + (interval / 2) * col + interval) / 2) / 6
			averages[col] /= height * parsets
			
		if mutant == "wildtype":
			excel_file.write("mutant,")
			for index in indexes:
				excel_file.write(str(index) + ",")
			excel_file.write("\n")

		for parset in range(parsets):
			f = shared.openFile(folder + "/" + mutant + "/set_" + str(parset) + "_sync_mh1.feats", "r")
		
			data = [line.split(",") for line in f]
		
			# std error = std deviation / sqrt(num data points)
			for col in range(width):
				for line in range(1, height + 1):
					stderr[col] += (shared.toFlo(data[line][col]) - averages[col]) ** 2
				stderr[col] = math.sqrt(stderr[col] / (height * parsets))
				stderr[col] /= math.sqrt(height * parsets)	

		# Print the means and standard deviations to the excel_file
		excel_file.write(mutant + ",")
		for average in averages:
			excel_file.write(str(average) + ",")
		excel_file.write("\n,")
		for stder in stderr:
			excel_file.write(str(stder) + ",")
		excel_file.write("\n")

		plt.errorbar(indexes, averages, stderr, fmt='ro', linestyle='-', marker=marker, color=color, label=mutant)
	plt.legend(prop={'size':8}, loc=3)
	pylab.xlim([0, (width + 1) * (interval / 2) / 6])
	plt.savefig(ofolder + "/" + image_name + ".png", format = "png")
	plt.close()

def usage():
	print "Usage: python plot-synchronization.py <directory with synchronization files> <number of parameter sets> <directory to store the image> <name of image> <name of excel file>"
	exit(0)

main()


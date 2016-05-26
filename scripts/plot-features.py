"""
Plots the period and/or amplitude change from posterior to anterior for the wildtype and every mutant.
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
	print 'Reading command line arguments...'
	# check the given arguments
	if len(sys.argv) < 8:
		usage()
	else:
		folder = sys.argv[1]
		parsets = shared.toInt(sys.argv[2])
		image_name = sys.argv[3]
		feature = sys.argv[4]
		ofolder = sys.argv[5]
		post_width = shared.toInt(sys.argv[6])
		excel_name = sys.argv[7]
		
	num_mutants = 6
	index = 0
	mutants = ["wildtype", "delta", "her1", "her7", "her7her13", "her13"]
	markers = ['o', '^', 's', '*', 'h', 'D']
	colors = ['k', 'b', 'g', 'r', 'c', 'm']
	

	features = []
	if (feature == "period" or feature == "amplitude"):
		features.append(feature)
	else:
		features.append("period")
		features.append("amplitude")
	
	for feat in features:
		# Create excel file in which the data used to create the plots will be stored
		excel_file = shared.openFile(ofolder + "/" + excel_name + "-" + feat + ".csv", "w")
		print "Plotting ", feat, "..."
		first_avg = 0
		num_first = 0
	
		for index in range(num_mutants):
			mutant = mutants[index]
			print '    Running ' + mutant + '...'
			marker = markers[index]
			color = colors[index]
			# open the input file
			f = shared.openFile(folder + "/" + mutant + "/set_0_" + feat + "_mh1.feats", "r")

			# split the lines to get data
			data = [line.split(",") for line in f]
	
			# calculate the tissue size
			height = shared.toInt(data[0][0])
			width = shared.toInt(data[0][1])
			xmin = 0
			xmax = 0.9 * width

			buckets = 9 # split the interval into 9 chunks
			chunk = (width - post_width) / (buckets - 1) # the width of the intervals after the posterior
				
			indexes = [0 for i in range(buckets)]
			for bucket in range(buckets):
				if bucket == 0:
					indexes[bucket] = post_width / 2
				else:
					indexes[bucket] = (post_width + (bucket - 1) * chunk) + (chunk / 2.0)
			averages = [0 for i in range(buckets)]
			num_points = [0 for i in range(buckets)]
			stderr = [0 for i in range(buckets)]

			if mutant == "wildtype":
				excel_file.write("mutant,")
				for index in indexes:
					excel_file.write(str(index) + ",")
				excel_file.write("\n")
			
				print '        Averaging the first bucket for the wildtype...' # all other data points will be averaged to this value
				for parset in range(parsets):
					# open the input file and ensure the directory exists
					f = shared.openFile(folder + "/" + mutant + "/set_" + str(parset) + "_" + feat + "_mh1.feats", "r")
	
					# split the lines to get data
					data = [line.split(",") for line in f]
					lines = len(data)
		
					for line in range(1, lines, 2):
						for col in range(len(data[line]) - 1):
							pos = shared.toInt(data[line][col])
							val = shared.toFlo(data[line + 1][col])
							if pos < post_width:
								first_avg += val
								num_first += 1
					
				first_avg /= num_first
	
			for parset in range(parsets):
				print '        Normalizing and analyzing data from set ' + str(parset) + '...'
				# open the input file and ensure the directory exists
				f = shared.openFile(folder + "/" + mutant + "/set_" + str(parset) + "_" + feat + "_mh1.feats", "r")
	
				# split the lines to get data
				data = [line.split(",") for line in f]
				lines = len(data)
		
				for line in range(1, lines, 2):
					for col in range(len(data[line]) - 1):
						pos = shared.toInt(data[line][col])
						val = shared.toFlo(data[line + 1][col]) / first_avg

						if pos < post_width:
							averages[0] += val
							num_points[0] += 1
						else:
							averages[(pos - post_width) / chunk + 1] += val
							num_points[(pos - post_width) / chunk + 1] += 1
		
			# ignore the buckets which don't have data
			buckets_with_data = buckets	

			for bucket in range(buckets):
				if post_width + ((bucket - 1) * chunk) + chunk - 1 > (0.9 * width):
					buckets_with_data -= 1
				else:
					if num_points[bucket] > 0:
						averages[bucket] /= num_points[bucket]
					elif feat == "amplitude":
						averages[bucket] = 0
					else:
						buckets_with_data -= 1

			buckets = buckets_with_data

			print '        Calculating standard error...'
			for parset in range(parsets):
				f = shared.openFile(folder + "/" + mutant + "/set_" + str(parset) + "_" + feat + "_mh1.feats", "r")
		
				data = [line.split(",") for line in f]
				lines = len(data)
		
				for line in range(1, lines, 2):
					for col in range(len(data[line]) - 1):
						pos = shared.toInt(data[line][col])
						val = shared.toFlo(data[line + 1][col]) / first_avg

						if pos < post_width:
							stderr[0] += (val - averages[0]) ** 2
						else:
							stderr[(pos - post_width) / chunk + 1] += (val - averages[(pos - post_width) / chunk + 1]) ** 2
			
			for bucket in range(buckets):
				if (num_points[bucket] > 0):
					stderr[bucket] = math.sqrt(stderr[bucket] / num_points[bucket])
					stderr[bucket] /= math.sqrt(num_points[bucket])
				else:
					stderr[bucket] = 0

			indexes = indexes[:buckets]
			averages = averages[:buckets]
			stderr = stderr[:buckets]
			# Print the means and standard deviations to the excel_file
			excel_file.write(mutant + ",")
			for average in averages:
				excel_file.write(str(average) + ",")
			excel_file.write("\n,")
			for stder in stderr:
				excel_file.write(str(stder) + ",")
			excel_file.write("\n")
			
			plt.errorbar(indexes, averages, stderr, fmt='ro', linestyle='-', marker=marker, color=color, label=mutant)
		plt.legend(prop={'size':8}, loc=2)
		pylab.xlim([xmin, xmax])
		excel_file.close()
		plt.savefig(ofolder + "/" + image_name + "_" + feat + ".png", format = "png")
		plt.close()
		print "Done. Your " + feat + " plot is stored in " + ofolder + "/" + image_name + "_" + feat + ".png"
		print "The data behind the plot can be found in " + ofolder + "/" + excel_name + "-" + feat + ".csv"

def usage():
	print "Usage: python plot-features.py <directory with period files> <number of parameter sets> <name of image> <feature(s) to plot> <directory to save image(s)> <width of the posterior> <name of excel file>"
	exit(0)

main()


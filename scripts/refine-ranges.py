"""
Refine existing ranges into more precise ones based on parameter sets
Copyright (C) 2013 Ahmet Ay, Jack Holland, Adriana Sperlea, Sebastian Sangervasi

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

import math
import sys
import shared

def main():
	print 'Reading command-line arguments...'
	args = sys.argv[1:] # Remove the name of the program from the arguments
	num_args = len(args)
	req_args = [False] * 3 # If every required argument was given then req_args will be all true
	if num_args >= 3:
		# Arguments with default values
		stdevs_away = 2
		round_to = 5
		
		for arg in range(0, num_args - 1, 2):
			option = args[arg]
			value = args[arg + 1]
			if option == '-s' or option == '--sets':
				sets_file = shared.openFile(value, 'r')
				req_args[0] = True
			elif option == '-c' or option == '--current-ranges':
				cur_ranges_file = shared.openFile(value, 'r')
				req_args[1] = True
			elif option == '-n' or option == '--new-ranges':
				new_ranges_fname = value
				new_ranges_file = shared.openFile(new_ranges_fname, 'w')
				req_args[2] = True
			elif option == '-d' or option == '--standard-dev':
				stdevs_away = shared.toInt(value)
			elif option == '-r' or option == '--round-to':
				round_to = shared.toInt(value)
			elif option == '-h' or option == '--help':
				usage()
			else:
				usage()
		for arg in req_args: # Check to ensure every required argument was entered
			if not arg:
				usage()
	else:
		usage()
	
	print 'Reading the parameter sets file...'
	# Parse the sets file to get the list of parameter sets
	sets = []
	for line in sets_file:
		if not(line == '' or line[0] == '#'): # Ignore blank lines and comments
			sets.append(line)
	if len(sets) < 1: # Ensure at least one set was given
		usage()
	
	print 'Reading the current ranges file...'
	# Parse the current ranges file to find the existing ranges
	par_names = []
	cur_ranges = []
	for line in cur_ranges_file:
		line = line.replace('\t', ' ')
		if not(line == '' or line[0] == '#'): # Ignore blank lines and comments
			# Get the human-readable description
			space = line.find(' ')
			if space <= 0:
				parsing_error()
			par_names.append(line[: space])
			line = line[space + 1:] # Skip past the description
			
			# Find the range bounds
			start = 0
			if line[start] == '\0':
				parsing_error()
			while line[start] == ' ':
				start += 1
			if line[start] != '[':
				parsing_error()
			end = start + 1
			while line[end] != ']' and line[end] != '\0':
				end += 1
			if line[end] == '\0':
				parsing_error()
			line = line[start + 1 : end]
			bounds = map(shared.toFlo, line.split(',')) # Convert the bounds to floats
			if len(bounds) != 2:
				parsing_error()
			cur_ranges.append(bounds)
	
	print 'Calculating new ranges...'
	# Calculate each parameter's new range
	flo_sets = map(lambda ls: map(shared.toFlo, ls), map(lambda s: s.split(','), sets)) # Convert each parameter set string into an array of floats
	num_sets = len(flo_sets)
	new_ranges = []
	for p in range(len(cur_ranges)): # For every range
		# Get the mean range based on every set
		vals = []
		for s in flo_sets:
			vals.append(s[p])
		mean = sum(vals) / num_sets
		# Calculate the standard deviation from the mean
		stdev_sum = 0
		for f in vals:
			stdev_sum += (f - mean) ** 2
		stdev = math.sqrt(stdev_sum / num_sets)
		# Define new ranges based on the mean and standard deviation that are at least as narrow as the current ranges
		lower_bound = max(cur_ranges[p][0], round(mean - stdev * stdevs_away, round_to))
		upper_bound = min(cur_ranges[p][1], round(mean + stdev * stdevs_away, round_to))
		new_ranges.append([lower_bound, upper_bound])
	
	print 'Writing the new ranges to the specified output file...'
	# Write the parameter ranges to the new ranges file
	for r in range(len(new_ranges)):
		new_ranges_file.write(par_names[r] + ' [' + str(new_ranges[r][0]) + ',' + str(new_ranges[r][1]) + ']\n')
	new_ranges_file.close()
	
	print 'Done. The new ranges are in ' + new_ranges_fname

def parsing_error():
	print 'There was a error parsing the current ranges file!'
	exit(1)

def usage():
	print 'Usage: python refine-ranges.py (-short_option value | --long_option value)...'
	print '-s, --sets           [filename] : the relative filename of the parameter sets to run the simulations with, required'
	print '-c, --current-ranges [filename] : the relative filename of the ranges file to base new ranges on, required'
	print '-n, --new-ranges     [filename] : the relative filename of the ranges file to stores the new ranges in, required'
	print '-d, --standard-dev   [int]      : how many standard deviations to stray from each mean range, default=2'
	print '-r, --round-to       [int]      : how many digits to round each new range to, default=5'
	print '-h, --help           [N/A]      : view usage information (i.e. this)'
	exit(0)

main()


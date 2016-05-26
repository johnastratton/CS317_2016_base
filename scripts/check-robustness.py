"""
Counts how many parameter sets pass with a threshold number of seeds
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

import sys
import shared

def main():
	#check the given arguments
	print "Reading command-line arguments..."
	args = sys.argv[1:]
	num_args = len(args)
	req_args = [False] * 8

	if num_args == 16:
		for arg in range(0, num_args - 1, 2):
			option = args[arg]
			value = args[arg + 1]
			if option == '-S' or option == '--seeds':
				num_seeds = shared.toInt(value)
				req_args[0] = True
			elif option == '-n' or option == '--num-params':
				num_sets = shared.toInt(value)
				req_args[1] = True
			elif option == '-o' or option == '--output-file':
				output_file = value
				req_args[2] = True
			elif option == '-f' or option == '--num-files':
				num_files = shared.toInt(value)
				req_args[3] = True
			elif option == '-d' or option == '--directory':
				folder = value
				req_args[4] = True
			elif option == '-m' or option == '--max-score':
				max_score = shared.toInt(value)
				req_args[5] = True
			elif option == '-i' or option == '--input-file':
				input_file = value
				req_args[6] = True
			elif option == '-t' or option == '--threshold':
				threshold = shared.toInt(value)
				req_args[7] = True
			elif option == '-h' or option == '--help':
				usage()
			else:
				usage()
		for arg in req_args:
			if not arg:
				usage()
	else:
		usage()
				
	parsets = [[0 for i in range(num_seeds)] for i in range(num_sets)]
	robust = open(output_file, "w")
	
	for seeds in range(num_seeds):
		seed = (seeds + 1) * 1000
		parindex = 0
		for i in range(0, num_files):
			scores = open(folder + "/scores-" + str(seed) + "-" + str(i) + ".csv")
			scores.readline() #skip the header line in the scores file

			for result in scores:			
				line = result.split(",")
				if (int(line[-2]) == max_score): #the total score is the last column in the scores file, after the '\n' character
					parsets[parindex][seed / 1000] += 1
				parindex += 1
	
	num_robust = 0
	pars = open(input_file, "r")
	for i in range(parindex):
		parset = pars.readline()
		if sum(parsets[i]) >= threshold:
			num_robust += 1
			robust.write(parset)
		
	print num_robust, " sets passed in more than ", threshold, " seeds "
	robust.close()

def usage():
	print 'Usage: python check-robustness.py (-short_option value | --long_option value)...'
	print '-S, --seeds       [int]       : the number of seeds for which the parameters were run'
	print '-n, --num-params  [int]       : the number of sets that were tested for robustness'
	print '-o, --output-file [filename]  : the name of the output file to which the robust sets will be printed'
	print '-f, --num-files   [int]       : the number of scores files which were created by the robustness runs'
	print '-d, --directory   [directory] : the relative path of the directory in which the scores files are located'
	print '-m, --max_score   [int]       : the score obtained by a passing parameter set'
	print '-i, --input_file  [filename]  : the relative path of the file containing the original paremeter sets which are being tested for robustness'
	print '-t, --threshold   [int]       : the number of seeds that a parameter has to pass in order to be deemed robust'
	print '-h, --help        [N/A]       : view usage information (i.e. this)'
	exit(0)
	
main()


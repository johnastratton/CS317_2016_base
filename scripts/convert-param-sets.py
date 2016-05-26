"""
Converts parameter sets between formats (e.g. 45 parameters to 88 parameters)
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
	print 'Reading command-line arguments...'
	args = sys.argv[1:] # Remove the name of the program from the arguments
	num_args = len(args)
	if num_args == 3: # There are three arguments, each of which is required
		input_file = shared.openFile(args[0], 'r') # The input parameter sets
		output_fname = args[1]
		output_file = shared.openFile(output_fname, 'w') # The output parameter sets
		num_output_params = shared.toInt(args[2]) # How many parameters each set should have in the output file
	else:
		usage()
	
	print 'Converting each parameter set...'
	num_input_params = -1
	for line in input_file: # For every parameter set
		if len(line) > 1 and line[0] != '#': # Skip blank lines and comments
			input_set = line.split(',')
			if num_input_params == -1: # Find the input format based on the first parameter set found
				num_input_params = len(input_set)
			else:
				output_file.write('\n') # Print a newline before each non-first set
			
			input_set[num_input_params - 1].replace('\n', '') # Get rid of the newline in the last parameter
			
			# Convert the set to the master (88) format
			base_set = ['0'] * 88
			for par in range(num_input_params):
				base_index = set_formats[num_input_params][par]
				base_set[base_index] = input_set[par]
			
			# Convert the master format to the specified one
			output_set = ['0'] * num_output_params
			for par in range(num_output_params):
				output_index = set_formats[num_output_params][par]
				output_set[par] = base_set[output_index]
			
			# Write the results to the output file
			output_file.write(output_set[0])
			for par in range(1, num_output_params):
				output_file.write(',' + output_set[par])
	
	print 'Closing files...'
	input_file.close()
	output_file.close()
	
	print 'Done. Your newly formatted parameter sets are stored in ' + output_fname

# Each parameter set format's indices relative to the master (88) format, i.e. a number x at index y means that in the given format, the rate index y is the equivalent to the master's rate index x
set_formats = {
#	  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
#	 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87
27:	[ 0,  1,  3,  5,  6,  7,  9, 11, 12, 13, 15, 17, 18, 19, 21, 23, 69, 70, 72, 74, 75, 76, 78, 80, 81, 82, 87],
45:	[ 0,  1,  3,  5,  6,  7,  9, 11, 12, 13, 15, 17, 18, 19, 21, 23, 24, 25, 27, 29, 31, 36, 39, 40, 42, 44, 46, 51, 54, 55, 57, 59, 61, 66, 69, 70, 72, 74, 75, 76, 78, 80, 81, 82, 87],
65:	[ 0,  1,  2,  3,  5,  6,  7,  8,  9, 11, 12, 13, 14, 15, 17, 18, 19, 20, 21, 23, 24, 25, 26, 27, 29, 30, 31, 33, 34, 36, 39, 40, 41, 42, 44, 45, 46, 48, 49, 51, 54, 55, 56, 57, 59,
	 60, 61, 63, 64, 66, 69, 70, 71, 72, 74, 75, 76, 77, 78, 80, 81, 82, 83, 84, 87],
88:	[ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
	 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87]
}

def usage():
	print 'Usage: python convert-param-sets.py <input file: parameter sets to convert> <output file: converted parameter sets> <int: number of parameters in the converted sets>'
	exit(0)

main()


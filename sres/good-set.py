"""
File parser that collects the good set from a result file after an SRES run
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

import sys
import shared

def main():
	if len(sys.argv) != 3:
		usage()
	else:
		input_filename = sys.argv[1]
		output_filename = sys.argv[2]

	input_file = shared.openFile(input_filename, "r")
	output_file = shared.openFile(output_filename, "w")

	lines = [line for line in input_file]
	for i in range(len(lines)):
		if "Found a good set" in lines[i]:
			good_set = lines[i+1][:]
			starting_index = good_set.find(',')
			good_set = good_set[starting_index+1:]
			output_file.write(good_set)

	input_file.close()
	output_file.close()

def usage():
	print "Wrong number of arguments"
	exit(0)

main()
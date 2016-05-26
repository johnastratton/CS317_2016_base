"""
Shared functionality for python scripts
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

import os

# try to open the file specified by the given filename
def openFile(filename, mode):
	try:
		return open(filename, mode)
	except IOError:
		print "Couldn't open '" + filename + "'!"
		exit(1);

# ensure the given directory exists
def ensureDir(directory):
	if directory[-1] == '/':
		directory = directory[:-1]
	if not os.path.exists(directory):
		os.makedirs(directory)
	return directory

# try to cast the given value to an integer
def toInt(val):
	try:
		val = int(val)
	except ValueError:
		print "'" + val + "' is not an integer!"
		exit(2)
	return val

# try to cast the given value to a float
def toFlo(val):
	try:
		val = float(val)
	except ValueError:
		print "'" + val + "' is not a number!"
		exit(2)
	return val

# extract the width and height from the given line
def widthAndHeight(line, filename):
	if len(line) == 2:
		width = toInt(line[0])
		height = toInt(line[1])
		if width == 0 or height == 0:
			print "The width and height of the tissue must be positive integers!"
			exit(2)
	else:
		print "The first line of", filename, "must contain the width and height of the tissue!"
		exit(2)
	return width, height


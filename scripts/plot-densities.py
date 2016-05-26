"""
Prints a spatiotemporal figure depicting cell oscillations through time
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

import Image, ImageDraw
import sys
import shared

def main():
	print 'Reading command-line arguments...'
	args = sys.argv[1:] # Remove the name of the program from the arguments
	num_args = len(args)
	cons_file = None # Concentrations file
	figure_fname = 'st' # Filename to give the figure minus its extension
	image_format = 'png' # Format in which to save the figure, also serving as its extension
	image_width = 1000 # Width of the image in pixels
	image_height = 250 # Height of the image in pixels
	steps_til_growth = 60000 # Steps before growth starts
	steps_to_split = 600 # Steps each split takes
	initial_width = 10 # Initial width of the PSM in cells
	granularity = 1 # Use each <granularity> time steps of data
	start_step = 0 # Start time step relative to steps_til_growth
	end_step = 60000 # End time step relative to steps_til_growth
	for arg in range(0, num_args - 1, 2):
		option = args[arg]
		value = args[arg + 1]
		if option == '-c' or option == '--cons-file':
			cons_file = shared.openFile(value, 'r')
		elif option == '-f' or option == '--figure-name':
			figure_fname = value
		elif option == '-i' or option == '--image-format':
			image_format = value
		elif option == '-w' or option == '--image-width':
			image_width = shared.toInt(value)
		elif option == '-h' or option == '--image-height':
			image_height = shared.toInt(value)
		elif option == '-G' or option == '--steps-til-growth':
			steps_til_growth = shared.toInt(value)
		elif option == '-S' or option == '--steps-to-split':
			steps_to_split = shared.toInt(value)
		elif option == '-n' or option == '--initial-width':
			initial_width = shared.toInt(value)
		elif option == '-g' or option == '--granularity':
			granularity = shared.toInt(value)
		elif option == '-s' or option == '--start-step':
			start_step = shared.toInt(value)
		elif option == '-e' or option == '--end-step':
			end_step = shared.toInt(value)
		elif option == '-h' or option == '--help':
			usage()
		else:
			usage()
	if cons_file is None: # The concentrations file is required
		usage()
	
	print 'Parsing concentrations file...'
	raw_data = [line.split() for line in cons_file] # Split the data into lines and split each line by spaces into an array
	cons_file.close()
	
	print 'Converting data to the appropriate sizes...'
	# Take the width and height from the first line of the file
	psm_width = shared.toInt(raw_data[0][0])
	psm_height = shared.toInt(raw_data[0][1])
	raw_data = raw_data[1 + steps_til_growth:] # Remove all data before growth starts
	# Adjust step sizes for the given granularity
	steps_til_growth /= granularity
	steps_to_split /= granularity
	data = [] # Like raw data, but takes only each <granularity> time steps of data and removes the time steps column
	for line in range(len(raw_data)):
		if line % granularity == 0:
			data.append(raw_data[line])
	total_steps = len(data)
	for row in range(total_steps):
		data[row] = data[row][1:] # Remove the time steps column
	steps_when_full = (psm_width - initial_width) * steps_to_split # When the PSM is done growing
	total_width = psm_width + (total_steps - steps_when_full) / steps_to_split # The width of every cell that exists at any point
	table = [[0 for i in range(total_steps)] for j in range(total_width)] # A table containing the data formatted more closely to what the figure requires
	
	print 'Accounting for cell growth and averaging cell columns...'
	min_con = float('inf')
	max_con = 0
	
	# Fill in the table with all data from when the PSM is growing
	current_width = initial_width
	row_start = current_width - 1
	steps_elapsed = 0
	for column in range(steps_when_full):
		for row in range(current_width):
			avg_con = 0
			cell_x = row_start - row # Posterior cells should be printed on the right
			for cell_y in range(psm_height): # Average each column of cells
				cell_index = cell_y * psm_width + cell_x
				avg_con += shared.toFlo(data[column][cell_index])
			avg_con /= psm_height
			table[row][column] = avg_con
			# Update the minimum and maximum concentrations
			min_con = min(min_con, avg_con)
			max_con = max(max_con, avg_con)
		for row in range(current_width, total_width): # Nonexistent cells get concentrations of 0
			table[row][column] = -10
		steps_elapsed += 1
		if steps_elapsed == steps_to_split: # Split the PSM every steps_to_split time steps
			current_width += 1
			row_start += 1 # Adjust because the first cell in data is the new, posterior-most cell
			steps_elapsed = 0
	for column in range(steps_when_full, total_steps):
		for row in range(current_width, total_width):
			table[row][column] = -10
	
	# Fill in the table with all data from when the PSM is done growing
	arrested_cells = []
	row_start = psm_width - 1
	row_offset = 0
	for column in range(steps_when_full, total_steps):
		for row in range(psm_width):
			avg_con = 0
			cell_x = (row_start - row) % psm_width # Posterior cells should be printed on the right
			for cell_y in range(psm_height): # Average each column of cells
				cell_index = cell_y * psm_width + cell_x
				avg_con += shared.toFlo(data[column][cell_index])
			avg_con /= psm_height
			table[row + row_offset][column] = avg_con
			# Update the minimum and maximum concentrations
			min_con = min(min_con, avg_con)
			max_con = max(max_con, avg_con)
		for cell in arrested_cells: # Print the last value each arrested cell had for the rest of time
			table[cell[0]][column] = -10

		steps_elapsed += 1
		if steps_elapsed == steps_to_split: # Split the PSM every steps_to_split time steps
			arrested_cells.append((row_offset, table[row_offset][column]))
			row_offset += 1
			steps_elapsed = 0
	max_con += 1
	print 'Cropping to the specified time range...'
	start_step /= granularity
	end_step /= granularity
	total_steps = end_step - start_step
	for row in range(total_width):
		table[row] = table[row][start_step:end_step]
	print 'Creating a blank image...'
	im = Image.new('RGB', (image_width, image_height), rgb('FFFFFF')) # Make an image with a blank, white canvas
	draw = ImageDraw.Draw(im) # Get the drawing object
	
	print 'Filling the image with the concentrations...'
	# Find the factors to scale the table data into an image_width by image_height sized figure
	x_factor = shared.toFlo(total_steps) / image_width
	y_factor = shared.toFlo(total_width) / image_height
	# Darker shades indicate higher concentrations
	shades = [rgb('FEB4EF'), rgb('FEB4EF'), rgb('FE5A77'), rgb('FE2D3B'), rgb('FF0000'), rgb('BF0000'), rgb('7F0000'), rgb('3F0000'), rgb('000000'), rgb('FFFFFF')]
	num_shades = len(shades)
	for i in range(image_width):
		x = shared.toInt(i * x_factor)
		for j in range(image_height):
			reverse_j = image_height - j - 1 # In the figure, cell 0 is at the bottom, not top
			y = shared.toInt(reverse_j * y_factor)
			con = table[y][x]
			if con == -10:
				color = rgb('EEE5DE')
			else:
				color = shades[int((con - min_con) / (max_con - min_con) * (num_shades - 1))] # Find the color matching the concentration
			draw.point((i, j), fill = color)
	
	print 'Saving the image...'
	figure_fname_full = figure_fname + '.' + image_format.lower()
	im.save(figure_fname_full, image_format.upper())
	
	print 'Done. Your figure is stored in ' + figure_fname_full

# Converts an RGB string into a hexadecimal triplet
hex_digits = '0123456789abcdef'
def rgb(triplet):
    triplet = triplet.lower()
    return (hex_digits.index(triplet[0]) * 16 + hex_digits.index(triplet[1]),
            hex_digits.index(triplet[2]) * 16 + hex_digits.index(triplet[3]),
            hex_digits.index(triplet[4]) * 16 + hex_digits.index(triplet[5]))

def usage():
	print 'Usage: python spatiotemporal.py (-short_option value | --long_option value)...'
	print '-c, --cons-file        [filename] : the relative filename that stores the concentrations, required'
	print '-f, --figure-name      [string]   : the name to give the figure, not including its extension, default=densities'
	print '-i, --image-format     [string]   : the format with which to save the image, default=png'
	print '-w, --image-width      [int]      : the image width in pixels, default=1000'
	print '-h, --image-height     [int]      : the image height in pixels, default=250'
	print '-G, --steps-til-growth [int]      : the number of steps until growth starts, default=60000'
	print '-S, --steps-to-split   [int]      : the number of steps each cell split takes, default=600'
	print '-n, --initial-width    [int]      : the initial number of cells in the PSM, default=10'
	print '-g, --granularity      [int]      : the number of time steps to jump between each value used, default=1'
	print '-s, --start-step       [int]      : the time step to start at, relative to the steps until growth, default=0'
	print '-e, --end-step         [int]      : the time step to end at, relative to the steps until growth, default=60000'
	print '-h, --help             [N/A]      : view usage information (i.e. this)'
	exit(0)

main()


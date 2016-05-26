/*
Stochastically ranked evolutionary strategy sampler for zebrafish segmentation
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
*/

/*
macros.hpp contains every constant and macro function used in the program.
*/

#ifndef MACROS_HPP
#define MACROS_HPP

// The number of implicit arguments sent to the simulation
#define NUM_IMPLICIT_SIM_ARGS 6

// Exit statuses
#define EXIT_SUCCESS			0
#define EXIT_MEMORY_ERROR		1
#define EXIT_FILE_READ_ERROR	2
#define EXIT_FILE_WRITE_ERROR	3
#define EXIT_FILE_REMOVE_ERROR	4
#define EXIT_PIPE_CREATE_ERROR	5
#define EXIT_PIPE_READ_ERROR	6
#define EXIT_PIPE_WRITE_ERROR	7
#define EXIT_FORK_ERROR			8
#define EXIT_EXEC_ERROR			9
#define EXIT_CHILD_ERROR		10
#define EXIT_INPUT_ERROR		11

// Macros for commonly used functions small enough to inject directly into the code
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define INT_STRLEN(x) (log10((x) > 0 ? (x) : 1) + 1)

#endif


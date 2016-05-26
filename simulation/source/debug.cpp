/*
Simulation for zebrafish segmentation
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
debug.cpp contains functions for debugging problems in the simulation. Programmers are encouraged to add commonly used debug statements, diagnoses, etc. to the file.
Do not add functions here that are needed for non-debugging purposes since the code should be compilable without this file.
*/

#include "debug.hpp" // Function declarations

#include "macros.hpp"

using namespace std;

/* is_nan returns whether or not a given double is NaN, i.e. not a number
	parameters:
		x: the double to check
	returns: true if x is NaN, false otherwise
	notes:
		x must be marked volatile to prevent the compiler from optimizing out 'x != x' to false without checking.
	todo:
*/
bool is_nan (volatile double x) {
	return x != x;
}

/* print_rs prints the base rates of the given rates structure
	parameters:
		rs:	the rates struct to print from
	returns: nothing
	notes:
		This function prints the base rates, not the active ones ultimately used.
	todo:
*/
void print_rs (rates& rs) {
	cerr << "rates:\n";
	for (int i = 0; i < NUM_RATES; i++) {
		cerr << "  " << i << ": " << rs.rates_base[i] << "\n";
	}
}

/* print_cl prints every concentration value of every cell at every time step
	parameters:
		cl:	the concentration levels struct to print from
	returns: nothing
	notes:
	todo:
*/
void print_cl (con_levels& cl) {
	for (int i = 0; i < cl.num_con_levels; i++) {
		print_con(cl, i);
	}
}

/* print_cl_at prints every concentration value of every cell at the given time step
	parameters:
		cl:	the concentration levels struct to print from
		timestep: the time step to print
	returns: nothing
	notes:
	todo:
*/
void print_cl_at (con_levels& cl, int timestep) {
	cerr << "timestep: " << timestep << ", concentrations:\n";
	for (int i = 0; i < cl.num_con_levels; i++) {
		cerr << "  " << i << ": " << cl.cons[i][timestep][0];
		for (int k = 1; k < cl.cells; k++) {
			cerr << "," << cl.cons[i][timestep][k];
		}
		cerr << "\n";
	}
}

/* print_cl_when_nonzero prints every concentration value of every cell at first time step a nonzero value appears
	parameters:
		cl:	the concentration levels struct to print from
	returns: nothing
	notes:
	todo:
*/
void print_cl_when_nonzero (con_levels& cl) {
	for (int i = 0; i < cl.num_con_levels; i++) {
		for (int j = 0; j < cl.time_steps; j++) {
			for (int k = 0; k < cl.cells; k++) {
				if (cl.cons[i][j][k] != 0) {
					print_cl_at(cl, j);
					return;
				}
			}
		}
	}
}

/* print_cl_when_same prints the time step and value for every time step for which every cell has the same value
	parameters:
		cl:	the concentration levels struct to print from
	returns: nothing
	notes:
		While this function has a similar name to print_cl_when_different, this function prints every time step that meets the criteria while the other prints only the first that does.
	todo:
*/
void print_cl_when_same (con_levels& cl) {
	for (int j = 0; j < cl.time_steps; j++) {
		double value = cl.cons[0][j][0];
		bool same = true;
		for (int i = 0; i < cl.num_con_levels; i++) {
			for (int k = 0; k < cl.cells; k++) {
				if (cl.cons[i][j][k] != value) {
					same = false;
					break;
				}
			}
			if (!same) {
				break;
			}
		}
		if (same) {
			cerr << "timestep: " << j << ", value: " << value << "\n";
		}
	}
}

/* print_cl_when_different prints the time step, cell index, and value for the first time step for which at least one cell has a different value than another
	parameters:
		cl:	the concentration levels struct to print from
	returns: nothing
	notes:
		While this function has a similar name to print_cl_when_same, this function prints only the first time step that meets the criteria while the other prints every one that does.
	todo:
*/
void print_cl_when_different (con_levels& cl) {
	for (int j = 0; j < cl.time_steps; j++) {
		bool different = false;
		for (int i = 0; i < cl.num_con_levels; i++) {		
			double value = cl.cons[i][j][0];
			for (int k = 0; k < cl.cells; k++) {
				if (i != BIRTH && cl.cons[i][j][k] != value && cl.cons[i][j][k] != 0) {
					different = true;
					cerr << "timestep: " << j << ", cell: " << k << ", value: " << i << "\n";
				}
			}
		}
		if (different) {
			return;
		}
	}
}

/* print_con prints the given concentration value of every cell at every time step
	parameters:
		cl:	the concentration levels struct to print from
		concentration: the index of the concentration value to print
	returns: nothing
	notes:
	todo:
*/
void print_con (con_levels& cl, int concentration) {
	cerr << "concentration: " << concentration << "\n";
	for (int j = 0; j < cl.time_steps; j++) {
		cerr << "  timestep: " << j << "\n    " << cl.cons[concentration][j][0];
		for (int k = 1; k < cl.cells; k++) {
			cerr << "," << cl.cons[concentration][j][k];
		}
		cerr << "\n";
	}
}

/* print_con_at prints the given concentration value of every cell at the given time step
	parameters:
		cl:	the concentration levels struct to print from
		concentration: the index of the concentration value to print
		timestep: the time step to print
	returns: nothing
	notes:
	todo:
*/
void print_con_at (con_levels& cl, int concentration, int timestep) {
	cerr << "concentration: " << concentration << ", timestep: " << timestep << "\n  " << cl.cons[concentration][timestep][0];
	for (int k = 1; k < cl.cells; k++) {
		cerr << "," << cl.cons[concentration][timestep][k];
	}
	cerr << "\n";
}

/* print_cl_nonzero_at prints every concentration value of every cell with a nonzero value at the given time step
	parameters:
		cl:	the concentration levels struct to print from
		timestep: the time step to print
	returns: nothing
	notes:
	todo:
*/
void print_cl_nonzero_at (con_levels& cl, int timestep) {
	cerr << "timestep: " << timestep << ", nonzero: ";
	bool first = true;
	for (int i = 0; i < cl.num_con_levels; i++) {
		for (int k = 0; k < cl.cells; k++) {
			if (cl.cons[i][timestep][k] != 0) {
				if (first) {
					first = false;
				} else {
					cerr << ",";
				}
				cerr << i;
				break;
			}
		}
	}
	cerr << "\n";
}

/* print_cl_at_for prints every concentration value of the given cell at the given time step
	parameters:
		cl:	the concentration levels struct to print from
		timestep: the time step to print
		cell: the cell to print
	returns: nothing
	notes:
	todo:
*/
void print_cl_at_for (con_levels& cl, int timestep, int cell) {
	cerr << "timestep: " << timestep << ", cell: " << cell << "\n";
	for (int i = 0; i < cl.num_con_levels; i++) {
		cerr << "  concentration " << i << ": " << cl.cons[i][timestep][cell] << "\n";
	}
}


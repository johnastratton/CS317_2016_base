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
io.hpp contains function declarations for io.cpp.
*/

#ifndef IO_HPP
#define IO_HPP

#include "structs.hpp"

using namespace std;

bool not_EOL(char);
void store_filename(char**, const char*);
void create_dir(char*);
void open_file(ofstream*, char*, bool);
void read_file(input_data*);
bool parse_param_line(double*, char*, int&);
void parse_ranges_file (pair <double, double>[], char*);
void print_passed(input_params&, ofstream*, rates&);
void print_concentrations(input_params&, sim_data&, con_levels&, mutant_data&, char*, int);
void print_cell_columns(input_params&, sim_data&, con_levels&, char*, int);
void print_osc_features(input_params&, ofstream*, mutant_data[], int, int);
void print_conditions (input_params&, ofstream*, mutant_data[], int);
void print_scores(input_params&, ofstream*, int, double[], double);
void close_if_open(ofstream*);
void read_pipe(double**&, input_params&);
void read_pipe_int(int, int*);
void read_pipe_set(int, double[]);
void write_pipe(double[], input_params&, sim_data&);
void write_pipe_double(int, double);

#endif


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
init.hpp contains function declarations for init.cpp.
*/

#ifndef INIT_HPP
#define INIT_HPP

#include "structs.hpp"

using namespace std;

char* copy_str(const char*);
int random_int(pair<int, int>);
double random_double(pair<double, double>);
double interpolate(double, int, int, double, double);
void init_terminal();
void free_terminal();
void accept_input_params(int, char**, input_params&);
bool option_set(const char*, const char*, const char*);
void ensure_nonempty(const char*, const char*);
void check_input_params(input_params&);
int generate_seed();
void init_seeds(input_params&, int, bool, bool);
void reset_seed(input_params&, sim_data& sd);
void init_verbosity(input_params&);
void read_sim_params(input_params&, input_data&, double**&, input_data&);
void read_perturb_params(input_params&, input_data&);
void read_gradients_params(input_params&, input_data&);
void fill_perturbations(rates&, char*);
void fill_gradients(rates&, char*);
void calc_max_delay_size(input_params&, sim_data&, rates&, double**);
void delete_file(ofstream*);
ofstream* create_passed_file(input_params&);
char** create_dirs(input_params&, sim_data&, mutant_data[]);
void delete_dirs(input_params&, char**);
ofstream* create_features_file(input_params&, mutant_data[]);
ofstream* create_conditions_file(input_params&, mutant_data[]);
ofstream* create_scores_file (input_params&, mutant_data[]);
mutant_data* create_mutant_data(sim_data&, input_params&);
void delete_mutant_data(mutant_data[]);
void delete_sets(double**, input_params&);
void copy_cl_to_mutant(sim_data&, con_levels&, mutant_data&);
void copy_mutant_to_cl(sim_data&, con_levels&, mutant_data&);
void reset_cout(input_params&);

#endif


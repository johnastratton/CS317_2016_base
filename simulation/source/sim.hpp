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
sim.hpp contains function declarations for sim.cpp.
*/

#ifndef SIM_HPP
#define SIM_HPP

#include "structs.hpp"

using namespace std;

void simulate_all_params(input_params&, rates&, sim_data&, double**, mutant_data[], ofstream*, ofstream*, char**, ofstream*, ofstream*);
bool determine_set_passed(sim_data&, int, double);
double simulate_param_set(int, input_params&, sim_data&, rates&, con_levels&, con_levels&, mutant_data[], ofstream*, ofstream*, char**, ofstream*, ofstream*);
int simulate_section(int, input_params&, sim_data&, rates&, con_levels&, con_levels&, mutant_data[], char**, double[]);
void determine_start_end(sim_data&);
void reset_mutant_scores(input_params&, mutant_data[]);
void mutant_sim_message(mutant_data&, int);
void store_original_rates (rates&, mutant_data&, double[]);
void knockout(rates& rs, mutant_data&, bool induction);
void revert_knockout(rates& rs, mutant_data&, double[]);
double simulate_mutant(int, input_params&, sim_data&, rates&, con_levels&, con_levels&, mutant_data&, features&, char*, double[2]);
bool model(sim_data&, rates&, con_levels&, con_levels&, mutant_data&, double[2]);
void calculate_delay_indices (sim_data&, con_levels&, int, int, int, double*[], int[], int[]);
int index_with_splits(sim_data&, con_levels&, int, int, int, double);
bool any_less_than_0(con_levels&, int);
bool concentrations_too_high(con_levels&, int, double);
void split(sim_data&, rates& rs, con_levels&, int, int);
void copy_records(sim_data& sd, con_levels& cl, int, int);
void update_rates(rates&, int);
void protein_synthesis(sim_data&, double**, con_levels&, st_context&, int[]);
void dim_int(di_args&, di_indices);
void con_protein_her(cp_args&, cph_indices);
void con_protein_delta(cp_args&, cpd_indices);
void dimer_proteins(sim_data&, double**, con_levels&, st_context&);
void con_dimer(cd_args&, int, int, cd_indices);
void mRNA_synthesis(sim_data&, double**, con_levels&, st_context&, int[], mutant_data&, bool, bool);
void calc_neighbors_1d(sim_data&, int[], int, int, int);
void calc_neighbors_2d(sim_data&);
double transcription(double**, con_levels&, int, int, double, double, double, int);
double transcription_mespa(double**, con_levels&, int, int, double, double, double, int);
double transcription_mespb(double**, con_levels&, int, int, double, double, double, int);
void perturb_rates_all(rates&);
void perturb_rates_column(sim_data&, rates&, int);
double random_perturbation(double);
void baby_to_cl (con_levels&, con_levels&, int, int);
int anterior_time(sim_data&, int);

#endif


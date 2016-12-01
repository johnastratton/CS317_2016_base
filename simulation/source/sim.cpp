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

#include "sim.hpp" // Function declarations

#include "debug.hpp"
#include "feats.hpp"
#include "init.hpp"
#include "io.hpp"

using namespace std;

extern terminal* term; // Declared in init.cpp

/* simulate_all_params simulates every parameter set after initialization is finished
	parameters:
		ip: the program's input parameters
		rs: the current simulation's rates
		sd: the current simulation's data
		sets: the array of parameter sets
		mds: the array of all mutant data
		file_passed: a pointer to the output file stream of the passed file
		file_scores: a pointer to the output file stream of the scores file
		dirnames_cons: the array of mutant directory paths
		file_features: a pointer to the output file stream of the features file
		file_conditions: a pointer to the output file stream of the conditions file
	returns: nothing
	notes:
	todo:
		TODO consolidate ofstream parameters.
*/
void simulate_all_params (input_params& ip, rates& rs, sim_data& sd, double** sets, mutant_data mds[], ofstream* file_passed, ofstream* file_scores, char** dirnames_cons, ofstream* file_features, ofstream* file_conditions) {
	// Initialize score data
	int sets_passed = 0;
	double score[ip.num_sets];

	// Initialize the concentration levels structs
	int max_cl_size = MAX(sd.steps_til_growth, sd.max_delay_size + sd.steps_total - sd.steps_til_growth) / sd.big_gran + 1;
	con_levels cl(NUM_CON_STORE, max_cl_size, sd.cells_total, sd.active_start); // Concentration levels for analysis and storage
	con_levels baby_cl(NUM_CON_LEVELS, sd.max_delay_size, sd.cells_total, sd.active_start); // Concentration levels for simulating (time in this cl is treated cyclically)

	// Simulate every parameter set
	for (int i = 0; i < ip.num_sets; i++) {
		memcpy(rs.rates_base, sets[i], sizeof(double) * NUM_RATES); // Copy the set's rates to the current simulation's rates
		score[i] = simulate_param_set(i, ip, sd, rs, cl, baby_cl, mds, file_passed, file_scores, dirnames_cons, file_features, file_conditions);
		sets_passed += determine_set_passed(sd, i, score[i]); // Calculate the maximum score and whether the set passed
	}

	// Pipe the scores if piping specified by the user
	if (ip.piping) {
		write_pipe(score, ip, sd);
	}

	cout << endl << term->blue << "Done: " << term->reset << sets_passed << "/" << ip.num_sets << " parameter sets passed all conditions" << endl;
}

/* simulate_param_set simulates the given parameter set with every specified mutant
	parameters:
		sd: the current simulation's data
		score: the set's received score
	returns: whether or not the set passed
	notes:
	todo:
*/
bool determine_set_passed (sim_data& sd, int set_num, double score) {
	cout << term->blue << "Done: " << term->reset << "set " << set_num << " scored ";
	double max_score = sd.no_growth ? sd.max_scores[SEC_POST] : sd.max_score_all;
	bool passed = score == max_score;
	if (passed) {
		cout << term->blue;
	} else {
		cout << term->red;
	}
	cout << score << " / " << max_score << term->reset << endl;
	return passed;
}

/* simulate_param_set simulates the given parameter set with every specified mutant
	parameters:
		set_num: the index of the parameter set to simulate
		ip: the program's input parameters
		sd: the current simulation's data
		rs: the current simulation's rates
		cl: the concentration levels used for analysis and storage
		baby_cl: the concentration levels used for simulating
		mds: the array of all mutant data
		file_passed: a pointer to the output file stream of the passed file
		file_scores: a pointer to the output file stream of the scores file
		dirnames_cons: the array of mutant directory paths
		file_features: a pointer to the output file stream of the features file
		file_conditions: a pointer to the output file stream of the conditions file
	returns: the cumulative score of every mutant
	notes:
	todo:
*/
double simulate_param_set (int set_num, input_params& ip, sim_data& sd, rates& rs, con_levels& cl, con_levels& baby_cl, mutant_data mds[], ofstream* file_passed, ofstream* file_scores, char** dirnames_cons, ofstream* file_features, ofstream* file_conditions) {
	// Prepare for the simulations
	cout << term->blue << "Simulating set " << term->reset << set_num << " . . ." << endl;
	int num_passed = 0;
	double scores[NUM_SECTIONS * NUM_MUTANTS] = {0};
	if (!ip.reset_seed) { // Reset the seed for each set if specified by the user
		init_seeds(ip, set_num, set_num > 0, true);
	}
	cl.reset(); // Reset the concentration levels for each set
	(*mds).feat.reset();

	// Simulate every mutant in the posterior before moving on to the anterior
	int end_section = SEC_ANT * !(sd.no_growth);
	for (int i = SEC_POST; i <= end_section; i++) {
		sd.section = i;
		num_passed += simulate_section(set_num, ip, sd, rs, cl, baby_cl, mds, dirnames_cons, scores);
	}

	// Calculate the total score
	double total_score = 0;
	for (int i = 0; i < NUM_SECTIONS * ip.num_active_mutants; i++) {
		total_score += scores[i];
	}

	// Print the mutant's results (if not short circuiting)
	if (total_score == sd.max_scores[SEC_POST] + sd.max_scores[SEC_WAVE] + sd.max_scores[SEC_ANT]) {
		print_passed(ip, file_passed, rs);
	}
	print_osc_features(ip, file_features, mds, set_num, num_passed);
	print_conditions(ip, file_conditions, mds, set_num);
	print_scores(ip, file_scores, set_num, scores, total_score);

	return total_score;
}

/* simulate_section simulates the given section with every specified mutant
	parameters:
		set_num: the index of the parameter set to simulate
		ip: the program's input parameters
		sd: the current simulation's data
		rs: the current simulation's rates
		cl: the concentration levels used for analysis and storage
		baby_cl: the concentration levels used for simulating
		mds: the array of all mutant data
		dirnames_cons: the array of mutant directory paths
		scores: the array of scores to populate with each mutant's results
	returns: the number of mutants that passed
	notes:
	todo:
*/
int simulate_section (int set_num, input_params& ip, sim_data& sd, rates& rs, con_levels& cl, con_levels& baby_cl, mutant_data mds[], char** dirnames_cons, double scores[]) {
	// Prepare for this section's simulations
	int num_passed = 0;
	double temp_rates[2]; // Array of knockout rates so knockouts can be quickly applied and reverted
	determine_start_end(sd);
	reset_mutant_scores(ip, mds);

	// Simulate each mutant
	for (int i = 0; i < ip.num_active_mutants; i++) {
		mutant_sim_message(mds[i], i);
		store_original_rates(rs, mds[i], temp_rates); // will be used to revert original rates after current mutant
		knockout (rs, mds[i], 0);
		double current_score = simulate_mutant(set_num, ip, sd, rs, cl, baby_cl, mds[i], mds[MUTANT_WILDTYPE].feat, dirnames_cons[i], temp_rates);
		scores[sd.section * ip.num_active_mutants + i] = current_score;
		baby_cl.reset();
		revert_knockout(rs, mds[i], temp_rates); // this should still happen at the end

		if (current_score == mds[i].max_cond_scores[sd.section]) { // If the mutant passed, increment the passed counter
			++num_passed;
		} else if (ip.short_circuit) { // Exit both loops if the mutant failed and short circuiting is active
			return num_passed;
		}
	}

	return num_passed;
}

/* determine_start_end determines the start and end points for the current simulation based on the current section
	parameters:
		sd: the current simulation's data
	returns: nothing
	notes:
	todo:
*/
void determine_start_end (sim_data& sd) {
	if (sd.section == SEC_POST) {
		sd.time_start = 1;
		sd.time_end = MIN(sd.steps_til_growth + 1, sd.steps_total);
	} else {
		sd.time_start = sd.max_delay_size;
		sd.time_end = sd.max_delay_size + sd.steps_total - sd.steps_til_growth;
	}
}

/* reset_mutant_scores resets each mutant's condition scores
	parameters:
		ip: the program's input parameters
		mds: the array of all mutant data
	returns: nothing
	notes:
	todo:
*/
void reset_mutant_scores (input_params& ip, mutant_data mds[]) {
	for (int i = 0; i < ip.num_active_mutants; i++) {
		for (int j = 0; j < NUM_SECTIONS; j++) {
			for (int k = 0; k < 1 + MAX_CONDS_ANY; k++) {
				mds[i].conds_passed[j][k] = 0;
			}
			mds[i].secs_passed[j] = false;

		}
	}
	mds[MUTANT_WILDTYPE].conds_passed[SEC_WAVE][0]=1;
	mds[MUTANT_WILDTYPE].conds_passed[SEC_WAVE][1]=1;
	mds[MUTANT_WILDTYPE].conds_passed[SEC_WAVE][2]=1;
	mds[MUTANT_WILDTYPE].conds_passed[SEC_WAVE][3]=1;
	mds[MUTANT_WILDTYPE].conds_passed[SEC_ANT][6]=1;
	mds[MUTANT_WILDTYPE].conds_passed[SEC_ANT][7]=1;
}

/* mutant_sim_message prints a message indicating that the given mutant is being simulated in the given section
	parameters:
		md: the mutant being simulated
		section: the section being simulated
	returns: nothing
	notes:
	todo:
*/
inline void mutant_sim_message (mutant_data& md, int section) {
	term->verbose() << term->blue << "  Simulating " << term->reset << md.print_name << " ";
	if (section == SEC_POST) {
		term->verbose() << "before";
	} else {
		term->verbose() << "with";
	}
	term->verbose() << " growth . . ." << endl;
}

/* store_original_rates stores the rates which might be knocked out later in the simulation
	parameters:
		rs: the current simulation's rates
		md: the mutant being simulated
		orig_rates: the array to store the original rates
	returns: nothing
	notes:
	todo:
*/
inline void store_original_rates (rates& rs, mutant_data& md, double orig_rates[]) {
	for (int i = 0; i < md.num_knockouts; i++) {
		orig_rates[i] = rs.rates_base[md.knockouts[i]];
	}
}

/* knockout knocks out the given mutant's rates and stores their originals in the given array
	parameters:
		rs: the current simulation's rates
		md: the mutant being simulated
		orig_rates: the array to store the original rates
	returns: nothing
	notes:
	todo:

	151221: For DAPT Mutant, knockout after induction
*/
inline void knockout (rates& rs, mutant_data& md, bool induction) {
	for (int i = 0; i < md.num_knockouts; i++) {
		if (!(md.index==MUTANT_DAPT && induction == 0)){
			rs.rates_base[md.knockouts[i]] = 0;
		}
	}
}

/* revert_knockout reverts a knockout for the given mutant using the given original rates
	parameters:
		rs: the current simulation's rates
		md: the mutant being simulated
		orig_rates: the array of original rates
	returns: nothing
	notes:
	todo:
*/
inline void revert_knockout (rates& rs, mutant_data& md, double orig_rates[]) {
	for (int i = 0; i < md.num_knockouts; i++) {
		rs.rates_base[md.knockouts[i]] = orig_rates[i];
	}
}

/* simulate_mutant simulates the given parameter set with the given mutant
	parameters:
		set_num: the index of the parameter set to simulate
		ip: the program's input parameters
		sd: the current simulation's data
		rs: the current simulation's rates
		cl: the concentration levels used for analysis and storage
		baby_cl: the concentration levels used for simulating
		md: the mutant to simulate
		wtfeat: the oscillation features the wild type produced (or will if the mutant is the wild type)
		dirname_cons: the directory path of the mutant
	returns: the score of the mutant
	notes:
	todo:
		TODO Break up this enormous function.
*/
double simulate_mutant (int set_num, input_params& ip, sim_data& sd, rates& rs, con_levels& cl, con_levels& baby_cl, mutant_data& md, features& wtfeat, char* dirname_cons, double temp_rates[2]) {
	reset_seed(ip, sd); // Reset the seed for each mutant
	baby_cl.reset(); // Reset the concentrations levels used for simulating
	perturb_rates_all(rs); // Perturb the rates of all starting cells

	// Initialize active record data and neighbor calculations
	sd.initialize_active_data();
	if (sd.height > 1) {
		calc_neighbors_2d(sd);
	}
	cl.active_start_record[0] = sd.active_start;
	baby_cl.active_start_record[0] = sd.active_start;

	// Copy the posterior results if this is an anterior simulation
	if (sd.section == SEC_ANT) {
		copy_mutant_to_cl(sd, baby_cl, md);
	}

	// Simulate the mutant
	bool passed = model(sd, rs, cl, baby_cl, md, temp_rates);

	// Analyze the simulation's oscillation features
	term->verbose() << term->blue << "    Analyzing " << term->reset << "oscillation features . . . ";
	double score = 0;
	if (sd.section == SEC_POST) { // Posterior analysis
		osc_features_post(sd, ip, cl, md.feat, wtfeat, dirname_cons, sd.time_start / sd.big_gran, sd.time_end / sd.big_gran, set_num);
		term->verbose() << term->blue << "Done" << endl;
	} else { // Anterior analysis
		if (ip.ant_features) {
			term->verbose() << endl;
		}
		osc_features_ant(sd, ip, wtfeat, dirname_cons, cl, md, 0, sd.height, 0, 5, set_num);
		if (ip.ant_features) {
			term->verbose() << term->blue << "    Done " << term->reset << "analyzing oscillation features" << endl;
		} else {
			term->verbose() << term->blue << "Done" << endl;
		}
	}

	// Copy and print the appropriate data
	print_concentrations(ip, sd, cl, md, dirname_cons, set_num);
	if (sd.section == SEC_ANT) { // Print concentrations of columns of cells from posterior to anterior to a file if the user specified it
		print_cell_columns(ip, sd, cl, dirname_cons, set_num);
	}
	if (!sd.no_growth && sd.section == SEC_POST && !ip.short_circuit) { // Copy the concentration levels to the mutant data (if not short circuiting)
		copy_cl_to_mutant(sd, baby_cl, md);
	}

	// Print how the mutant performed and finish book-keeping
	term->verbose() << "  " << term->blue << "Done: " << term->reset << md.print_name << " scored ";
	if (passed) {
		md.secs_passed[sd.section] = true; // Mark that this mutant has passed this simulation
		score += md.tests[sd.section](md, wtfeat);
		double max_score = md.max_cond_scores[sd.section];
		if (sd.section == SEC_ANT && (md.index == MUTANT_WILDTYPE)) { // The max score has to be adjusted for mutants which have a wave section
			max_score += md.max_cond_scores[SEC_WAVE];
			int time_full = anterior_time(sd, sd.steps_til_growth + (sd.width_total - sd.width_initial - 1) * sd.steps_split);
			int wave_score=0;
			for (int time = time_full; time < sd.time_end; time += (sd.time_end - 1 - time_full) / 4) {
				if (md.index == MUTANT_WILDTYPE){
					wave_score = wave_testing(sd, cl, md, time, CMH1, sd.active_start);
				} else {
					wave_score = wave_testing_her1(sd, cl, md, time, sd.active_start);  // unused 151221
				}
			}
			score += wave_score;
		}
		if (score == max_score) {
			// Copy the concentration levels to the mutant data if this is a posterior simulation (if short circuiting)
			if (!sd.no_growth && sd.section == SEC_POST && ip.short_circuit) {
				copy_cl_to_mutant(sd, baby_cl, md);
			}
			term->verbose() << term->blue;
		} else {
			term->verbose() << term->red;
		}
		term->verbose() << score << " / " << max_score;
	} else {
		term->verbose() << term->red << "failed to complete the simulation";
	}
	term->verbose() << term->reset << endl;
	return score;
}

/* model performs the biological functions of a simulation
	parameters:
		sd: the current simulation's data
		rs: the current simulation's rates
		cl: the concentration levels used for analysis and storage
		baby_cl: the concentration levels used for simulating
		md: the mutant to simulate
	returns: the score of the mutant
	notes:
	todo:
*/
bool model (sim_data& sd, rates& rs, con_levels& cl, con_levels& baby_cl, mutant_data& md, double temp_rates[2]) {
	int steps_elapsed = sd.steps_split; // Used to determine when to split a column of cells
	update_rates(rs, sd.active_start); // Update the active rates based on the base rates, perturbations, and gradients

	// Iterate through each time step
	int j; // Absolute time used by cl
	int baby_j; // Cyclical time used by baby_cl
	bool past_induction = false; // Whether we've passed the point of induction of knockouts or overexpression
	bool past_recovery = false; // Whether we've recovered from the knockouts or overexpression
	for (j = sd.time_start, baby_j = 0; j < sd.time_end; j++, baby_j = WRAP(baby_j + 1, sd.max_delay_size)) {

		if (!past_induction && !past_recovery && (j  > anterior_time(sd,md.induction))) {
			knockout(rs, md, 1); //knock down rates after the induction point
			perturb_rates_all(rs); //This is used for knockout the rate in the existing cells, may need modification
			past_induction = true;
		}
		if (past_induction && (j + sd.steps_til_growth > md.recovery)) {
			revert_knockout(rs, md, temp_rates);
			past_recovery = true;
		}

		int time_prev = WRAP(baby_j - 1, sd.max_delay_size); // Time is cyclical, so time_prev may not be baby_j - 1
		copy_records(sd, baby_cl, baby_j, time_prev); // Copy each cell's birth and parent so the records are accessible at every time step

		// Iterate through each extant cell
		for (int k = 0; k < sd.cells_total; k++) {
			if (sd.width_current == sd.width_total || k % sd.width_total <= sd.active_start) { // Compute only existing (i.e. already grown) cells
				// Calculate the cell indices at the start of each mRNA and protein's delay
				int old_cells_mrna[NUM_INDICES];
				int old_cells_protein[NUM_INDICES];
				calculate_delay_indices(sd, baby_cl, baby_j, j, k, rs.rates_active, old_cells_mrna, old_cells_protein);

				// Perform biological calculations
				st_context stc(time_prev, baby_j, k);
				protein_synthesis(sd, rs.rates_active, baby_cl, stc, old_cells_protein);
				dimer_proteins(sd, rs.rates_active, baby_cl, stc);
				mRNA_synthesis(sd, rs.rates_active, baby_cl, stc, old_cells_mrna, md, past_induction, past_recovery);
			}
		}

		// Check to make sure the numbers are still valid
		if (any_less_than_0(baby_cl, baby_j) || concentrations_too_high(baby_cl, baby_j, sd.max_con_thresh)) {
			return false;
		}

		// Split cells periodically in anterior simulations
		if (sd.section == SEC_ANT && (steps_elapsed % sd.steps_split) == 0) {
			split(sd, rs, baby_cl, baby_j, j);
			update_rates(rs, sd.active_start);
			steps_elapsed = 0;
		}

		// Update the active record data and split counter
		steps_elapsed++;
		baby_cl.active_start_record[baby_j] = sd.active_start;
		baby_cl.active_end_record[baby_j] = sd.active_end;

		// Copy from the simulating cl to the analysis cl
		if (j % sd.big_gran == 0) {
			baby_to_cl(baby_cl, cl, baby_j, j / sd.big_gran);
		}
	}

	// Copy the last time step from the simulating cl to the analysis cl and mark where the simulating cl left off time-wise
	baby_to_cl(baby_cl, cl, WRAP(baby_j - 1, sd.max_delay_size), (j - 1) / sd.big_gran);
	sd.time_baby = baby_j;

	return true;
}

/* calculate_delay_indices calculates where the given cell was at the start of all mRNA and protein delays
	parameters:
		sd: the current simulation's data
		cl: the concentration levels used for analysis and storage
		baby_time: the cyclical time used by baby_cl, the cl for simulating
		time: the absolute time used by cl, the cl for analysis
		cell_index: the current cell index
		active_rates: the active rates
		old_cells_mrna: the indices of the cell's old positions for mRNA delays
		old_cells_protein: the indices of the cell's old positions for protein delays
	returns: nothing
	notes:
	todo:
*/
void calculate_delay_indices (sim_data& sd, con_levels& cl, int baby_time, int time, int cell_index, double* active_rates[], int old_cells_mrna[], int old_cells_protein[]) {
	if (sd.section == SEC_POST) { // Cells in posterior simulations do not split so the indices never change
		for (int l = 0; l < NUM_INDICES; l++) {
			old_cells_mrna[IMH1 + l] = cell_index;
			old_cells_protein[IPH1 + l] = cell_index;
		}
	} else { // Cells in anterior simulations split so with long enough delays the cell must look to its parent for values, causing its effective index to change over time
		for (int l = 0; l < NUM_INDICES; l++) {
			old_cells_mrna[IMH1 + l] = index_with_splits(sd, cl, baby_time, time, cell_index, active_rates[RDELAYMH1 + l][cell_index]);
			old_cells_protein[IPH1 + l] = index_with_splits(sd, cl, baby_time, time, cell_index, active_rates[RDELAYPH1 + l][cell_index]);
		}
	}
}

/* index_with_splits calculates where the given cell was at the start of the given delay
	parameters:
		sd: the current simulation's data
		cl: the concentration levels used for analysis and storage
		baby_time: the cyclical time used by baby_cl, the cl for simulating
		time: the absolute time used by cl, the cl for analysis
		cell_index: the current cell index
		delay: the amount of time (in minutes) the delay takes
	returns: the index of the cell at the start of the delay
	notes:
	todo:
*/
inline int index_with_splits (sim_data& sd, con_levels& cl, int baby_time, int time, int cell_index, double delay) {
	int delay_steps = delay / sd.step_size;
	if (time - delay_steps < 0 || time - delay_steps >= cl.cons[BIRTH][baby_time][cell_index]) { // If the delay is longer than the simulation has run or shorter than the cell's age then return the cell's index
		return cell_index;
	} else { // If the delay is not longer than the simulation has run but longer than the cell's age then move to the cell's parent and check again
		return index_with_splits(sd, cl, baby_time, time, cl.cons[PARENT][baby_time][cell_index], delay);
	}
}

/* any_less_than_0 checks if any of the concentrations in the given range at the given time are less than 0
	parameters:
		cl: the concentration levels
		time: the time step
	returns: true if any concentrations are less than 0, false otherwise
	notes:
		This function checks only the first cell's concentrations for the sake of speed
	todo:
*/
inline bool any_less_than_0 (con_levels& cl, int time) {
	for (int i = MIN_CON_LEVEL; i <= MAX_CON_LEVEL; i++) {
		if (cl.cons[i][time][0] < 0) { // This checks only the first cell
			return true;
		}
	}
	return false;
}

/* concentrations_too_high checks if any concentrations are higher than the given maximum threshold
	parameters:
		cl: the concentration levels used for analysis and storage
		time: the time step
		max_con_thresh: the maximum concentration threshold
	returns: true if any concentrations are too high, false otherwise
	notes:
		This function checks only the first cell's concentrations for the sake of speed
	todo:
*/
inline bool concentrations_too_high (con_levels& cl, int time, double max_con_thresh) {
	if (max_con_thresh != INFINITY) {
		for (int i = MIN_CON_LEVEL; i <= MAX_CON_LEVEL; i++) {
			if (cl.cons[i][time][0] > max_con_thresh) { // This checks only the first cell
				return true;
			}
		}
	}
	return false;
}

/* split splits the posterior-most column of cells
	parameters:
		sd: the current simulation's data
		rs: the current simulation's rates
		cl: the concentration levels used for simulating
		baby_time: the cyclical time used by baby_cl, the cl for simulating
		time: the absolute time used by cl, the cl for analysis
	returns: nothing
	notes:
	todo:
*/
void split (sim_data& sd, rates& rs, con_levels& cl, int baby_time, int time) {
	// Calculate the next active start and current width
	int next_active_start = (sd.active_start + 1) % sd.width_total;
	sd.width_current = MIN(sd.width_current + 1, sd.width_total);

	// Determine which child corresponds to which parent
	int parents[sd.height];
	for (int i = 0; i < sd.height; i++) {
		parents[i] = -1;
		int index;
		bool dup;
		do { // Ensure each parent produces exactly one child
			dup = false;
			index = random_int(pair<int, int>(0, sd.height - 1));
			for (int j = 0; j < i; j++) {
				if (index == parents[j]) {
					dup = true;
					break;
				}
			}
		} while (dup);
		parents[i] = index;
	}

	// Transfer each new cell's parent concentration levels to the new cell
	for (int i = 0; i < NUM_CON_LEVELS; i++) {
		for (int k = 0; k < sd.height; k++) {
			cl.cons[i][baby_time][next_active_start + k * sd.width_total] = cl.cons[i][baby_time][sd.active_start + parents[k] * sd.width_total];
		}
	}

	// Set each new cell's birth to the current time and store its assigned parent
	for (int k = 0; k < sd.height; k++) {
		cl.cons[BIRTH][baby_time][next_active_start + k * sd.width_total] = time;
		cl.cons[PARENT][baby_time][next_active_start + k * sd.width_total] = sd.active_start + parents[k] * sd.width_total;
	}

	// Perturb the new cells and update the active record data
	perturb_rates_column(sd, rs, next_active_start);
	sd.active_start = next_active_start;
	sd.active_end = WRAP(sd.active_start - sd.width_current + 1, sd.width_total);
}

/* copy_records copies each cell's birth and parent to the given time step
	parameters:
		sd: the current simulation's data
		cl: the concentration levels used for analysis and storage
		time: the time step
		time_prev: the previous time step
	returns: nothing
	notes:
	todo:
*/
inline void copy_records (sim_data& sd, con_levels& cl, int time, int time_prev) {
	for (int k = 0; k < sd.cells_total; k++) {
		cl.cons[BIRTH][time][k] = cl.cons[BIRTH][time_prev][k];
		cl.cons[PARENT][time][k] = cl.cons[PARENT][time_prev][k];
	}
}

/* update_rates updates the rates_active array in the given rates struct to account for perturbations and gradients
	parameters:
		rs: the current simulation's rates
		active_start: the column at the start of the posterior
	returns: nothing
	notes:
	todo:
*/
void update_rates (rates& rs, int active_start) {
	if (rs.using_gradients) { // If at least one rate has a gradient
		for (int i = 0; i < NUM_RATES; i++) {
			if (rs.has_gradient[i]) { // If this rate has a gradient
				// Iterate through every cell
				for (int k = 0; k < rs.cells; k++) {
					// Calculate the cell's index relative to the active start
					int col = k % rs.width;
					int gradient_index;
					if (col <= active_start) {
						gradient_index = active_start - col;
					} else {
						gradient_index = active_start + rs.width - col;
					}

					// Set the cell's active rate to its perturbed rate modified by its position's gradient factor
					rs.rates_active[i][k] = rs.rates_cell[i][k] * rs.factors_gradient[i][gradient_index];
				}
			} else { // If this rate does not have a gradient then set every cell's active rate to its perturbed rate
				for (int k = 0; k < rs.cells; k++) {
					rs.rates_active[i][k] = rs.rates_cell[i][k];
				}
			}
		}
	} else { // If no rates have gradients then set every cell's active rate to its perturbed rate
		for (int i = 0; i < NUM_RATES; i++) {
			for (int k = 0; k < rs.cells; k++) {
				rs.rates_active[i][k] = rs.rates_cell[i][k];
			}
		}
	}
}

/* protein_synthesis calculates the concentrations of every protein for a given cell
	parameters:
		sd: the current simulation's data
		rs: the active rates
		cl: the concentration levels for simulating
		stc: the spatiotemporal context, i.e. cell and time steps
		old_cells_protein: an array of the cell's indices at the start of each protein's delay
	returns: nothing
	notes:
		Every dimerizing gene must be added to every other dimerizing gene's section in this function.
	todo:

	151221: Added prtein synthesis for mespa and mespb
*/
void protein_synthesis (sim_data& sd, double** rs, con_levels& cl, st_context& stc, int old_cells_protein[]) {
	double dimer_effects[NUM_HER_INDICES] = {0}; // Heterodimer calculations
	di_args dia(rs, cl, stc, dimer_effects); // WRAPper for repeatedly used structs
	cp_args cpa(sd, rs, cl, stc, old_cells_protein, dimer_effects); // WRAPper for repeatedly used indices

	/// Dimerizing genes

	// Her1
	dim_int(dia, di_indices(CPH1, CPH7,  CPH1H7,  RDAH1H7,  RDDIH1H7,  IH1));
	/*if (sd.section == SEC_ANT) {
		dim_int(dia, di_indices(CPH1, CPMESPA, CPH1MESPA, RDAH1MESPA, RDDIH1MESPA, IH1));
		dim_int(dia, di_indices(CPH1, CPMESPB, CPH1MESPB, RDAH1MESPB, RDDIH1MESPB, IH1));
	}*/
	dim_int(dia, di_indices(CPH1, CPH13, CPH1H13, RDAH1H13, RDDIH1H13, IH1));
	con_protein_her(cpa, cph_indices(CMH1, CPH1, CPH1H1, RPSH1, RPDH1, RDAH1H1, RDDIH1H1, RDELAYPH1, IH1, IPH1));

	// Her7
	dim_int(dia, di_indices(CPH7, CPH1,  CPH1H7,  RDAH1H7,  RDDIH1H7,  IH7));
	/*if (sd.section == SEC_ANT) {
		dim_int(dia, di_indices(CPH7, CPMESPA, CPH7MESPA, RDAH7MESPA, RDDIH7MESPA, IH7));
		dim_int(dia, di_indices(CPH7, CPMESPB, CPH7MESPB, RDAH7MESPB, RDDIH7MESPB, IH7));
	}*/
	dim_int(dia, di_indices(CPH7, CPH13, CPH7H13, RDAH7H13, RDDIH7H13, IH7));
	con_protein_her(cpa, cph_indices(CMH7, CPH7, CPH7H7, RPSH7, RPDH7, RDAH7H7, RDDIH7H7, RDELAYPH7, IH7, IPH7));

	if (sd.section == SEC_ANT) {
		// MespA
		//dim_int(dia, di_indices(CPMESPA, CPH1,  CPH1MESPA,  RDAH1MESPA,  RDDIH1MESPA,  IMESPA));
		//dim_int(dia, di_indices(CPMESPA, CPH7,  CPH7MESPA,  RDAH7MESPA,  RDDIH7MESPA,  IMESPA));
		dim_int(dia, di_indices(CPMESPA, CPMESPB, CPMESPAMESPB, RDAMESPAMESPB, RDDIMESPAMESPB, IMESPA));
		//dim_int(dia, di_indices(CPMESPA, CPH13, CPMESPAH13, RDAMESPAH13, RDDIMESPAH13, IMESPA));
		con_protein_her(cpa, cph_indices(CMMESPA, CPMESPA, CPMESPAMESPA, RPSMESPA, RPDMESPA, RDAMESPAMESPA, RDDIMESPAMESPA, RDELAYPMESPA, IMESPA, IPMESPA));

		// MespB
		//dim_int(dia, di_indices(CPMESPB, CPH1,  CPH1MESPB,  RDAH1MESPB,  RDDIH1MESPB,  IMESPB));
		//dim_int(dia, di_indices(CPMESPB, CPH7,  CPH7MESPB,  RDAH7MESPB,  RDDIH7MESPB,  IMESPB));
		dim_int(dia, di_indices(CPMESPB, CPMESPA, CPMESPAMESPB, RDAMESPAMESPB, RDDIMESPAMESPB, IMESPB));
		//dim_int(dia, di_indices(CPMESPB, CPH13, CPMESPBH13, RDAMESPBH13, RDDIMESPBH13, IMESPB));
		con_protein_her(cpa, cph_indices(CMMESPB, CPMESPB, CPMESPBMESPB, RPSMESPB, RPDMESPB, RDAMESPBMESPB, RDDIMESPBMESPB, RDELAYPMESPB, IMESPB, IPMESPB));
	}

	// Her13
	dim_int(dia, di_indices(CPH13, CPH1,  CPH1H13,  RDAH1H13,  RDDIH1H13,  IH13));
	dim_int(dia, di_indices(CPH13, CPH7,  CPH7H13,  RDAH7H13,  RDDIH7H13,  IH13));
	/*if (sd.section == SEC_ANT) {
		dim_int(dia, di_indices(CPH13, CPMESPA, CPMESPAH13, RDAMESPAH13, RDDIMESPAH13, IH13));
		dim_int(dia, di_indices(CPH13, CPMESPB, CPMESPBH13, RDAMESPBH13, RDDIMESPBH13, IH13));
	}*/
	con_protein_her(cpa, cph_indices(CMH13, CPH13, CPH13H13, RPSH13, RPDH13, RDAH13H13, RDDIH13H13, RDELAYPH13, IH13, IPH13));

	/// Nondimerizing genes

	// Delta
	con_protein_delta(cpa, cpd_indices(CMDELTA, CPDELTA, RPSDELTA, RPDDELTA, RDELAYPDELTA, IPDELTA));
}

/* dim_int calculates the dimer interactions for a given protein (a step in protein_synthesis)
	parameters:
		a: a struct containing the arguments needed
		dii: a struct containing the indices needed
	returns: nothing
	notes:
	todo:
*/
inline void dim_int (di_args& a, di_indices dii) {
	double** r = a.rs;
	double*** c = a.cl.cons;
	int tp = a.stc.time_prev;
	int cell = a.stc.cell;

	// The part of the given Her protein concentration's differential equation that accounts for heterodimers
	a.dimer_effects[dii.dimer_effect] =
		a.dimer_effects[dii.dimer_effect]
		- r[dii.rate_association][cell] * c[dii.con_protein_self][tp][cell] * c[dii.con_protein_other][tp][cell]
		+ r[dii.rate_dissociation][cell] * c[dii.con_dimer][tp][cell];
}

/* con_protein_her calculates the protein concentration of the given Her gene
	parameters:
		a: a struct containing the arguments needed
		dii: a struct containing the indices needed
	returns: nothing
	notes:
	todo:
*/
inline void con_protein_her (cp_args& a, cph_indices i) {
	double** r = a.rs;
	double*** c = a.cl.cons;
	int cell = a.stc.cell;
	int delay_steps = r[i.delay_protein][cell] / a.sd.step_size;
	int tc = a.stc.time_cur;
	int tp = a.stc.time_prev;
	int td = WRAP(tc - delay_steps, a.sd.max_delay_size);

	// The part of the given Her protein concentration's differential equation that accounts for everything but heterodimers, whose influence is calculated in dim_int
	c[i.con_protein][tc][cell] =
		c[i.con_protein][tp][cell]
		+ a.sd.step_size * (r[i.rate_synthesis][cell] * c[i.con_mrna][td][a.old_cells[i.old_cell]]
		- r[i.rate_degradation][cell] * c[i.con_protein][tp][cell]
		- 2 * r[i.rate_association][cell] * SQUARE(c[i.con_protein][tp][cell])
		+ 2 * r[i.rate_dissociation][cell] * c[i.con_dimer][tp][cell]
		+ a.dimer_effects[i.dimer_effect]);
}

/* con_protein_delta calculates the protein concentration of the given Delta gene
	parameters:
		a: a struct containing the arguments needed
		dii: a struct containing the indices needed
	returns: nothing
	notes:
	todo:
*/
inline void con_protein_delta (cp_args& a, cpd_indices i) {
	double** r = a.rs;
	double*** c = a.cl.cons;
	int cell = a.stc.cell;
	int delay_steps = r[i.delay_protein][cell] / a.sd.step_size;
	int tc = a.stc.time_cur;
	int tp = a.stc.time_prev;
	int td = WRAP(tc - delay_steps, a.sd.max_delay_size);

	// The Delta protein concentration's differential equation (no dimerization occurs)
	c[i.con_protein][tc][cell] =
		c[i.con_protein][tp][cell]
		+ a.sd.step_size * (r[i.rate_synthesis][cell] * c[i.con_mrna][td][a.old_cells[i.old_cell]]
		- r[i.rate_degradation][cell] * c[i.con_protein][tp][cell]);
}

/* dimer_proteins calculates the concentrations of every dimer for a given cell
	parameters:
		sd: the current simulation's data
		rs: the active rates
		cl: the concentration levels for simulating
		stc: the spatiotemporal context, i.e. cell and time steps
	returns: nothing
	notes:
	todo:

	151221: added dimerization for mespamespa, mespamespb, mespbmespb
*/
void dimer_proteins (sim_data& sd, double** rs, con_levels& cl, st_context& stc) {
	cd_args cda(sd, rs, cl, stc); // WRAPper for repeatedly used structs

	for (int i = CPH1H1,       j = 0;   i <= CPH1H13;   i++, j++) {
		/*while (sd.section == SEC_POST && CPH1MESPA <= i && i <= CPH1MESPB) {
			i++;
			j++;
		}*/
		con_dimer(cda, i, j, cd_indices(CPH1, RDAH1H1, RDDIH1H1, RDDGH1H1));
	}
	for (int i = CPH7H7,       j = 0;   i <= CPH7H13;   i++, j++) {
		/*while (sd.section == SEC_POST && CPH7MESPA <= i && i <= CPH7MESPB) {
			i++;
			j++;
		}*/
		con_dimer(cda, i, j, cd_indices(CPH7, RDAH7H7, RDDIH7H7, RDDGH7H7));
	}
	if (sd.section == SEC_ANT) {
		for (int i = CPMESPAMESPA, j = 0;   i <= CPMESPAMESPB; i++, j++) {
			con_dimer(cda, i, j, cd_indices(CPMESPA, RDAMESPAMESPA, RDDIMESPAMESPA, RDDGMESPAMESPA));
		}
		//for (int i = CPMESPBMESPB, j = 0;   i <= CPMESPBH13;  i++, j++) {
		con_dimer(cda, CPMESPBMESPB, 0, cd_indices(CPMESPB, RDAMESPBMESPB, RDDIMESPBMESPB, RDDGMESPBMESPB));
		//}
	}

	con_dimer(cda, CPH13H13, 0, cd_indices(CPH13, RDAH13H13, RDDIH13H13, RDDGH13H13));
}

/* con_dimer calculates the given dimer's concentration
	parameters:
		a: a struct containing the arguments needed
		con: the index of the dimer
		offset: an index offset for reusability purposes
		i: a struct containing the indices needed
	returns: nothing
	notes:
	todo:
		TODO consolidate these parameters

	151221: pay attention to the index for mesp genes
*/
inline void con_dimer (cd_args& a, int con, int offset, cd_indices i) {
	double** r = a.rs;
	double*** c = a.cl.cons;
	int tc = a.stc.time_cur;
	int tp = a.stc.time_prev;
	int cell = a.stc.cell;
	int con_offset=offset;
	if (i.con_protein == CPH1 && offset == 2){
		con_offset=4;
	}
	if (i.con_protein == CPH7 && offset == 1){
		con_offset=3;
	}
	// The given dimer concentration's differential equation
	c[con][tc][cell] =
		c[con][tp][cell]
		+ a.sd.step_size * (r[i.rate_association + offset][cell] * c[i.con_protein][tp][cell] * c[i.con_protein + con_offset][tp][cell]
			- r[i.rate_dissociation + offset][cell] * c[con][tp][cell]
			- r[i.rate_degradation + offset][cell] * c[con][tp][cell]);
}

/* mRNA_synthesis calculates the concentrations of every mRNA for a given cell
	parameters:
		sd: the current simulation's data
		rs: the active rates
		cl: the concentration levels for simulating
		stc: the spatiotemporal context, i.e. cell and time steps
		old_cells_mrna: an array of the cell's indices at the start of each mRNA's delay
		md: the currently simulating mutant's data
	returns: nothing
	notes:
	todo:

	151221: Added mRNA transcription for meps genes, pay attention to index of mesp genes
*/
void mRNA_synthesis (sim_data& sd, double** rs, con_levels& cl, st_context& stc, int old_cells_mrna[], mutant_data& md, bool past_induction, bool past_recovery) {
	// Translate delays from minutes to time steps
	int delays[NUM_INDICES];
	for (int j = 0; j < NUM_INDICES; j++) {
		delays[j] = rs[RDELAYMH1 + j][stc.cell] / sd.step_size;
	}

	// Calculate the influence of the given cell's neighbors (via Delta-Notch signaling)
	double avg_delays[NUM_DD_INDICES]; // Averaged delays for each mRNA concentration caused by the given cell's neighbors' Delta protein concentrations
	if (sd.height == 1) { // For 2-cell and 1D simulations
		if (sd.width_current > 2) { // For 1D simulations
			// Each cell has 2 neighbors so calculate where they and the active start and end were at the start of each mRNA concentration's delay
			int neighbors[NUM_DD_INDICES][NEIGHBORS_1D];
			for (int j = 0; j < NUM_INDICES; j++) {
				int old_time = WRAP(stc.time_cur - delays[j], sd.max_delay_size);
				int old_active_start = cl.active_start_record[old_time];
				int old_active_end = cl.active_end_record[old_time];
				calc_neighbors_1d(sd, neighbors[IMH1 + j], old_cells_mrna[IMH1 + j], old_active_start, old_active_end);
			}

			// For each mRNA concentration, average the give cell's neighbors' Delta protein concentrations
			for (int j = 0; j < NUM_DD_INDICES; j++) {
				int* cells = neighbors[IMH1 + j];
				int cell = old_cells_mrna[IMH1 + j];
				int time = WRAP(stc.time_cur - delays[j], sd.max_delay_size);

				if (cell % sd.width_total == cl.active_start_record[time]) {
					avg_delays[IMH1 + j] = cl.cons[CPDELTA][time][cells[0]];
				} else if (cell % sd.width_total == cl.active_end_record[time]) {
					avg_delays[IMH1 + j] = cl.cons[CPDELTA][time][cells[1]];
				} else {
					avg_delays[IMH1 + j] = (cl.cons[CPDELTA][time][cells[0]] + cl.cons[CPDELTA][time][cells[1]]) / 2;
				}
			}
		} else { // For 2-cell simulations
			// Both cells have one neighbor each so no averaging is required
			for (int j = 0; j < NUM_DD_INDICES; j++) {
				avg_delays[IMH1 + j] = cl.cons[CPDELTA][WRAP(stc.time_cur - delays[j], sd.max_delay_size)][1 - old_cells_mrna[IMH1 + j]];
			}
		}
	} else { // For 2D simulations
		// Each cell has 6 neighbors so calculate where they and the active start and end were at the start of each mRNA concentration's delay
		int neighbors[NUM_DD_INDICES][NEIGHBORS_2D];
		for (int j = 0; j < NUM_DD_INDICES; j++) {
			// 2D neighbors are precalculated and simply copied from the structure as needed
			int cell = old_cells_mrna[IMH1 + j];
			memcpy(neighbors[IMH1 + j], sd.neighbors[cell], sizeof(int) * NEIGHBORS_2D);

			// merge happened here

			// For each mRNA concentration, average the given cell's neighbors' Delta protein concentrations
			int* cells = neighbors[IMH1 + j];
			int time = WRAP(stc.time_cur - delays[j], sd.max_delay_size);
			double* cur_cons = cl.cons[CPDELTA][time];
			double sum;
			if (cell % sd.width_total == cl.active_start_record[time]) {
				sum = (cur_cons[cells[0]] + cur_cons[cells[3]] + cur_cons[cells[4]] + cur_cons[cells[5]]) / 4;
			} else if (cell % sd.width_total == cl.active_start_record[time]) {
				sum = (cur_cons[cells[0]] + cur_cons[cells[1]] + cur_cons[cells[2]] + cur_cons[cells[3]]) / 4;
			} else {
				sum = (cur_cons[cells[0]] + cur_cons[cells[1]] + cur_cons[cells[2]] + cur_cons[cells[3]] + cur_cons[cells[4]] + cur_cons[cells[5]]) / 6;
			}
			avg_delays[IMH1 + j] = sum;
			// EDIT MADE NOVEMBER 8th, 2016: MERGED TWO for LOOPS ITERATING OVER THE SAME DOMAIN, BOTH ASSIGNED "int cell = old_cells_mrna[IMH1 + j];"
		}
	}
	// Calculate every mRNA concentration
	for (int j = 0; j < NUM_INDICES; j++) {
		double mtrans;
		if (j == IMH13) { // her13 mRNA is not affected by dimers' repression
			mtrans = rs[RMSH13][stc.cell];
		} else {
			double avgpd;
			if (j >= IMH1 && j <= IMMESPB) {
				avgpd = avg_delays[IMH1 + j];
			} else { // delta mRNA is not affected by Delta-Notch signaling
				avgpd = 0;
			}

			double oe = 0;
			if (past_induction && !past_recovery && ((IMH1 + j) == md.overexpression_rate)) {
				oe = md.overexpression_factor;
			}
			if (j == IMMESPA && sd.section == SEC_ANT) {
				mtrans = transcription_mespa(rs, cl, WRAP(stc.time_cur - delays[j], sd.max_delay_size), old_cells_mrna[IMH1 + j], avgpd, rs[RMSH1 + j][stc.cell], oe, sd.section);
				//cout<<"mespa"<<mtrans<<endl;
			} else if (j == IMMESPB && sd.section == SEC_ANT) {
				mtrans = transcription_mespb(rs, cl, WRAP(stc.time_cur - delays[j], sd.max_delay_size), old_cells_mrna[IMH1 + j], avgpd, rs[RMSH1 + j][stc.cell], oe, sd.section);

			} else {
				mtrans = transcription(rs, cl, WRAP(stc.time_cur - delays[j], sd.max_delay_size), old_cells_mrna[IMH1 + j], avgpd, rs[RMSH1 + j][stc.cell], oe, sd.section);
			}

		}

		// The current mRNA concentration's differential equation
		cl.cons[CMH1 + j][stc.time_cur][stc.cell] =
			cl.cons[CMH1 + j][stc.time_prev][stc.cell]
			+ sd.step_size * (mtrans - rs[RMDH1 + j][stc.cell] * cl.cons[CMH1 + j][stc.time_prev][stc.cell]);
	}
}

/* transcription calculates mRNA transcription, taking into account the effects of dimer repression
	parameters:
		rs: the active rates
		cl: the concentration levels for simulating
		time: the current time
		cell: the current cell
		avgpd: the neighbor-averaged Delta protein concentration
		ms: the rate of mRNA synthesis
		oe: the rate of mRNA overexpression
	returns: the transcription factor
	notes:
	todo:
		TODO clean up these parameters
*/
inline double transcription (double** rs, con_levels& cl, int time, int cell, double avgpd, double ms, double oe, int section) {
	double th1h1, th7h13, tmespamespa = 0, tmespamespb = 0, tmespbmespb = 0, tdelta;
	th1h1 = rs[RCRITPH1H1][cell] == 0 ? 0 : cl.cons[CPH1H1][time][cell] / rs[RCRITPH1H1][cell];
	th7h13 = rs[RCRITPH7H13][cell] == 0 ? 0 : cl.cons[CPH7H13][time][cell] / rs[RCRITPH7H13][cell];
	//if (section == SEC_ANT) {
	  //  tmespamespa = rs[RCRITPMESPAMESPA][cell] == 0 ? 0 : cl.cons[CPMESPAMESPA][time][cell] / rs[RCRITPMESPAMESPA][cell];
	   // tmespamespb = rs[RCRITPMESPAMESPB][cell] == 0 ? 0 : cl.cons[CPMESPAMESPB][time][cell] / rs[RCRITPMESPAMESPB][cell];
	   // tmespbmespb = rs[RCRITPMESPBMESPB][cell] == 0 ? 0 : cl.cons[CPMESPBMESPB][time][cell] / rs[RCRITPMESPBMESPB][cell];
	//}
	tdelta = rs[RCRITPDELTA][cell] == 0 ? 0 : avgpd / rs[RCRITPDELTA][cell];
	return ms * (oe + (1 + tdelta) / (1 + tdelta + SQUARE(th1h1) + SQUARE(th7h13) + SQUARE(tmespamespa) + SQUARE(tmespamespb) + SQUARE(tmespbmespb)));
}

/* 151221: transcription_mespa calculates mRNA transcription for mespa, taking into account the effects of dimer repression
	parameters:
		rs: the active rates
		cl: the concentration levels for simulating
		time: the current time
		cell: the current cell
		avgpd: the neighbor-averaged Delta protein concentration
		ms: the rate of mRNA synthesis
		oe: the rate of mRNA overexpression
	returns: the transcription factor
	notes:
	todo:
		TODO clean up these parameters
*/
inline double transcription_mespa (double** rs, con_levels& cl, int time, int cell, double avgpd, double ms, double oe, int section) {
	double th1h1, th7h13, tmespbmespb = 0, tdelta;
	th1h1 = rs[RCRITPH1H1][cell] == 0 ? 0 : cl.cons[CPH1H1][time][cell] / rs[RCRITPH1H1][cell];
	th7h13 = rs[RCRITPH7H13][cell] == 0 ? 0 : cl.cons[CPH7H13][time][cell] / rs[RCRITPH7H13][cell];
	//if (section == SEC_ANT) {
		//tmespamespa = rs[RCRITPMESPAMESPA][cell] == 0 ? 0 : cl.cons[CPMESPAMESPA][time][cell] / rs[RCRITPMESPAMESPA][cell];
		//tmespamespb = rs[RCRITPMESPAMESPB][cell] == 0 ? 0 : cl.cons[CPMESPAMESPB][time][cell] / rs[RCRITPMESPAMESPB][cell];
	tmespbmespb = rs[RCRITPMESPBMESPB][cell] == 0 ? 0 : cl.cons[CPMESPBMESPB][time][cell] / rs[RCRITPMESPBMESPB][cell];
	//}
	tdelta = rs[RCRITPDELTA][cell] == 0 ? 0 : avgpd / rs[RCRITPDELTA][cell];

	return ms * (oe + (tdelta) / (tdelta + rs[NS1][cell] * SQUARE(th1h1) + SQUARE(th7h13) + SQUARE(tmespbmespb)));
}

/* 151221: transcription_mespb calculates mRNA transcription for mespb, taking into account the effects of dimer repression
	parameters:
		rs: the active rates
		cl: the concentration levels for simulating
		time: the current time
		cell: the current cell
		avgpd: the neighbor-averaged Delta protein concentration
		ms: the rate of mRNA synthesis
		oe: the rate of mRNA overexpression
	returns: the transcription factor
	notes:
	todo:
		TODO clean up these parameters
*/
inline double transcription_mespb (double** rs, con_levels& cl, int time, int cell, double avgpd, double ms, double oe, int section) {
	double tmespamespa = 0, tmespamespb = 0, tmespbmespb = 0, tdelta;
	//th1h1 = rs[RCRITPH1H1][cell] == 0 ? 0 : cl.cons[CPH1H1][time][cell] / rs[RCRITPH1H1][cell];
	//th7h13 = rs[RCRITPH7H13][cell] == 0 ? 0 : cl.cons[CPH7H13][time][cell] / rs[RCRITPH7H13][cell];
	//if (section == SEC_ANT) {
	tmespamespa = rs[RCRITPMESPAMESPA][cell] == 0 ? 0 : cl.cons[CPMESPAMESPA][time][cell] / rs[RCRITPMESPAMESPA][cell];
	tmespamespb = rs[RCRITPMESPAMESPB][cell] == 0 ? 0 : cl.cons[CPMESPAMESPB][time][cell] / rs[RCRITPMESPAMESPB][cell];
	tmespbmespb = rs[RCRITPMESPBMESPB][cell] == 0 ? 0 : cl.cons[CPMESPBMESPB][time][cell] / rs[RCRITPMESPBMESPB][cell];
	//}
	tdelta = rs[RCRITPDELTA][cell] == 0 ? 0 : rs[NS2][cell] * avgpd / rs[RCRITPDELTA][cell];

	return ms * (oe + (1 + tdelta) / (1 + tdelta + SQUARE(tmespamespa) + SQUARE(tmespamespb) + SQUARE(tmespbmespb)));
}

/* calc_neighbors_1d calculates a given cell's neighbors in a 1D simulation
	parameters:
		sd: the current simulation's data
		neighbors: the 2-element array of neighbor indices to fill
		cell: the index of the cell whose neighbors should be calculated
		active_start: the start of the active PSM
		active_end: the end of the active PSM
	returns: nothing
	notes:
	todo:
*/
void calc_neighbors_1d (sim_data& sd, int neighbors[], int cell, int active_start, int active_end) {
	if (cell == active_start) {
		neighbors[0] = cell - 1;
		neighbors[1] = cell - 1;
	} else if (cell == active_end) {
		neighbors[0] = cell + 1;
		neighbors[1] = cell + 1;
	} else {
		neighbors[0] = cell - 1;
		neighbors[1] = cell + 1;
	}

	neighbors[0] = WRAP(neighbors[0], sd.width_total);
	neighbors[1] = WRAP(neighbors[1], sd.width_total);
}

/* calc_neighbors_2d calculates a given cell's neighbors in a 2D simulation
	parameters:
		sd: the current simulation's data
	returns: nothing
	notes:
		2D simulations use a hexagonal grid of cells indexed like this:
		 ____  ____  ____  ____
		/    \/    \/    \/    \
		| 0  || 1  || 2  || 3  |
		\____/\____/\____/\____/__
		   /    \/    \/    \/    \
		   | 4  || 5  || 6  || 7  |
		 __\____/\____/\____/\____/
		/    \/    \/    \/    \
		| 8  || 9  || 10 || 11 |
		\____/\____/\____/\____/__
		   /    \/    \/    \/    \
		   | 12 || 13 || 14 || 15 |
		   \____/\____/\____/\____/

		This function should be called only when necessary due to the time cost; the populated neighbors array should be reused until invalid.
	todo:
*/
void calc_neighbors_2d (sim_data& sd) {
	for (int i = 0; i < sd.cells_total; i++) {
		if (i % 2 == 0) {																		// All even column cells
			sd.neighbors[i][0] = (i - sd.width_total + sd.cells_total) % sd.cells_total;			// Top
			sd.neighbors[i][1] = (i - sd.width_total + 1 + sd.cells_total) % sd.cells_total;		// Top-right
			sd.neighbors[i][2] = (i + 1) % sd.cells_total;											// Bottom-right
			sd.neighbors[i][3] = (i + sd.width_total) % sd.cells_total;								// Bottom
			if (i % sd.width_total == 0) {														// Left edge
				sd.neighbors[i][4] = i + sd.width_total - 1;										// Bottom-left
				sd.neighbors[i][5] = (i - 1 + sd.cells_total) % sd.cells_total;						// Top-left
			} else {																			// Not a left edge
				sd.neighbors[i][4] = i - 1;															// Bottom-left
				sd.neighbors[i][5] = (i - sd.width_total - 1 + sd.cells_total) % sd.cells_total;	// Top-left
			}
		} else {																				// All odd column cells
			sd.neighbors[i][0] = (i - sd.width_total + sd.cells_total) % sd.cells_total;			// Top
			if (i % sd.width_total == sd.width_total - 1) {											// Right edge
				sd.neighbors[i][1] = i - sd.width_total + 1;										// Top-right
				sd.neighbors[i][2] = (i + 1) % sd.cells_total;										// Bottom-right
			} else {																			// Not a right edge
				sd.neighbors[i][1] = i + 1;															// Top-right
				sd.neighbors[i][2] = (i + sd.width_total + 1 + sd.cells_total) % sd.cells_total;	// Nottom-right
			}																					// All odd column cells
			sd.neighbors[i][3] = (i + sd.width_total) % sd.cells_total;								// Bottom
			sd.neighbors[i][4] = (i + sd.width_total - 1) % sd.cells_total;							// Bottom-left
			sd.neighbors[i][5] = (i - 1 + sd.cells_total) % sd.cells_total;							// Top-left
		}
	}
}

/* perturb_rates_all perturbs every rate for every cell
	parameters:
		rs: the current simulation's rates
	returns: nothing
	notes:
	todo:
*/
void perturb_rates_all (rates& rs) {
	for (int i = 0; i < NUM_RATES; i++) {
		if (rs.factors_perturb[i] == 0) { // If the current rate has no perturbation factor then set every cell's rate to the base rate
			for (int j = 0; j < rs.cells; j++) {
				rs.rates_cell[i][j] = rs.rates_base[i];
			}
		} else { // If the current rate has a perturbation factor then set every cell's rate to a randomly perturbed positive or negative variation of the base with a maximum perturbation up to the rate's perturbation factor
			for (int j = 0; j < rs.cells; j++) {
				rs.rates_cell[i][j] = rs.rates_base[i] * random_perturbation(rs.factors_perturb[i]);
			}
		}
	}
}

/* perturb_rates_all perturbs every rate for every cell in the given column
	parameters:
		sd: the current simulation's data
		rs: the current simulation's rates
		column: the column of cells to perturb
	returns: nothing
	notes:
	todo:
*/
void perturb_rates_column (sim_data& sd, rates& rs, int column) {
	for (int i = 0; i < NUM_RATES; i++) {
		if (rs.factors_perturb[i] != 0) { // Alter only rates with a perturbation factor
			for (int j = 0; j < sd.height; j++) {
				rs.rates_cell[i][j * sd.width_total + column] = rs.rates_base[i] * random_perturbation(rs.factors_perturb[i]);
			}
		}
	}
}

/* random_perturbation calculates a random perturbation with a maximum absolute change up to the given perturbation factor
	parameters:
		perturb: the perturbation factor
	returns: the random perturbation
	notes:
	todo:
*/
inline double random_perturbation (double perturb) {
	return random_double(pair<double, double>(1 - perturb, 1 + perturb));
}

/* baby_to_cl copies the data from the given time step in baby_cl to cl
	parameters:
		baby_cl: the concentration levels for simulating
		cl: the concentration levels for analysis and storage
		baby_time: the time step to access baby_cl with
		time: the time step to access cl with
	returns: nothing
	notes:
	todo:
*/
void baby_to_cl (con_levels& baby_cl, con_levels& cl, int baby_time, int time) {
	for (int i = 0; i < cl.num_con_levels; i++) {
		for (int k = 0; k < cl.cells; k++) {
			cl.cons[i][time][k] = baby_cl.cons[i][baby_time][k];
		}
	}
	cl.active_start_record[time] = baby_cl.active_start_record[baby_time];
	cl.active_end_record[time] = baby_cl.active_end_record[baby_time];
}

/* anterior_time converts the given time step to its equivalent in anterior time
	parameters:
		sd: the current simulation's data
		time: the time step
	returns: the converted time step
	notes:
	todo:
*/
int anterior_time (sim_data& sd, int time) {
	return time / sd.big_gran - sd.steps_til_growth / sd.big_gran + sd.max_delay_size;
}

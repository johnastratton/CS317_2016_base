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
init.cpp contains initialization functions used before any simulations start.
*/

#include <cmath> // Needed for log10

#include "init.hpp" // Function declarations

#include "io.hpp"
#include "macros.hpp"
#include "main.hpp"
#include "sres.hpp"

using namespace std; 

terminal* term; // The global terminal struct
extern int printing_precision; // Declared in main.cpp

/* copy_str copies the given string, allocating enough memory for the new string
	parameters:
		str: the string to copy
	returns: a pointer to the new string
	notes:
	todo:
*/
char* copy_str (const char* str) { 
	if (str == NULL) {
		return NULL;
	} else {
		char* newstr = (char*)mallocate(sizeof(char) * (strlen(str) + 1));
		return strcpy(newstr, str);
	}
}

/* init_terminal creates and initializes a new terminal struct
	parameters:
	returns: nothing
	notes:
	todo:
*/
void init_terminal () {
	if (term != NULL) {
		delete term;
	}
	term = new terminal();
}

/* free_terminal frees the terminal from memory and resets the terminal text color to its default value
	parameters:
	returns: nothing
	notes:
	todo:
*/
void free_terminal () {
	cout << term->reset;
	delete term;
}

/* accept_input_params fills the given input_params with values from the given command-line arguments
	parameters:
		num_args: the number of command-line arguments (i.e. argc)
		args: the array of command-line arguments (i.e. argv)
		ip: the program's input parameters
	returns: nothing
	notes:
		Ensure the usage function's message in main.cpp matches the arguments this function accepts whenever editing or adding command-line argument acceptance.
	todo:
*/
void accept_input_params (int num_args, char** args, input_params& ip) {
	// Store the program's command-line arguments for MPI
	ip.argc = num_args;
	ip.argv = args;
	
	if (num_args > 1) { // If arguments were given (the 0th argument is the program name)
		for (int i = 1; i < num_args; i += 2) { // Iterate through each argument pair (if an argument does not have an accompanying value, i-- should be called when found)
			char* option = args[i];
			char* value;
			if (i < num_args - 1) {
				value = args[i + 1];
			} else {
				value = NULL;
			}
			
			// Accept command-line arguments in both short and long form
			if (option_set(option, "-r", "--ranges-file")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.ranges_file), value);
			} else if (option_set(option, "-f", "--simulation")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.sim_file), value);
			} else if (option_set(option, "-o", "--print-good-sets")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.good_sets_file), value);
				ip.print_good_sets = true;
			} else if (option_set(option, "-G", "--good-set-threshold")) {
				ensure_nonempty(option, value);
				ip.good_set_threshold = atof(value);
				if (ip.good_set_threshold < 0 || ip.good_set_threshold > 1) {
					usage("The threshold for a good set must be a possible score. Set -G or --good-set-threshold to between 0 and 1.");
				}
			} else if (option_set(option, "-d", "--dimensions")) {
				ensure_nonempty(option, value);
				ip.num_dims = atoi(value);
				if (ip.num_dims < 1) {
					usage("The simulation must include a positive number of dimensions. Set -d or --dimensions to at least 1.");
				}
			} else if (option_set(option, "-P", "--parent-population")) {
				ensure_nonempty(option, value);
				ip.pop_parents = atoi(value);
				if (ip.pop_parents < 1) {
					usage("The parent population must be at least one simulation. Set -P or --parent-population to at least 1.");
				}
			} else if (option_set(option, "-p", "--total-population")) {
				ensure_nonempty(option, value);
				ip.pop_total = atoi(value);
				if (ip.pop_total < 1) {
					usage("The total population must be at least one simulation. Set -p or --child-population to at least 1.");
				}
			} else if (option_set(option, "-g", "--generations")) {
				ensure_nonempty(option, value);
				ip.generations = atoi(value);
				if (ip.generations < 1) {
					usage("The population must exist for at least one generation. Set -g or --generations to at least 1.");
				}
			} else if (option_set(option, "-s", "--seed")) {
				ensure_nonempty(option, value);
				ip.seed = atoi(value);
				if (ip.seed <= 0) {
					usage("The seed to generate random numbers must be a positive integer. Set -s or --seed to at least 1.");
				}
			} else if (option_set(option, "-e", "--printing-precision")) {
				ensure_nonempty(option, value);
				ip.printing_precision = atoi(value);
				if (ip.printing_precision < 1) {
					usage("The printing precision must be a positive integer. Set -e or --printing-precision to at least 1.");
				}
			} else if (option_set(option, "-R", "--gradients-file")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.gradients_file), value);
				ip.read_gradients = true;
			} else if (option_set(option, "-i", "--gradient-index")) {
				ensure_nonempty(option, value);
				int index = atoi(value);
				if (index < 0 || index > 44) {
					usage("Gradient indices must be valid parameter indices. Set each -i or --gradient-index to between 0 and 44 (inclusive).");
				}
				add_gradient_index(&(ip.gradient_indices), index);
			} else if (option_set(option, "-S", "--params-file")) {
				ensure_nonempty(option, value);
				store_filename(&(ip.sets_file), value);
			} else if (option_set(option, "-n", "--number-of-sets")) {
				ensure_nonempty(option, value);
				ip.num_sets = atoi(value);
				if (ip.num_sets < 1) {
					usage("The number of sets to simulate must be a positive integer. Set -n or --number-of-sets to at least 1.");
				}
			} else if (option_set(option, "-a", "--arguments")) {
				ensure_nonempty(option, value);
				++i;
				ip.num_sim_args = num_args - i + NUM_IMPLICIT_SIM_ARGS;
				ip.sim_args = (char**)mallocate(sizeof(char*) * (ip.num_sim_args));
				for (int j = 1; j < ip.num_sim_args - (NUM_IMPLICIT_SIM_ARGS - 1); j++) {
					char* arg = args[i + j - 1];
					ip.sim_args[j] = (char*)mallocate(sizeof(char) * (strlen(arg) + 1));
					sprintf(ip.sim_args[j], "%s", arg);
				}
				i = num_args;
			} else if (option_set(option, "-c", "--no-color")) {
				mfree(term->blue);
				mfree(term->red);
				mfree(term->yellow);
				mfree(term->reset);
				term->blue = copy_str("");
				term->red = copy_str("");
				term->yellow = copy_str("");
				term->reset = copy_str("");
				i--;
			} else if (option_set(option, "-v", "--verbose")) {
				if (!ip.verbose) {
					ip.verbose = true;
				}
				i--;
			} else if (option_set(option, "-q", "--quiet")) {
				if (!ip.quiet) {
					ip.quiet = true;
					ip.cout_orig = cout.rdbuf();
					cout.rdbuf(ip.null_stream->rdbuf());
					term->set_verbose_streambuf(ip.null_stream->rdbuf());
				}
				i--;
			} else if (option_set(option, "-h", "--help")) {
				usage("");
				i--;
			} else if (option_set(option, "-l", "--licensing")) { 
				licensing();
				i--;
			} else {
				const char* message_0 = "'";
				const char* message_1 = "' is not a valid option! Please check that every argument matches one available in the following usage information.";
				char* message = (char*)mallocate(sizeof(char) * (strlen(message_0) + strlen(option) + strlen(message_1) + 1));
				sprintf(message, "%s%s%s", message_0, option, message_1);
				usage(message);
			}
		}
	}
}

/* option_set checks if the given string matches either given version (short or long) of an option
	parameters:
		option: the string to check
		short_name: the short version of the option
		long_name: the long version of the option
	returns: true if the string matches a version, false otherwise
	notes:
	todo:
*/
inline bool option_set (const char* option, const char* short_name, const char* long_name) {
	return strcmp(option, short_name) == 0 || strcmp(option, long_name) == 0;
}

/* ensure_nonempty ensures that an option that should have an associated value has one or exits with an error
	parameters:
		option: the option to check
		arg: the value to check for
	returns: nothing
	notes:
	todo:
*/
void ensure_nonempty (const char* option, const char* arg) {
	if (arg == NULL) {
		char* message = (char*)mallocate(strlen("Missing the argument for the '' option.") + strlen(option) + 1);
		sprintf(message, "Missing the argument for the '%s' option.", option);
		usage(message);
	}
}

/* check_input_params checks that the given command-line arguments are semantically valid
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
		Since command-line arguments may be given in any order, it is impossible to check certain requirements immediately after an argument is read. This function is called after accept_input_params and therefore has access to every argument.
	todo:
*/
void check_input_params (input_params& ip) {
	if (ip.ranges_file == NULL) {
		usage("A ranges file must be specified! Set the ranges file with -r or --ranges-file.");
	}
	if (ip.sets_file == NULL) {
		usage("A parameter sets file must be specified! Set the parameter sets file with -S or --params-file.");
	}
	if (ip.gradient_indices == NULL) {
		usage("At least one parameter index must be altered by gradients! Add at least one instance of -i or --gradient-index to an index to alter.");
	}
	printing_precision = ip.printing_precision; // ip cannot be imported into a C file so the printing precision must be its own global
}

/* add_gradient_index adds an index to the given list of gradient indices
	parameters:
		gi: a pointer to the list of gradients (or a pointer to NULL if the list is empty)
		index: the index to add
	returns: nothing
	notes:
	todo:
*/
void add_gradient_index (gradient_index** gi, int index) {
	gradient_index* gi_new = (gradient_index*)mallocate(sizeof(gradient_index));
	if (*gi == NULL) {
		gi_new->next = NULL;
		*gi = gi_new;
	} else {
		gi_new->next = *gi;
		*gi = gi_new;
	}
	(*gi)->index = index;
}

/* read_sets fills in the parameter sets array with the specified parameter sets file
	parameters:
		ip: the program's input parameters
		params_data: the input_data for the parameter sets input file
		sets: the array to add parameter sets to
	returns: nothing
	notes:
		This function is responsible for filling in sets via whatever method the user specified so add any future input methods here.
	todo:
*/
void read_sets (input_params& ip, input_data& params_data) {
	read_file(&params_data);
	ip.sets = new double*[ip.num_sets];
	for (int i = 0; i < ip.num_sets; i++) {
		if (params_data.index < params_data.size) { // Parse only as many lines as specified, even if the file is longer
			ip.sets[i] = new double[NUM_PARS];
			memset(ip.sets[i], 0, sizeof(double) * NUM_PARS);
			if(!parse_param_line(ip.sets[i], params_data.buffer, params_data.index)) { // Parse each line until the file is empty or the required number of sets have been found
				ip.num_sets = i;
			}
		} else {
			ip.num_sets = i;
		}
	}
}

/* read_base_gradients fills in the base gradients string with the specified gradients file
	parameters:
		ip: the program's input parameters
		gradients_data: the input_data for the gradients input file
	returns: nothing
	notes:
		This function is responsible for filling in sets via whatever method the user specified so add any future input methods here.
	todo:
*/
void read_base_gradients (input_params& ip, input_data& gradients_data) {
	if (gradients_data.filename != NULL) {
		read_file(&gradients_data);
		ip.base_gradients = gradients_data.buffer;
	}
}

/* init_verbosity sets the verbose stream to /dev/null if verbose mode is not enabled
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void init_verbosity (input_params& ip) {
	if (!ip.verbose) {
		term->set_verbose_streambuf(ip.null_stream->rdbuf());
	}
}

/* create_good_sets_file creates a file to store the sets that received a good score
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void create_good_sets_file (input_params& ip) {
	if (ip.print_good_sets) { // Print the good sets only if the user specified it
		if (get_rank() == 0) {
			open_file(&(ip.good_sets_stream), ip.good_sets_file, false);
		}
	}
}

/* init_sim_args initializes the arguments to be passed into every simulation
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void init_sim_args (input_params& ip) {
	if (ip.num_sim_args == 0) { // If the arguments were not initialized in accept_input_params (i.e. the user did not specify simulation arguments with -a or --arguments)
		ip.num_sim_args = NUM_IMPLICIT_SIM_ARGS; // "simulation --pipe-in x --pipe-out y" takes 5 terms and the final NULL element makes the sum 6
		ip.sim_args = (char**)mallocate(sizeof(char*) * NUM_IMPLICIT_SIM_ARGS);
	}
	
	// Initialize the implicit arguments
	ip.sim_args[0] = copy_str("simulation");
	ip.sim_args[ip.num_sim_args - 7] = copy_str("--pipe-in");
	ip.sim_args[ip.num_sim_args - 6] = copy_str("0");
	ip.sim_args[ip.num_sim_args - 5] = copy_str("--pipe-out");
	ip.sim_args[ip.num_sim_args - 4] = copy_str("0");
	ip.sim_args[ip.num_sim_args - 3] = copy_str("--gradients-file");
	ip.sim_args[ip.num_sim_args - 2] = NULL;
	ip.sim_args[ip.num_sim_args - 1] = NULL;
}

/* copy_args copies the given array of arguments
	parameters:
		args: the array of arguments to copy
		num_args: the number of elements in the array of arguments
	returns: the new array of arguments
	notes:
	todo:
*/
char** copy_args (char** args, int num_args) {
	char** new_args = (char**)mallocate(sizeof(char*) * num_args);
	for (int i = 0; i < num_args; i++) {
		new_args[i] = copy_str(args[i]);
	}
	return new_args;
}

/* read_ranges fills in the sres_params struct with ranges from the given ranges file
	parameters:
		ip: the program's input parameters
		ranges_data: the input_data for the ranges input file
		sp: parameters required by libSRES to put the ranges in
	returns: nothing
	notes:
	todo:
*/
void read_ranges (input_params& ip, input_data& ranges_data, sres_params& sp) {
	read_file(&ranges_data);
	sp.lb = (double*)mallocate(sizeof(double) * ip.num_dims); // Lower bounds
	sp.ub = (double*)mallocate(sizeof(double) * ip.num_dims); // Upper bounds
	parse_ranges_file(ranges_data.buffer, ip, sp);
}

/* store_pipe stores the given pipe file descriptor into the given index in the array of arguments
	parameters:
		args: the array of arguments
		index: the index in the array to place the pipe file descriptor
		pipe: the file descriptor of the pipe
	returns: nothing
	notes:
	todo:
*/
void store_pipe (char** args, int index, int pipe) {
	mfree(args[index]);
	int int_size = log10(pipe > 0 ? pipe : 1) + 1; // How many bytes the ASCII representation of pipe takes
	args[index] = (char*)mallocate(sizeof(char) * (int_size + 1));
	sprintf(args[index], "%d", pipe);
}

/* delete_file closes all of the used files and frees them from memory
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void delete_files (input_params& ip) {
	close_if_open(ip.good_sets_stream);
}

/* reset_cout resets the cout buffer to its original stream if quiet mode was on and cout was therefore redirected to /dev/null
	parameters:
		ip: the program's input parameters
	returns: nothing
	notes:
	todo:
*/
void reset_cout (input_params& ip) {
	if (ip.quiet) {
		cout.rdbuf(ip.cout_orig);
	}
}


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
main.cpp contains the main, usage, and licensing functions.
Avoid putting functions in main.cpp that could be put in a more specific file.
*/

// Include MPI if compiled with it
#if defined(MPI)
	#undef MPI // MPI uses this macro as well, so temporarily undefine it
	#include <mpi.h> // Needed for MPI_Comm_rank, MPI_COMM_WORLD
	#define MPI // The MPI macro should be checked only for definition, not value
#endif

#include "main.hpp" // Function declarations

#include "init.hpp"
#include "macros.hpp"
#include "sres.hpp"

using namespace std;

extern terminal* term; // Declared in init.cpp

// libSRES does not allow additional parameters to be passed into the fitness function so to pass non-libSRES data to each simulation ip must be global
input_params ip;
int printing_precision; // ip cannot be imported into a C file

/* main is called when the program is run and performs all program functionality
	parameters:
		argc: the number of command-line arguments
		argv: the array of command-line arguments
	returns: 0 on success, a positive integer on failure
	notes:
		Main should only delegate functionality; let the functions it calls handle specific tasks. This keeps the function looking clean and helps maintain the program structure.
	todo:
*/
int main (int argc, char** argv) {
	// Initialize MPI if compiled with it
	#if defined(MPI)
		MPI_Init(&argc, &argv);
	#endif
	
	// Initialize the program's terminal functionality and input parameters
	init_terminal();
	accept_input_params(argc, argv, ip);
	check_input_params(ip);
	init_verbosity(ip);
	init_sim_args(ip);
	
	// Read the specified input files
	input_data ranges_data(ip.ranges_file);
	input_data sets_data(ip.sets_file);
	input_data gradients_data(ip.gradients_file);
	read_sets(ip, sets_data);
	read_base_gradients(ip, gradients_data);
	
	// Create the specified output files
	create_good_sets_file(ip);
	
	// Initialize libSRES and the ranges it will use
	sres_params sp;
	read_ranges(ip, ranges_data, sp);
	init_sres(ip, sp);
	
	// Run libSRES
	run_sres(sp);
	
	// Free used memory, wrap up libSRES, etc.
	free_sres(sp);
	delete_files(ip);
	#if defined(MEMTRACK)
		print_heap_usage();
	#endif
	free_terminal();
	reset_cout(ip);
	return 0;
}

/* usage prints the usage information and, optionally, an error message and then exits
	parameters:
		message: an error message to print before the usage information (set message to NULL or "\0" to not print any error)
	returns: nothing
	notes:
		This function exits after printing the usage information.
		Note that accept_input_params in init.cpp handles actual command-line input and that this information should be updated according to that function.
	todo:
		TODO somehow free memory even with the abrupt exit
*/
void usage (const char* message) {
	cout << endl;
	bool error = message != NULL && message[0] != '\0';
	if (error) {
		cout << term->red << message << term->reset << endl << endl;
	}
	cout << "Usage: [-option [value]]. . . [--option [value]]. . ." << endl;
	cout << "-r, --ranges-file        [filename]   : the relative filename of the ranges input file, default=none" << endl;
	cout << "-f, --simulation         [filename]   : the relative filename of the simulation executable, default=../simulation/simulation" << endl;
	cout << "-o, --print-good-sets    [filename]   : the relative filename of the good sets output file, default=none" << endl;
	cout << "-G, --good-set-threshold [float]      : the worst score a set must receive to be printed to the good sets file, default=0.0" << endl;
	cout << "-d, --dimensions         [int]        : the number of dimensions (i.e. rate parameters) to explore, min=1, default=3" << endl;
	cout << "-P, --parent-population  [int]        : the population of parent simulations to use each generation, min=1, default=3" << endl;
	cout << "-p, --total-population   [int]        : the population of total simulations to use each generation, min=1, default=20" << endl;
	cout << "-g, --generations        [int]        : the number of generations to run before returning results, min=1, default=1750" << endl;
	cout << "-s, --seed               [int]        : the seed used in the evolutionary strategy (not simulations), min=1, default=time" << endl;
	cout << "-e, --printing-precision [int]        : how many digits of precision parameters should be printed with, min=1, default=6" << endl;
	cout << "-R, --gradients-file     [filename]   : the relative filename of the gradients file that supplies base gradients to every generated gradients file, default=none" << endl;
	cout << "-i, --gradient-index     [int]        : the index of a parameter to apply gradients to, can be entered multiple times, min=1, max=# of dimensions, default=none" << endl;
	cout << "-S, --params-file        [filename]   : the relative filename of the parameter sets input file, default=none" << endl;
	cout << "-n, --number-of-sets     [int]        : the number of parameter sets to run each simulation" << endl;
	cout << "-a, --arguments          [N/A]        : every argument following this will be sent to the simulation" << endl;
	cout << "-c, --no-color           [N/A]        : disable coloring the terminal output, default=unused" << endl;
	cout << "-v, --verbose            [N/A]        : print detailed messages about the program state" << endl;
	cout << "-q, --quiet              [N/A]        : hide the terminal output, default=unused" << endl;
	cout << "-l, --licensing          [N/A]        : view licensing information (no simulations will be run)" << endl;
	cout << "-h, --help               [N/A]        : view usage information (i.e. this)" << endl;
	cout << endl << term->blue << "Example: ./sres-sampler -r input.ranges -f ../simulation/simulation --print-good-sets good-sets.params -G 0.05 --arguments -x 4 -w 4 -y 4 -G 1200" << term->reset << endl << endl;
	if (error) {
		exit(EXIT_INPUT_ERROR);
	} else {
		exit(EXIT_SUCCESS);
	}
}

/* licensing prints the program's copyright and licensing information and then exits
	parameters:
	returns: nothing
	notes:
	todo:
*/
void licensing () {
	cout << endl;
	cout << "Stochastically ranked evolutionary strategy sampler for zebrafish segmentation" << endl;
	cout << "Copyright (C) 2013 Ahmet Ay (aay@colgate.edu), Jack Holland (jholland@colgate.edu), Adriana Sperlea (asperlea@colgate.edu), Sebastian Sangervasi (ssangervasi@colgate.edu)" << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY" << endl;
	cout << "This is free software, and you are welcome to redistribute it under certain conditions;" << endl;
	cout << "You can use this code and modify it as you wish under the condition that you refer to the article: \"Short-lived Her proteins drive robust synchronized oscillations in the zebrafish segmentation clock\" (Development 2013 140:3244-3253; doi:10.1242/dev.093278)" << endl;
	cout << endl;
	exit(EXIT_SUCCESS);
}


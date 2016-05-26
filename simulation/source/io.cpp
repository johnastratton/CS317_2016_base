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
io.cpp contains functions for input and output of files and pipes. All I/O related functions should be placed in this file.
*/

#include <cerrno> // Needed for errno, EEXIST
#include <cstdio> // Needed for fopen, fclose, fseek, ftell, rewind
#include <sys/stat.h> // Needed for mkdir
#include <unistd.h> // Needed for read, write, close

#include "io.hpp" // Function declarations
#include "sim.hpp" // Needed for anterior_time

#include "main.hpp"

using namespace std;

extern terminal* term; // Declared in init.cpp

/* not_EOL returns whether or not a given character is the end of a line or file (i.e. '\n' or '\0', respectively)
	parameters:
		c: the character to check
	returns: true if c is the end of a line or file, false otherwise
	notes:
		When reading input file strings, use this instead of a straight newline check to avoid EOF (end of file) issues.
	todo:
*/
bool not_EOL (char c) {
	return c != '\n' && c != '\0';
}

/* store_filename stores the given value in the given field
	parameters:
		field: a pointer to the filename's field
		value: the filename to store
	returns: nothing
	notes:
		The previous field value is freed before assigning the new one.
	todo:
*/
void store_filename (char** field, const char* value) {
	mfree(*field);
	*field = copy_str(value);
}

/* create_dir creates a directory with the given path and name
	parameters:
		dir: the path and name of the directory to create
	returns: nothing
	notes:
		If the directory already exists, the error about it is suppressed.
	todo:
*/
void create_dir (char* dir) {
	cout << term->blue << "Creating " << term->reset << dir << " directory if necessary . . . ";
	
	// Create a directory, allowing the owner and group to read and write but others to only read
	if (mkdir(dir, 0775) != 0 && errno != EEXIST) { // If the error is that the directory already exists, ignore it
		cout << term->red << "Couldn't create '" << dir << "' directory!" << term->reset << endl;
		exit(EXIT_FILE_WRITE_ERROR);
	}
	
	term->done();
}

/* open_file opens the file with the given name and stores it in the given output file stream
	parameters:
		file_pointer: a pointer to the output file stream to open the file with
		file_name: the path and name of the file to open
		append: if true, the file will appended to, otherwise any existing data will be overwritten
	returns: nothing
	notes:
	todo:
*/
void open_file (ofstream* file_pointer, char* file_name, bool append) {
	try {
		if (append) {
			cout << term->blue << "Opening " << term->reset << file_name << " . . . ";
			file_pointer->open(file_name, fstream::app);
		} else {
			cout << term->blue << "Creating " << term->reset << file_name << " . . . ";
			file_pointer->open(file_name, fstream::out);
		}
	} catch (ofstream::failure) {
		cout << term->red << "Couldn't write to " << file_name << "!" << term->reset << endl;
		exit(EXIT_FILE_WRITE_ERROR);
	}
	term->done();
}

/* read_file takes an input_data struct and stores the contents of the associated file in a string
	parameters:
		ifd: the input_data struct to contain the file name, buffer to store the contents, size of the file, and current index
	returns: nothing
	notes:
		The buffer in ifd will be sized large enough to fit the file
	todo:
*/
void read_file (input_data* ifd) {
	cout << term->blue << "Reading file " << term->reset << ifd->filename << " . . . ";
	
	// Open the file for reading
	FILE* file = fopen(ifd->filename, "r");
	if (file == NULL) {
		cout << term->red << "Couldn't open " << ifd->filename << "!" << term->reset << endl;
		exit(EXIT_FILE_READ_ERROR);
	}
	
	// Seek to the end of the file, grab its size, and then rewind
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	ifd->size = size;
	rewind(file);
	
	// Allocate enough memory to contain the whole file
	ifd->buffer = (char*)mallocate(sizeof(char) * size + 1);
	
	// Copy the file's contents into the buffer
	long result = fread(ifd->buffer, 1, size, file);
	if (result != size) {
		cout << term->red << "Couldn't read from " << ifd->filename << term->reset << endl;
		exit(EXIT_FILE_READ_ERROR);
	}
	ifd->buffer[size] = '\0';
	
	// Close the file
	if (fclose(file) != 0) {
		cout << term->red << "Couldn't close " << ifd->filename << term->reset << endl;
		exit(EXIT_FILE_READ_ERROR);
	}
	
	term->done();
}

/* parse_param_line reads a line in the given parameter sets buffer and stores it in the given array of doubles
	parameters:
		params: the array of doubles to store the parameters in
		buffer_line: the buffer with the line to read
		index_buffer: the index of the buffer to start from
	returns: true if a line was found, false if the end of the file was reached without finding a valid line
	notes:
		The buffer should contain one parameter set per line, each set containing comma-separated floating point parameters.
		Blank lines and lines starting with # will be ignored.
		Each line must contain the correct number of parameters or the program will exit.
		index_buffer is a reference, allowing this function to store where it finished parsing.
	todo:
*/
bool parse_param_line (double* params, char* buffer_line, int& index_buffer) {
	static const char* usage_message = "There was an error reading the given parameter sets file.";
	int index_params = 0; // Current index in params
	int index_digits = index_buffer; // Index of the start of the digits to read
	int i = index_buffer; // Current index in buffer_line
	int line_start = i; // The start of the line, used to tell whether or not a line is empty
	for (; not_EOL(buffer_line[i]); i++) {
		if (buffer_line[i] == '#') { // Skip any lines starting with #
			for(; not_EOL(buffer_line[i]); i++);
			i++;
		} else if (buffer_line[i] == ',') { // Indicates the end of the digits to read
			if (sscanf(buffer_line + index_digits, "%lf", &(params[index_params++])) < 1) { // Convert the string of digits to a double when storing it in params
				usage(usage_message);
			}
			index_digits = i + 1;
		}
	}
	index_buffer = i + 1;
	if (i - line_start > 0) { // This line has content
		if (sscanf(buffer_line + index_digits, "%lf", &(params[index_params++])) < 1) {
			usage(usage_message);
		}
		if (index_params != NUM_RATES) {
			cout << term->red << "The given parameter sets file contains sets with an incorrect number of rates! This simulation requires " << NUM_RATES << " per set but at least one line contains " << index_params << " per set." << term->reset << endl;
			exit(EXIT_INPUT_ERROR);
		}
		return true;
	} else if (buffer_line[index_buffer] != '\0') { // There are more lines to try to parse
		return parse_param_line(params, buffer_line, index_buffer);
	} else { // The end of the buffer was found
		return false;
	}
}

/* parse_ranges_file reads the given buffer and stores every range found in the given ranges array
	parameters:
		ranges: the array of pairs in which to store the lower and upper bounds of each range
		buffer: the buffer with the ranges to read
	returns: nothing
	notes:
		The buffer should contain one range per line, starting the name of the parameter followed by the bracked enclosed lower and then upper bound optionally followed by comments.
		e.g. 'msh1 [30, 65] comment'
		The name of the parameter is so humans can conveniently read the file and has no semantic value to this parser.
		Blank lines and lines starting with # will be ignored. Anything after the upper bound is ignored.
	todo:
*/
void parse_ranges_file (pair <double, double> ranges[], char* buffer) {
	int i = 0;
	int rate = 0;
	for (; buffer[i] != '\0'; i++) {
		// Ignore lines starting with #
		while (buffer[i] == '#') {
			while (buffer[i] != '\n' && buffer[i] != '\0') {i++;}
			i++;			
		}
		
		// Ignore whitespace before the opening bracket
		while (buffer[i] != '[' && buffer[i] != '\0') {i++;}
		if (buffer[i] == '\0') {break;}
		i++;
		
		// Read the bounds
		ranges[rate].first = atof(buffer + i);
		while (buffer[i] != ',') {i++;}
		i++;
		ranges[rate].second = atof(buffer + i);
		if (ranges[rate].first < 0 || ranges[rate].second < 0) { // If the ranges are invalid then set them to 0
			ranges[rate].first = 0;
			ranges[rate].second = 0;
		}
		rate++;
		// Skip any comments until the end of the line
		while (buffer[i] != '\n' && buffer[i] != '\0') {i++;}
	}
}

/* print_passed prints the parameter sets that passed all required conditions of all required mutants
	parameters:
		ip: the program's input parameters
		file_passed: a pointer to the output file stream to open the file with
		rs: the current simulation's rates to pull the parameter sets from
	returns: nothing
	notes:
		This function prints each parameter separated by a comma, one set per line.
	todo:
*/
void print_passed (input_params& ip, ofstream* file_passed, rates& rs) {
	if (ip.print_passed) { // Print which sets passed only if the user specified it
		try {
			*file_passed << rs.rates_base[0];
			for (int i = 1; i < NUM_RATES; i++) {
				*file_passed << "," << rs.rates_base[i];
			}
			*file_passed << endl;
		} catch (ofstream::failure) {
			cout << term->red << "Couldn't write to " << ip.passed_file << "!" << term->reset << endl;
			exit(EXIT_FILE_WRITE_ERROR);
		}
	}
}

/* print_concentrations prints the concentration values of every cell at every time step of the given mutant for the given run
	parameters:
		ip: the program's input parameters
		sd: the current simulation's data
		cl: the current simulation's concentration levels
		md: mutant data
		filename_cons: the path and name of the file in which to store the concentration levels
		set_num: the index of the parameter set whose concentration levels are being printed
	returns: nothing
	notes:
		The first line of the file is the total width then a space then the height of the simulation.
		Each line after starts with the time step then a space then space-separated concentration levels for every cell ordered by their position relative to the active start of the PSM.
		If binary mode is set, the file will get the extension .bcons and print raw binary values, not ASCII text.
		The concentration printed is mutant dependent, but usually mh1.
	todo:
*/
void print_concentrations (input_params& ip, sim_data& sd, con_levels& cl, mutant_data& md, char* filename_cons, int set_num) {
	if (ip.print_cons) { // Print the concentrations only if the user specified it
		int strlen_set_num = INT_STRLEN(set_num); // How many bytes the ASCII representation of set_num takes
		char* str_set_num = (char*)mallocate(sizeof(char) * (strlen_set_num + 1));
		sprintf(str_set_num, "%d", set_num);
		char* extension;
		if (ip.binary_cons_output) { // Binary files get the extension .bcons
			extension = copy_str(".bcons");
		} else { // ASCII files (the default option) get the extension specified by the user
			extension = copy_str(".cons");
		}
		char* filename_set = (char*)mallocate(sizeof(char) * (strlen(filename_cons) + strlen("set_") + strlen_set_num + strlen(extension) + 1));
		sprintf(filename_set, "%sset_%s%s", filename_cons, str_set_num, extension);
		
		cout << "    "; // Offset the open_file message to preserve horizontal spacing
		ofstream file_cons;
		open_file(&file_cons, filename_set, sd.section == SEC_ANT);
		mfree(filename_set);
		mfree(str_set_num);
		mfree(extension);
		
		// If the file was just created then prepend the concentration levels with the simulation size
		if (sd.section == SEC_POST) {
			if (ip.binary_cons_output) {
				file_cons.write((char*)(&sd.width_total), sizeof(int));
				file_cons.write((char*)(&sd.height), sizeof(int));
			} else {
				file_cons << sd.width_total << " " << sd.height << "\n";
			}
		}
		
		// Calculate which time steps to print
		int step_offset = (sd.section == SEC_ANT) * ((sd.steps_til_growth - sd.time_start) / sd.big_gran + 1); // If the file is being appended to then offset the time steps
		int start = sd.time_start / sd.big_gran;
		int end = sd.time_end / sd.big_gran;
		
		// Print the concentration levels of every cell at every time step
		if (ip.binary_cons_output) {
			for (int j = start; j < end; j++) {
				int time_step = (j + step_offset) * sd.big_gran;
				file_cons.write((char*)(&time_step), sizeof(int));
				for (int i = 0; i < sd.height; i++) {
					int num_printed = 0;
					for (int k = cl.active_start_record[j]; num_printed < sd.width_total; k = WRAP(k - 1, sd.width_total), num_printed++) {
						float con = cl.cons[md.print_con][j][i * sd.width_total + k];	
						file_cons.write((char*)(&con), sizeof(float));
					}
				}
			}
		} else {
			for (int j = start; j < end; j++) {
				int time_step = (j + step_offset) * sd.big_gran;
				file_cons << time_step << " ";
				for (int i = 0; i < sd.height; i++) {
					int num_printed = 0;
					for (int k = cl.active_start_record[j]; num_printed < sd.width_total; k = WRAP(k - 1, sd.width_total), num_printed++) {
						file_cons << cl.cons[md.print_con][j][i * sd.width_total + k] << " ";
					}
				}
				file_cons << "\n";
			}
		}
	}
}

/* print_cell columns prints the concentrations of a number of columns of cells given by the user to an output file for plotting from the cells birth
	to their death:
		ip: the program's input parameters
		sd: the current simulation's data
		cl: the current simulation's concentration levels
		filename_cons: the path and name of the file in which to store the file being created
		set_num: the index of the parameter set whose concentration levels are being printed
	returns: nothing
	notes:
		The first line of the file is the number of columns printed then a space then the total height of the simulation
		Each line after starts with the time step then a space then space-separated concentration levels for every cell ordered by their position relative to the active start of the PSM.
		The number of columns given is relative to the start of the PSM and the cells are traced until one of the columns enters the determined region and is overwritten.
		The concentration printed is mutant dependent, but usually mh1.
	todo:
*/

void print_cell_columns (input_params& ip, sim_data& sd, con_levels &cl, char* filename_cons, int set_num) {
	if (ip.num_colls_print) { // Print the cells only if the user specified it
        //cerr << "But why are we here" << endl;
		int strlen_set_num = INT_STRLEN(set_num); // How many bytes the ASCII representation of set_num takes
		char* str_set_num = (char*)mallocate(sizeof(char) * (strlen_set_num + 1));
		sprintf(str_set_num, "%d", set_num);
		char* filename_set = (char*)mallocate(sizeof(char) * (strlen(filename_cons) + strlen("set_") + strlen_set_num + strlen(".cells") + 1));
		sprintf(filename_set, "%sset_%s.cells", filename_cons, str_set_num);
		
		cout << "    "; // Offset the open_file message to preserve horizontal spacing
		ofstream file_cons;
		open_file(&file_cons, filename_set, false);
		file_cons << sd.height << " " << ip.num_colls_print << endl;
		mfree(filename_set);
		mfree(str_set_num);

		int time_full = anterior_time(sd, sd.steps_til_growth + (sd.width_total - sd.width_initial - 1) * sd.steps_split) ; // Time after which the PSM is full of cells
		int time = time_full;
		double time_point[sd.height * ip.num_colls_print]; // Array for storing all the cells that need to be printed at each time point
		memset(time_point, 0, sizeof(double) * sd.height * ip.num_colls_print);
		int first_active_start = cl.active_start_record[time_full];
		
		while ((time < time_full + sd.steps_split) || cl.active_start_record[time] != first_active_start) {
			file_cons << time - time_full << " ";
			
			int col = 0;
			for (; col < ip.num_colls_print; col++) {
				int cur_col = WRAP(first_active_start + col, sd.width_total);
				for (int line = 0; line < sd.height; line++) {
					time_point[line * ip.num_colls_print + col] = cl.cons[CMMESPA][time][line * sd.width_total + cur_col];
				}
			
				if (cur_col == cl.active_start_record[time]) {
					col++;
					break;
				}
			}
			
			// If not all the cells we're following are born yet, fill the rest of the slots with -1 for no data
			for (; col < ip.num_colls_print; col++) {
				for (int line = 0; line < sd.height; line++) {
					time_point[line * ip.num_colls_print + col] = -1;
				}
			}
			
			for (int cell = 0; cell < sd.height * ip.num_colls_print; cell++) {
				file_cons << time_point[cell] << " ";
			}
			
			file_cons << endl;
			time++;
		}
		
		int end_col = WRAP(first_active_start + ip.num_colls_print - 1, sd.width_total);
		while (cl.active_start_record[time] != WRAP(end_col, sd.width_total)) {
			int col = 0;
			file_cons << time - time_full << " ";
			
			for (; col < ip.num_colls_print; col++) {
				int cur_col = WRAP(end_col - col, sd.width_total);
				if (cur_col == cl.active_start_record[time]) {
					break;
				}
				
				for (int line = 0; line < sd.height; line++) {
					time_point[line * ip.num_colls_print + (ip.num_colls_print - col - 1)] = cl.cons[CMMESPA][time][line * sd.width_total + cur_col];
				}
			}
			
			// If not all the cells we're following are still alive, fill the rest of the slots with -1 for no data
			for (; col < ip.num_colls_print; col++) {
				for (int line = 0; line < sd.height; line++) {
					time_point[line * ip.num_colls_print + (ip.num_colls_print - col - 1)] = -1;
				}
			}
			
			for (int cell = 0; cell < sd.height * ip.num_colls_print; cell++) {
				file_cons << time_point[cell] << " ";
			}
			file_cons << endl;
			time++;
		}
				
		file_cons.close();
	}		
}

/* print_osc_features prints the oscillation features for every mutant of the given run
	parameters:
		ip: the program's input parameters
		file_features: a pointer to the output file stream to open the file with
		mds: an array of the mutant_data structs for every mutant
		set_num: the index of the parameter set whose features are being printed
		num_passed: the number of mutants that passed the parameter set
	returns: nothing
	notes:
		This function sets the precision of file_features to 30, which is higher than the default value.
		This function prints each feature for each mutant, each feature separated by a comma, one set per line.
	todo:
		TODO Print anterior scores.
*/
void print_osc_features (input_params& ip, ofstream* file_features, mutant_data mds[], int set_num, int num_passed) {
	if (ip.print_features) { // Print the features only if the user specified it
		file_features->precision(30);
		try {
			*file_features << set_num << ",";
			for (int i = 0; i < ip.num_active_mutants; i++) {
				*file_features << mds[i].feat.sync_score_post[IMH1] << "," << mds[i].feat.period_post[IMH1] << "," << mds[i].feat.amplitude_post[IMH1] << "," << (mds[i].feat.period_post[IMH1]) / (mds[MUTANT_WILDTYPE].feat.period_post[IMH1]) << "," << (mds[i].feat.amplitude_post[IMH1]) / (mds[MUTANT_WILDTYPE].feat.amplitude_post[IMH1]) << ",";
				*file_features << mds[i].feat.sync_score_ant[IMH1] << "," << mds[i].feat.period_ant[IMH1] << "," << mds[i].feat.amplitude_ant[IMH1] << "," << (mds[i].feat.period_ant[IMH1]) / (mds[MUTANT_WILDTYPE].feat.period_ant[IMH1]) << "," << (mds[i].feat.amplitude_ant[IMH1]) / (mds[MUTANT_WILDTYPE].feat.amplitude_ant[IMH1]) << ",";
			}
			if (num_passed == ip.num_active_mutants) {
				*file_features << "PASSED" << endl;
			} else {
				*file_features << "FAILED" << endl;
			}	
		} catch (ofstream::failure) {
			cout << term->red << "Couldn't write to " << ip.features_file << "!" << term->reset << endl;
			exit(EXIT_FILE_WRITE_ERROR);
		}
	}
}

/* print_conditions prints which conditions each mutant of the given run passed
	parameters:
		ip: the program's input parameters
		file_conditions: a pointer to the output file stream to open the file with
		mds: an array of the mutant data structs for every mutant
		set_num: the index of the parameter set whose conditions are being printed
	returns: nothing
	notes:
		This function prints -1, 0, or 1 for each condition for each mutant, each condition separated by a comma, one set per line.
	todo:
*/
void print_conditions (input_params& ip, ofstream* file_conditions, mutant_data mds[], int set_num) {
	if (ip.print_conditions) { // Print the conditions only if the user specified it
		try {
			*file_conditions << set_num << ",";
			for (int i = 0; i < ip.num_active_mutants; i++) {
				*file_conditions << mds[i].print_name << ",";
				for (int j = 0; j < NUM_SECTIONS; j++) {
					for (int k = 0; k < mds[i].num_conditions[j]; k++) {
						if (mds[i].secs_passed[j]) { // If the mutant was run, print 1 if it passed, 0 if it failed
							*file_conditions << mds[i].conds_passed[j][k] << ",";
						} else { // If the mutant was not run, print -1
							*file_conditions << "-1,";
						}
					}
				}
			}
			*file_conditions << endl;
		} catch (ofstream::failure) {
			cout << term->red << "Couldn't write to " << ip.conditions_file << "!" << term->reset << endl;
			exit(EXIT_FILE_WRITE_ERROR);
		}
	}
}

/* print_scores prints the scores of each mutant run for the given set
	parameters:
		ip: the program's input parameters
		file_scores: a pointer to the output file stream to open the file with
		set_num: the index of the parameter set whose conditions are being printed
		scores: the array of scores to print
		total_score: the sum of all the scores
	returns: nothing
	notes:
		This function prints the set index then score for each mutant then the total score, all separated by commas, one set per line.
	todo:
*/
void print_scores (input_params& ip, ofstream* file_scores, int set_num, double scores[], double total_score) {
	if (ip.print_scores) {
		try {
			*file_scores << set_num << ",";
			for (int i = 0; i < NUM_SECTIONS * ip.num_active_mutants; i++) {
				*file_scores << scores[i] << ",";
			}
			*file_scores << total_score << endl;
		} catch (ofstream::failure) {
			cout << term->red << "Couldn't write to " << ip.scores_file << "!" << term->reset << endl;
			exit(EXIT_FILE_WRITE_ERROR);
		}
	}
}

/* close_if_open closes the given output file stream if it is open
	parameters:
		file: a pointer to the output file stream to close
	returns: nothing
	notes:
	todo:
*/
void close_if_open (ofstream* file) {
	if (file->is_open()) {
		file->close();
	}
}

/* read_pipe reads parameter sets from a pipe created by a program interacting with this one
	parameters:
		sets: the array of parameter sets in which to store any received sets
		ip: the program's input parameters
	returns: nothing
	notes:
		The first information piped in must be an integer specifying the number of rates (i.e. parameters) per set that will be piped in. If this number differs from what is expected, the program will exit.
		The next information piped in must be an integer specifying the number of parameter sets that will be piped in.
		Each set to be piped in should be sent as a stream of doubles, with each set boundary determined by the previously sent number of rates per set.
		This function does not create a pipe; it must be created by an external program interfacing with this one.
	todo:
*/
void read_pipe (double**& sets, input_params& ip) {
	// Read how many rates per set will be piped in
	int num_pars = 0;
	read_pipe_int(ip.pipe_in, &num_pars);
	if (num_pars != NUM_RATES) {
		cout << term->red << "An incorrect number of rates will be piped in! This simulation requires " << NUM_RATES << " rates per set but the sampler is sending " << num_pars << " per set." << term->reset << endl;
		exit(EXIT_INPUT_ERROR);
	}
	
	// Read how many sets will be piped in
	int num_sets = 0;
	read_pipe_int(ip.pipe_in, &num_sets);
	if (num_sets <= 0) {
		cout << term->red << "An invalid number of parameter sets will be piped in! The number of sets must be a positive integer but the sampler is sending " << num_sets << "." << term->reset << endl;
		exit(EXIT_INPUT_ERROR);
	}
	ip.num_sets = num_sets;
	
	// Read every set and store it as an array of doubles
	sets = new double*[num_sets];
	for (int i = 0; i < num_sets; i++) {
		sets[i] = new double[NUM_RATES];
		read_pipe_set(ip.pipe_in, sets[i]);
	}
}

/* read_pipe_int reads an integer from the given pipe and stores it in the given address
	parameters:
		fd: the file descriptor identifying the pipe
		address: the address to store the integer read from the pipe
	returns: nothing
	notes:
		This function assumes that fd identifies a valid pipe end and will exit with an error if it does not.
	todo:
*/
void read_pipe_int (int fd, int* address) {
	if (read(fd, address, sizeof(int)) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_READ_ERROR);
	}
}

/* read_pipe_set reads a parameter set from the given pipe and stores it in the given address
	parameters:
		fd: the file descriptor identifying the pipe
		pars: the array in which to store the set read from the pipe
	returns: nothing
	notes:
		This function assumes that fd identifies a valid pipe end and will exit with an error if it does not.
	todo:
*/
void read_pipe_set (int fd, double pars[]) {
	if (read(fd, pars, sizeof(double) * NUM_RATES) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_READ_ERROR);
	}
}

/* write_pipe writes run scores to a pipe created by a program interacting with this one
	parameters:
		score: the array of scores to send
		ip: the program's input parameters
		sd: the current simulation's data
	returns: nothing
	notes:
		This function assumes that fd points to a valid pipe and will exit with an error if it does not.
	todo:
*/
void write_pipe (double score[], input_params& ip, sim_data& sd) {
	// Write the maximum possible score an single run can achieve to the pipe
	write_pipe_double(ip.pipe_out, (sd.no_growth ? sd.max_scores[SEC_POST] : sd.max_score_all));
	
	// Write the scores to the pipe
	for (int i = 0; i < ip.num_sets; i++) {
		write_pipe_double(ip.pipe_out, score[i]);
	}
	
	// Close the pipe
	if (close(ip.pipe_out) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
}

/* write_pipe_int writes the given integer to the given pipe
	parameters:
		fd: the file descriptor identifying the pipe
		value: the integer to write to the pipe
	returns: nothing
	notes:
		This function assumes that fd points to a valid pipe and will exit with an error if it does not.
	todo:
*/
void write_pipe_double (int fd, double value) {
	if (write(fd, &value, sizeof(double)) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
}


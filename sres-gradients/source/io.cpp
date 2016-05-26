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
io.cpp contains functions for input and output of files and pipes. All I/O related functions should be placed in this file.
*/

#include <cmath> // Needed for log10
#include <sys/wait.h> // Needed for waitpid
#include <unistd.h> // Needed for pipe, read, write, close, fork, execv

#include "io.hpp" // Function declarations

#include "init.hpp"
#include "macros.hpp"
#include "main.hpp"
#include "sres.hpp"

extern terminal* term; // Declared in init.cpp
extern input_params ip; // Declared in main.cpp

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

/* read_file takes an input_data struct and stores the contents of the associated file in a string
	parameters:
		ifd: the input_data struct to contain the file name, buffer to store the contents, size of the file, and current index
	returns: nothing
	notes:
		The buffer in ifd will be sized large enough to fit the file
	todo:
*/
void read_file (input_data* ifd) {
	int rank = get_rank();
	ostream& v = term->verbose();
	term->rank(rank, v);
	v << term->blue << "Reading file " << term->reset << ifd->filename << " . . . ";
	
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
	
	term->done(v);
}

/* parse_ranges_file reads the given buffer and stores every range found in the given ranges array
	parameters:
		buffer: the buffer with the ranges to read
		ip: the program's input parameters
		sp: parameters required by libSRES with arrays in which to store the lower and upper bounds of each range
	returns: nothing
	notes:
		The buffer should contain one range per line, starting the name of the parameter followed by the bracked enclosed lower and then upper bound optionally followed by comments.
		e.g. 'msh1 [30, 65] comment'
		The name of the parameter is so humans can conveniently read the file and has no semantic value to this parser.
		Blank lines and lines starting with # will be ignored. Anything after the upper bound is ignored.
	todo:
*/
void parse_ranges_file (char* buffer, input_params& ip, sres_params& sp) {
	int i = 0;
	int rate = 0;
	for (; buffer[i] != '\0'; i++) {
		// Ensure that the number of rates in the given ranges file does not exceed the given number of dimensions
		if (rate >= ip.num_dims) {
			cout << term->red << "The number of rates in the given ranges file does not match the given number of dimensions! Please check that the rates file matches the number of dimensions (" << ip.num_dims << ")." << term->reset << endl;
		exit(EXIT_INPUT_ERROR);
		}
		
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
		sp.lb[rate] = atof(buffer + i);
		while (buffer[i] != ',') {i++;}
		i++;
		sp.ub[rate] = atof(buffer + i);
		if (sp.lb[rate] < 0 || sp.ub[rate] < 0) { // If the ranges are invalid then set them to 0
			sp.lb[rate] = 0;
			sp.ub[rate] = 0;
		}
		
		// Skip any comments until the end of the line
		while (buffer[i] != '\n' && buffer[i] != '\0') {i++;}
		rate++;
	}
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
		if (index_params != NUM_PARS) {
			cout << term->red << "The given parameter sets file contains sets with an incorrect number of rates! This simulation requires " << NUM_PARS << " per set but at least one line contains " << index_params << " per set." << term->reset << endl;
			exit(EXIT_INPUT_ERROR);
		}
		return true;
	} else if (buffer_line[index_buffer] != '\0') { // There are more lines to try to parse
		return parse_param_line(params, buffer_line, index_buffer);
	} else { // The end of the buffer was found
		return false;
	}
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
	ostream& v = term->verbose();
	try {
		if (append) {
			v << term->blue << "Opening " << term->reset << file_name << " . . . ";
			file_pointer->open(file_name, fstream::app);
		} else {
			v << term->blue << "Creating " << term->reset << file_name << " . . . ";
			file_pointer->open(file_name, fstream::out);
		}
	} catch (ofstream::failure) {
		cout << term->red << "Couldn't write to " << file_name << "!" << term->reset << endl;
		exit(EXIT_FILE_WRITE_ERROR);
	}
	term->done(v);
}

/* simulate_set performs the required piping to setup and run a simulation with the given parameters
	parameters:
		parameters: the parameters to pass as a parameter set to the simulation
	returns: the score the simulation received
	notes:
	todo:
*/
double simulate_set (double parameters[]) {
	// Get the MPI rank of the process
	int rank = get_rank();
	ostream& v = term->verbose();
	
	// Create a pipe
	int pipes[2];
	v << "  ";
	term->rank(rank, v);
	v << term->blue << "Creating a pipe " << term->reset << ". . . ";
	if (pipe(pipes) == -1) {
		term->failed_pipe_create();
		exit(EXIT_PIPE_CREATE_ERROR);
	}
	v << term->blue << "Done: " << term->reset << "using file descriptors " << pipes[0] << " and " << pipes[1] << endl;
	
	// Fork the process so the child can run the simulation
	v << "  ";
	term->rank(rank, v);
	v << term->blue << "Forking the process " << term->reset << ". . . ";
	pid_t pid = fork();
	if (pid == -1) {
		term->failed_fork();
		exit(EXIT_FORK_ERROR);
	}
	
	int child_pid;
	if (pid == 0) {
		child_pid = getpid();
	} else {
		child_pid = pid;
		v << term->blue << "Done: " << term->reset << "the child process's PID is " << child_pid << endl;
	}
	int rank_strlen = INT_STRLEN(rank);
	char* grad_fname = (char*)mallocate(sizeof(char) * (strlen("input-.gradients") + rank_strlen + 1));
	sprintf(grad_fname, "input-%d.gradients", rank);
	
	if (pid == 0) {
		char** sim_args = copy_args(ip.sim_args, ip.num_sim_args);
		store_pipe(sim_args, ip.num_sim_args - 6, pipes[0]);
		store_pipe(sim_args, ip.num_sim_args - 4, pipes[1]);
		sim_args[ip.num_sim_args - 2] = grad_fname;
		
		ofstream grad_file;
		v << "  ";
		term->rank(rank, v);
		open_file(&grad_file, grad_fname, false);
		if (ip.base_gradients != NULL) {
			grad_file << ip.base_gradients << "\n";
		}
		int loc_start = parameters[0];
		int loc_end = parameters[1];
		int val = parameters[2];
		gradient_index* gi = ip.gradient_indices;
		while (gi != NULL) {
			grad_file << gi->index << " (" << loc_start << " 100) (" << loc_end << " " << val << ")\n";
			gi = gi->next;
		}
		grad_file.close();
		
		v << "  ";
		term->rank(rank, v);
		v << term->blue << "Checking that the simulation file exists and can be executed " << term->reset << ". . . ";
		if (access(ip.sim_file, X_OK) == -1) {
			term->failed_exec();
			exit(EXIT_EXEC_ERROR);
		}
		term->done(v);
		if (execv(ip.sim_file, sim_args) == -1) {
			term->failed_exec();
			exit(EXIT_EXEC_ERROR);
		}
	} else {
		v << "  ";
		term->rank(rank, v);
		v << term->blue << "Writing to the pipe " << term->reset << "(file descriptor " << pipes[1] << ") . . . ";
		write_pipe(pipes[1], ip.sets, ip.num_sets);
		term->done(v);
	}
	
	// Wait for the child to finish simulating
	int status = 0;
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status) == 0) {
		term->failed_child();
		exit(EXIT_CHILD_ERROR);
	}
	
	// Close the writing end of the pipe
	if (close(pipes[1]) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
	
	// Pipe in the simulation's score
	int max_score;
	int scores[ip.num_sets];
	memset(scores, 0, sizeof(int) * ip.num_sets);
	v << "  ";
	term->rank(rank, v);
	v << term->blue << "Reading the pipe " << term->reset << "(file descriptor " << pipes[0] << ") . . . ";
	read_pipe(pipes[0], &max_score, scores, ip.num_sets);
	v << term->blue << "Done: " << term->reset << "(raw score of set 0: " << scores[0] << " / " << max_score << ")" << endl;
	
	// Close the reading end of the pipe
	v << "  ";
	term->rank(rank, v);
	v << term->blue << "Closing the reading end of the pipe " << term->reset << "(file descriptor " << pipes[0] << ") . . . ";
	if (close(pipes[0]) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
	term->done(v);
	
	// Remove the gradient file
	v << "  ";
	term->rank(rank, v);
	v << term->blue << "Removing " << term->reset << grad_fname << " . . . ";
	if (remove(grad_fname) != 0) {
		term->failed_file_remove(grad_fname);
		exit(EXIT_FILE_REMOVE_ERROR);
	}
	mfree(grad_fname);
	term->done(v);
	
	// Calculate the average score of all the runs
	int avg_score = 0;
	for (int i = 0; i < ip.num_sets; i++) {
		avg_score += scores[i];
	}
	avg_score /= ip.num_sets;
	
	// libSRES requires scores from 0 to 1 with 0 being a perfect score so convert the simulation's score format into libSRES's
	return 1 - ((double)avg_score / max_score);
}

/* print_good_set prints the given parameter set if its score is good enough
	parameters:
		parameters: the set of parameters run
		score: the received score
	returns: nothing
	notes:
	todo:
*/
void print_good_set (double parameters[], double score) {
	if (ip.print_good_sets && score <= ip.good_set_threshold) {
		cout << term->blue << "  Found a good set " << term->reset << "(score " << score << ")" << endl;
		ip.good_sets_stream << score << "," << parameters[0];
		for (int i = 1; i < ip.num_dims; i++) {
			ip.good_sets_stream << "," << parameters[i];
		}
		ip.good_sets_stream << endl;
	}
}

/* write_pipe writes the given parameter set to the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		parameters: the parameter set to pipe
	returns: nothing
	notes:
	todo:
*/
void write_pipe (int fd, double* sets[], int num_sets) {
	write_pipe_int(fd, NUM_PARS);
	write_pipe_int(fd, num_sets);
	for (int i = 0; i < num_sets; i++) {
		if (write(fd, sets[i], sizeof(double) * NUM_PARS) == -1) {
			term->failed_pipe_write();
			exit(EXIT_PIPE_WRITE_ERROR);
		}
	}
}

/* write_pipe_int writes the given integer to the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		value: the integer to pipe
	returns: nothing
	notes:
	todo:
*/
void write_pipe_int (int fd, int value) {
	if (write(fd, &value, sizeof(int)) == -1) {
		term->failed_pipe_write();
		exit(EXIT_PIPE_WRITE_ERROR);
	}
}

/* read_pipe reads the maximum score and the received score from the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		max_score: a pointer to store the maximum score the simulation could have received
		score: a pointer to store the score the simulation actually received
	returns: nothing
	notes:
	todo:
*/
void read_pipe (int fd, int* max_score, int scores[], int num_sets) {
	read_pipe_int(fd, max_score);
	for (int i = 0; i < num_sets; i++) {
		read_pipe_int(fd, &(scores[i]));
	}
}

/* read_pipe_int writes an integer from the given pipe
	parameters:
		fd: the file descriptor of the pipe to write to
		address: a pointer to store the received integer
	returns: nothing
	notes:
	todo:
*/
void read_pipe_int (int fd, int* address) {
	if (read(fd, address, sizeof(int)) == -1) {
		term->failed_pipe_read();
		exit(EXIT_PIPE_READ_ERROR);
	}
}

/* close_if_open closes the given output file stream if it is open
	parameters:
		file: a pointer to the output file stream to close
	returns: nothing
	notes:
	todo:
*/
void close_if_open (ofstream& file) {
	if (file.is_open()) {
		file.close();
	}
}


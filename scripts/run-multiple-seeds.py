"""
Tests parameter sets for robustness by running them with multiple seeds
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
"""

import sys
import shared
import subprocess

def main():
	#check the given arguments
	print "Reading command-line arguments..."
	args = sys.argv[1:]
	num_args = len(args)
	req_args = [False] * 6
	num_seeds = 0
	sim_arguments = ""

	if num_args >= 6:
		for arg in range(0, num_args - 1, 2):
			option = args[arg]
			value = args[arg + 1]
			if option == '-i' or option == '--input-file':
				ifile = value
				req_args[0] = True
			elif option == '-n' or option == '--num-params':
				num_params = shared.toInt(value)
				req_args[1] = True
			elif option == '-p' or option == '--pars-per-job':
				pars_per_job = shared.toInt(value)
				req_args[2] = True
			elif option == '-d' or option == '--directory':
				folder = value
				req_args[3] = True
			elif option == '-s' or option == '--simulation':
				simulation = value
				req_args[4] = True
			elif option == '-S' or option == '--seeds':
				num_seeds = int(value)
				req_args[5] = True
			elif option == '-a' or option == '--arguments':
				for a in range(arg + 1, num_args):
					sim_arguments += ' ' + args[a]
				break
			elif option == '-h' or option == '--help':
				usage()
			else:
				usage()
		for arg in req_args:
			if not arg:
				req_args
				usage()
	else:
		usage()
	
	index = 0
	
	input_file = shared.openFile(ifile, "r")
	shared.ensureDir(folder)
	for parset in range(0, num_params, pars_per_job):
		params = shared.openFile(folder + "/input" + str(index) + ".params", "w")
		for line in range(pars_per_job):
			params.write(input_file.readline())
		params.close()
		index += 1;
	
	for seeds in range(num_seeds):
		seed = (seeds + 1) * 1000
		for parset in range(index):
			job = shared.openFile(folder + "/pbs-job-" + str(seed) + "-" + str(parset), 'w')
			job.write('''
#PBS -N robust-test 
#PBS -l nodes=1:ppn=1
#PBS -l mem=500mb
#PBS -l file=300mb
#PBS -q biomath
#PBS -j oe
#PBS -o ''' + folder + '''/output''' + str(seed) + "-" + str(parset) + '''.txt
#PBS -l walltime=06:00:00

cd $PBS_O_WORKDIR

''' + simulation + ' ' + sim_arguments + ' -p ' + str(pars_per_job) + ' -i ' + ifile + ''' -s ''' + str(seed) + " -M 6 -E " + folder + "/scores-" + str(seed) + "-" + str(parset) + ".csv")
			job.close()
			subprocess.call(["qsub", folder + "/pbs-job-" + str(seed) + "-" + str(parset)])

def usage():
	print 'Usage: python robust-distributed.py (-short_option value | --long_option value)...'
	print '-i, --input-file   [filename]  : the relative filename of the parameter sets to run the simulation with, required'
	print '-n, --num-params	  [int]       : the number of parameters to be tested for robustness, required'
	print '-p, --pars-per-job [int]       : the number of parameters to be run for each job, required'
	print '-d, --directory    [directory] : the relative path of the folder in which all the output files will be placed, required'
	print '-s, --simulation   [filename]  : the relative filename of the simulation to run, required'
	print '-S, --seeds        [int]       : the number of seeds to run the simulation for, required'
	print '-a, --arguments    [N/A]       : every argument following this will be sent to the simulation'
	print '-h, --help         [N/A]       : view usage information (i.e. this)'
	exit(0)

main()


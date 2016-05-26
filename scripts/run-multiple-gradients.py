"""
Brute-force searches different gradient configurations
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
import subprocess
import shared
import time

def get_num_jobs():
	wc = subprocess.Popen(["qstat"], stdout=subprocess.PIPE).communicate()[0]
	if wc != "":
		num_jobs = wc.count("\n")
	else:
		num_jobs = 0
	return num_jobs

def main():
	gradients = {0: ['28', '29'], 1: ['37'], 2: ['41'], 3: ['34', '35'], 4: ['38', '39'], 5: ['4', '5'], 6: ['34', '35', '37'], 7: ['38', '39', '41'], 8: ['4', '5', '7'], 9: ['28', '32', '15']}
	end_rate = [-1, 1001, 1001, 1001, 1001, -1, 1001, 1001, -1, -1]
	incs = [-1, 1, 1, 1, 1, -1, 1, 1, -1, -1]

	print 'Reading command-line arguments...'
	args = sys.argv[1:] # Remove the name of the program from the arguments
	num_args = len(args)
	num_req_args = 2
	req_args = [False] * num_req_args # If every required argument was given then req_args will be all true
	if num_args >= num_req_args:
		# Arguments with default values
		base_gradients_file = None
		simulation = './simulation'
		sim_arguments = ""

		for arg in range(0, num_args - 1, 2):
			option = args[arg]
			value = args[arg + 1]
			if option == '-s' or option == '--sets':
				sets_fname = value # Parameter sets file
				req_args[0] = True
			elif option == '-o' or option == '--output-dir':
				odir = value
				req_args[1] = True
			elif option == '-g' or option == '--base-gradients':
				base_gradients_file = shared.openFile(value, 'r') # Gradients to add to every simulation on top of the changing ones
			elif option == '-S' or option == '--simulation':
				simulation = value # Filename of the simulation
			elif option == '-a' or option == '--arguments':
				for a in range(arg + 1, num_args): # Arguments to pass to the simulation
					sim_arguments += ' ' + args[a]
				break
			else:
				usage()
		for arg in req_args: # Check to ensure every required argument was entered
			if not arg:
				usage()
	else:
		usage()

	print 'Reading the base gradients file...'
	# Add any given base gradients to a string to be passed to each simulation's gradients file
	if base_gradients_file is None:
		base_gradients = ''
	else:
		base_gradients = ''
		for line in base_gradients_file:
			base_gradients += line
	
	print 'Start creating gradient files and pbs-jobs for each gradient combination'
	start_point = '9'
	end_point = '49'

	# For every gradient option
	num_gradients = len(gradients)
	jobs_to_submit = []
	for i in range(num_gradients):
		si = str(i)
		for j in range(100, end_rate[i], incs[i]):
			sj = str(j)
			grad_id = '-' + si + '-' + sj
			grad_fname = odir + "/grad" + grad_id + ".gradient"
			scores_fname = odir + "/scores" + grad_id + ".csv"
			subprocess.call(["mkdir", "-p", odir])
			grad_file = shared.openFile(grad_fname, 'w')

			# Write the base gradient to the file
			grad_file.write(base_gradients)
			for rate in gradients[i]:
				grad_file.write(rate + ' (' + start_point + ' 100) (' + end_point + ' ' + sj + ')\n')

			# Create the PBS job file
			pbs_fname = odir + '/pbs-job' + grad_id
			jobs_to_submit.append(pbs_fname)
			pbs_file = shared.openFile(pbs_fname, 'w')
			pbs_file.write("""
#PBS -N gradient-run""" + grad_id + """
#PBS -l nodes=1:ppn=1
#PBS -l mem=500MB
#PBS -l file=500MB
#PBS -q biomath
#PBS -j oe
#PBS -o output.txt
#PBS -l walltime=360:00:00

cd $PBS_O_WORKDIR\n""")
	
			# Write the simulation call to the PBS job file
			pbs_file.write(simulation + sim_arguments + ' -i ' + sets_fname + ' -r ' + grad_fname + ' -E ' + scores_fname)
			pbs_file.close()
			if j % 50 == 0:
				queue_jobs = get_num_jobs()
				# Add parsing of queue_jobs
				while (queue_jobs > 150):
					time.sleep(300)
					queue_jobs = get_num_jobs()
				for job in range(min(100, len(jobs_to_submit))):
					print ("Submitting job " + jobs_to_submit[job])
					subprocess.call(['qsub', jobs_to_submit[job]])
				jobs_to_submit = jobs_to_submit[100:]
				
def usage():
	print 'Usage: python gradients.py (-short_option value | --long_option value)...'
	print '-s, --sets           [filename]   : the relative filename of the parameter sets to run the simulations with, required'
	print '-g, --base-gradients [filename]   : the relative filename of the gradients to run each simulation on top of the changing ones, default=none'
	print '-S, --simulation     [filename]   : the relative filename of the simulation to run, default=simulation'
	print '-a, --arguments      [N/A]        : every argument following this will be sent to the simulation'
	exit(0)

main()


"""
Sensitivity Analysis plotting progam, designed for use with the Sensitivity Analysis package that is part of the Deterministic simulator for zebrafish segmentation.
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
import matplotlib as mpl
mpl.use('Agg')

import numpy
import scipy
import matplotlib.pylab as pl
import time
from time import clock
from scipy.stats.stats import pearsonr
import sys
import shared
import subprocess
import struct
import math

JOB_NAME = '''PLOT_SA'''
Y_BOUNDS = None
#Putting these parameter names for the 45 parameter simulation runs here for convenience. We may want a general way of getting these labels onto the graphs.
PARAM_NAMES = ["MSH1" , " MSH7" , " MSH13" , " MSDELTA" , " MDH1 " , " MDH7 " , " MDH13" , " MDDELTA" , " PSH1" , " PSH7" , " PSH13" , " PSDELTA" , " PDH1" , " PDH7" , " PDH13" , " PDDELTA" , " DAH1H1" , " DAH1H7" , " DAH1H13" , " DAH7H7" , " DAH7H13" , " DAH13H13" , " DDIH1H1" , " DDIH1H7" , " DDIH1H13" , " DDIH7H7" , " DDIH7H13" , " DDIH13H13" , " DDGH1H1" , " DDGH1H7" , " DDGH1H13" , " DDGH7H7" , " DDGH7H13" , " DDGH13H13" , " DELAYMH1" , " DELAYMH7" , " DELAYMH13" , " DELAYMDELTA" , " DELAYPH1" , " DELAYPH7" , " DELAYPH13" , " DELAYPDELTA" , " CRITPH1H1" , " CRITPH7H13" , " CRITPDELTA"]


def main():
	# Default argument values.
	global Y_BOUNDS
	global JOB_NAME
	global PARAM_NAMES
	nodes = 1
	feature = -1
	points = 4
	percent = 20
	file_out = "sensitivity_graphs"
	nominal_file = "../sensitivity-analysis/nominal.params"
	data_dir = "../sensitivity-analysis/sense-for-plot"
	image_dir = "plots"
	perturb_file = "../simulation/input.perturb"
	nominal_count = 1
	ex_path = "../sensitivity-analysis/sensitivity"
	sim_path = "../simulation/simulation"
	ppn = "2"
	graph = False
	elasticity = False
	sim_args = " "
	additional_args = " "
	cname = None
	
	#Check the commmand line input:
	if len(sys.argv) < 2 or ("-h" in sys.argv) or ("--help" in sys.argv):
		usage()
	ishift = 0
	for i in range(1, len(sys.argv), 2):
		i += ishift
		if i+1 >= len(sys.argv):
			usage()
		#elif sys.argv[i] == "-j" or sys.argv[i] == "--job-name":
		#	global JOB_NAME = sys.argv[i+1]
		elif sys.argv[i] == "-n" or sys.argv[i] == "--nominal-file":
			nominal_file = sys.argv[i+1]
		elif sys.argv[i] == "-c" or sys.argv[i] == "--nominal-count":
			nominal_count = shared.toInt(sys.argv[i+1])
		elif sys.argv[i] == "-p" or sys.argv[i] == "--percent":
			percent = shared.toInt(sys.argv[i+1])
		elif sys.argv[i] == "-P" or sys.argv[i] == "--Points":
			points = shared.toInt(sys.argv[i+1])
		elif sys.argv[i] == "-l" or sys.argv[i] == "--ppn":
			ppn = sys.argv[i+1]
		elif sys.argv[i] == "-N" or sys.argv[i] == "--nodes":
			nodes = shared.toInt(sys.argv[i+1])
		elif sys.argv[i] == "-f" or sys.argv[i] == "--feature":
			feature = shared.toInt(sys.argv[i+1])
		elif sys.argv[i] == "-e" or sys.argv[i] == "--exec":
			ex_path = sys.argv[i+1]
		elif sys.argv[i] == "-s" or sys.argv[i] == "--sim":
			sim_path = sys.argv[i+1]
		elif sys.argv[i] == "-o" or sys.argv[i] == "--output":
			file_out = sys.argv[i+1]
		elif sys.argv[i] == "-d" or sys.argv[i] == "--dir":
			image_dir = sys.argv[i+1]
		elif sys.argv[i] == "-D" or sys.argv[i] == "--data-dir":
			data_dir = sys.argv[i+1]
		elif sys.argv[i] == "-j" or sys.argv[i] == "--job-name":
			JOB_NAME = sys.argv[i+1]
		elif sys.argv[i] == "-C" or sys.argv[i] == "--cluster-name":
			cname = sys.argv[i+1]
		elif sys.argv[i] == "--ymin":
			val = float(sys.argv[i+1])
			if Y_BOUNDS == None:
				Y_BOUNDS = (val , max(1.5, 2*val) )
			else:
				Y_BOUNDS[0] = val
		elif sys.argv[i] == "--ymax":
			val = float(sys.argv[i+1])
			if Y_BOUNDS == None:
				Y_BOUNDS = (min(0, 2*val) , val)
			else:
				Y_BOUNDS[1] = val
		elif sys.argv[i] == "-E" or sys.argv[i] == "--elasticity":
			elasticity = True
			ishift = -1
		elif sys.argv[i] == "-g" or sys.argv[i] == "--graph":
			graph = True
			ishift = -1
		elif sys.argv[i] == "-a" or sys.argv[i] == "--args":
			for a in sys.argv[i+1:]:
				additional_args += " " + a + " "
			break
			
	#Ensure that the necessary directories exist -- if not, make them.
	shared.ensureDir(data_dir)
	shared.ensureDir(image_dir)
	
	#Additional args is a string that that is attached to the final arguments sent to the sensitivity analysis program.
	additional_args = " -p " + str(percent) + " -P " + str(points) + " " + additional_args
	
	#Depending on whether elasticity is chosen, either create sensitivity bar graphs or scatter-line plots. 
	if(not elasticity):
		#This statement checks to see if simulations actually need to be run. This is mostly true, but if all the data has been created already then '-g' will cause the script to skip to the plotting.
		if(not graph):
			#dispatch_jobs takes care of running the program locally or making the pbs jobs.
			dispatch_jobs(nodes, file_out, nominal_file , data_dir , image_dir , perturb_file, nominal_count, 0, ex_path, sim_path, ppn, sim_args , None, additional_args, cname)
			print "\t~ Done with runs ~"
		
		#Once the data has been collected, load it in and make the graphs.
		print "\t ~ Generating graphs ~ "
		#Load all of the data from the sensitivity results. 
		#This uses "/normalized_[number]" as the file name because that is how it's set in sensitivity-analysis/init.hpp.
		#The struct input_params has two strings, norm_file and sense_file, that determine the names of specific files to load. 
		#These could be specified more generally by making a new commandline argument for the sensitivity executible, but this has not seemed necessary because there is already so much customization of the directies these files end up in.
		data = []
		names = []
		for i in range(nominal_count):
			temp_data, names = parse_files(data_dir+"/normalized_"+str(i))
			data.append(temp_data)
	
		#If just one feature is specified, this makes just one graph. Otherwise it loops through all features and makes a graph for each.
		bar_data = [] 	#This holds onto the data that was actually plotted, i.e. average sensitivity values for each parameter.
		bar_error = []	#This holds onto the standard error for each parameter.
		if feature > 0:
			temp_sense, temp_error = sense_bar(data, image_dir, feature, feat_name = names[feature])
			bar_data.append(temp_sense)
			bar_error.append(temp_error)
		else:
			sys.stdout.write("Done with normalized graphs: ")
			sys.stdout.flush()
			for i in range(len(data[0][0])):
				temp_sense, temp_error = sense_bar(data, image_dir, i, feat_name = names[i])
				bar_data.append(temp_sense)
				bar_error.append(temp_error)				
				sys.stdout.write(str(i) + "... ")
				sys.stdout.flush()
		#Write out the bar graph data to file
		write_bar_data(bar_data, bar_error, data_dir+"/bar_graph_data_normalized.csv", ynames=names, xnames=PARAM_NAMES)
		
		#Abosulte sensitivity graphs
		#Similarly, this uses "/LSA_[number]" as the file name because that is how it's set in sensitivity-analysis/init.hpp.
		data = []
		names = []
		for i in range(nominal_count):
			temp_data, names = parse_files(data_dir+"/LSA_"+str(i))
			data.append(temp_data)
	
		#If just one feature is specified, this makes just one graph. Otherwise it loops through all features and makes a graph for each.
		bar_data = [] 	#This holds onto the data that was actually plotted, i.e. average sensitivity values for each parameter.
		bar_error = []	#This holds onto the standard error for each parameter.
		if feature > 0:
			temp_sense, temp_error = sense_bar(data, image_dir, feature, feat_name = names[feature], normal=False)
			bar_data.append(temp_sense)
			bar_error.append(temp_error)
		else:
			sys.stdout.write("Done with absolute graphs: ")
			sys.stdout.flush()
			for i in range(len(data[0][0])):
				temp_sense, temp_error = sense_bar(data, image_dir, i, feat_name = names[i], normal=False)
				bar_data.append(temp_sense)
				bar_error.append(temp_error)
				sys.stdout.write(str(i) + "... ")
				sys.stdout.flush()	
		#Write out the bar graph data to file
		write_bar_data(bar_data, bar_error, data_dir+"/bar_graph_data_absolute.csv", ynames=names, xnames=PARAM_NAMES)
	
	#If the elasticity option was included, the following code makes scatter plots of the oscillation features data at different perturbations of each nominal parameter.
	else:
		#This adds a commandline argument that is passed to the sensitivity analysis program to tell it to gather the data without caluclating the sensitivity.
		additional_args = " --generate-only " + additional_args
		#Note that for the elasticity/scatter-line plots each instance of sensitivity used to gather the data is given only one parameter set to ensure data files will be unique (and not get overwritten). This makes it slower than the sensitivity graphs.
		print "\n\t ~ Elasticity data collection ~ "
		data = [] 		# this will be a four dimensional list indexed by: data[which nominal set][which parameter][which perturbation amount][which oscillation feature value]
		names = []	
		nominal = []	# this will be a three dimensional list indexed by: nominal[which nominal set][0][which oscillation feature], the middle index is zero because there is only one parameter set in the nominal features file.
		
		#This loop runs if the data needs to be collected. There are some unintuitive additions that are used to keep track of how many jobs should be sent out the index of which nominal parameter set to use.	 
		if not graph:
			disp = 0		# a counter used to keep track of how many jobs to dispatch.
			raw_data_dirs = [] #A list that gets filled with file name strings which get passed as arguments to the sensitivity program. 
			for c in range(0, nominal_count):
				raw_data_dirs.append(data_dir +"/elastic_data_" + str(c))
				disp += 1
				if disp == nodes or c == nominal_count - 1:
					dispatch_jobs(disp, file_out, nominal_file , data_dir , image_dir , perturb_file, disp, c-disp+1, ex_path, sim_path, ppn, sim_args , raw_data_dirs,  additional_args, cname)
					raw_data_dirs = []
					disp = 0
		
		#Now that the data files exist, load them and parse them into the appropriate arrays. 
		#The "/dim_[number]" and "/nominal_0" strings are the file names that the sensitivity analysis program uses to distinguish output features files. 
		#Modifying these file names would require changing nom_file and dim_file in the constructor of input_params in sensitivity-analysis/init.hpp.
		#This loop is similar to the above, but simpler -- for every nominal parameter set it opens its /elastic_data_[number] directory, parses the files in it and stores the data in data[number] and nominal[number]
		for c in range(0, nominal_count):
			data.append([])
			for d in range(44):
				temp_data, names = parse_files(data_dir +"/elastic_data_" + str(c) + "/dim_" + str(d))
				data[c].append(temp_data)
			temp_data , names = parse_files(data_dir +"/elastic_data_" + str(c) + "/nominal_0")
			nominal.append(temp_data)
			
		#data[] and nominal[] should have everything we need for the graphs now, so plot them.
		print "\n\t ~ Elasticity graphing ~ "
		sys.stdout.write("Done with parameter: ")
		sys.stdout.flush()
		
		#Loop through each parameter 
		for p in range(len(data[0])):
			#Loop through each feature
			for f in range(len(data[0][0][0])):
				#Plot!
				line_plot(data, nominal, p, f, names[f], PARAM_NAMES[p], image_dir, percent, points)
			sys.stdout.write(str(p) + "...")
			sys.stdout.flush()
	print "\n\t ~ Graphs complete ~ "
	return	

#This method is for dispatching jobs to either the local machine or the cluster and waiting on the results. Most of the arguments are from the command line, but some are customized by main() to give different amounts of paallelization.
def dispatch_jobs(nodes,  file_out, nominal_file , data_dir , image_dir , perturb_file, nominal_count, nominal_shift, ex_path, sim_path, ppn, sim_args, raw_data_dirs, additional_args, cname):
	#Put together the structure of the arguments to be passed
	sense_args = ex_path + " -n " + nominal_file + " -d " +  data_dir + " -l " + ppn +  " -e " + sim_path 
	
	#A value to know how many nominal sets to send per qsub job.
	per_node = nominal_count/nodes;
	#A list to hold all of the Popen classes so that they can be waited on.
	popens = []
	
	#Make the call to run the simulations and hold on to the Popen instances for waiting.
	#The sense_args + '-c' etc is to pass the correct nominal counter to sensitivity analysis if the work is to be spread accross multiple nodes.
	#raw_data_dirs is used if the call to function specified the name of the directory where the oscillation features data should be stored.
	for i in range(nodes - 1):
		run_args = sense_args + " -c " + str(per_node) + " -k " + str(i * per_node + nominal_shift) 
		if (raw_data_dirs != None):
			run_args += " -D " + raw_data_dirs[i]
		run_args += " " + additional_args + " "
		popens.append(run_sensitivity( run_args, sim_args, ppn, i, cluster_name=cname ))
	
	#We can't guarantee that the number of nodes evenly divides the number of nominal sets, so this gives the last instance of the simulation program its fair share plus the remainder. 
	excess = nominal_count % nodes
	run_args = sense_args + " -c " + str(per_node + excess) + " -k " + str(nominal_count-per_node-excess +nominal_shift)
	if (raw_data_dirs != None):
		run_args += " -D " + raw_data_dirs[(nodes-1)]
	run_args += " " + additional_args + " "
	popens.append( run_sensitivity(run_args, sim_args, ppn, nodes-1, cluster_name = cname) )

	#Wait for all of the processes that were called by run_sensitivity() to finish.
	for i in range(len(popens)):
		popens[i].wait()
		
	#If using multiple nodes, the above wait only checks that the 'qsub' system calls finished.
	#To check whether simulations are still running on nodes, a check of 'qstat' needs to be done to see if any JOB_NAME instances are running. 
	#The following code handles getting the output of 'qstat', searching for instances of JOB_NAME, and delivering updates to the user when jobs have changed status or finished.
	outstr = "" #String that will hold on to most recent 'qstat' output.
	times = 0	#A counter for how many times the loop has slept while waiting for 'qstat' output to change.
	total_sec = 0	#A counter of seconds to give the user some run time information.
	rewind = 7	#A counter that increases as 'qstat' output remains unchanged, used to put the scipt to sleep longer as the simulations take more time.
	#This loop just keeps putting this script to sleep while there are still simulations running on nodes.  Note that this never runs if the simulation is peformed locally, i.e. if cname == None.
	qjobs = True #A boolean that makes the loop continue or cease.
	while(qjobs and cname != None):
		if(times % rewind == 0):
			stat = subprocess.Popen(["qstat"], stdout=subprocess.PIPE)
			tstr, errstr = stat.communicate()
			if(errstr != None):
				print "\t~ Error ~\t\n" + errstr
			if(len(tstr) != len(outstr) or tstr.count('R') != outstr.count('R') ):
				#If 'qstat' output has changed, give the user an update of how their jobs are doing.
				pstr = tstr[0: tstr.find("\n")]+"\n" + tstr[max(0, ( tstr[0:tstr.find(JOB_NAME)].rfind("\n") ) ): max(-1, (tstr.rfind(JOB_NAME) + tstr[tstr.rfind(JOB_NAME):].find("\n")))]
				rewind = 3
				print "\n\t~ Queue Update -- Running Time = " + str( (total_sec)/60 ) + ":" + str( (total_sec)%60 ) +" ~\n" + pstr
				sys.stdout.write("Running ")
				sys.stdout.flush()
			else:
				rewind = min(rewind + 1, 30)							
			outstr = tstr
			qjobs = (JOB_NAME in outstr )
		#Writing out some dots to keep the user sane.
		subprocess.check_call(["sleep", str(2 + times%rewind)])
		total_sec += 2 + times%rewind
		times += 1	
		sys.stdout.write(" .")
		sys.stdout.flush()
		
	#Jobs are all finished!
	return


#This function make a call to the simulation program and returns an instance of the subprocess.Popen class.
#If you set cluster = True in the funciton call, this will create a pbs job and call qsub to run it.
#Problem fix: had to include pbs_count to avoid race condition with qsub instances trying to read+write the same file.
def run_sensitivity( sense_args, sim_args, ppn, pbs_count,  cluster_name = "biomath"):
	if " -a " not in sense_args:
		sense_args += " -a "
	sense_args += sim_args
	
	if cluster_name == None:
		return subprocess.Popen(space_split(sense_args))
	else:
		memory_limit = "3GB"
		disk_limit = "500MB"	
		wall_time = "24:00:00"
		job = shared.openFile("pbs-job-"+str(pbs_count), "w")
		job.write('''
#PBS -N ''' + JOB_NAME + ''' 
#PBS -l nodes=1:ppn='''+str(ppn)+'''
#PBS -l mem=''' + memory_limit + '''
#PBS -l file=''' + disk_limit + '''
#PBS -q ''' + cluster_name + '''
#PBS -j oe
#PBS -o sensitivity_'''+str(pbs_count)+'''.output
#PBS -l walltime='''+wall_time+'''
cd $PBS_O_WORKDIR

''' + sense_args + '''\n''')
		job.close()
		return subprocess.Popen(["qsub", "pbs-job-"+str(pbs_count)])

#A function that is basically string.split() except it won't ever make a space be an item.
def space_split(s):
	s_list  = []
	next_str = ""
	for c in s:
		if c in " \t":
			if next_str not in (" "*50)+("\t"*10):
				s_list.append(next_str)
			next_str = ""
		else:
			next_str += c
	if next_str != "" and (next_str not in (" "*50)+("\t"*10)):
		s_list.append(next_str)
	return s_list
		 
#A function designed to parse the files that the sensitivity analysis program outputs. 
#This returns a two dimensional array whose rows are which parameter set and whose columns are feauture values. It also returns a one dimensional array of strings that are the feature names.
def parse_files(file_in):
	# open the input file and ensure the directory exists
	fin = shared.openFile(file_in, "r")
	
	# split the lines to get data
	data = []
	names = []
	for line in fin:
		#print line
		if names == []:
			names = line.split(',')[1:]
		data += [line.split(',')[:-1] ]
	
	#Number of parameters is the number of lines, minus one for the name line at the top
	params = len(data) - 1 
	#Number of sensitivity values is the length of any one line array, minus one for the "PASSED" or "FAILED" message, and minus one for the set number
	features = len(data[1]) - 2 
	#Create the sense array that will actually contain the data.
	sense = numpy.zeros(shape = (params, features))
	
	#Put the sensitivity values into matrix. The +1's take into account that the indexing skips the first item in each loop.
	for i in range(1, params+1 ):
		for j in range(1, features + 1):
			num = data[i][j]
			if ("FAILED" in num) or ("PASSED" in num): #This shouldn't ever be true, but just in case...
				num = 0
			num = num_check(float(num))
			sense[i-1][j-1] = num

	# close the file
	fin.close()
	return sense, names

#This function takes care of averaging the sensitivity values accross parameter sets and finding the standard deviation and standard error.
#It then passes the relevent data to make_bar which performs the actual plotting.
#Note: std dev = sqrt( ( (mean - y[0])**2 + (mean- y[1])**2 ) + ... + (mean - y[n])**2 )/n) 		
def sense_bar(data, out_dir, feature=0, feat_name = None, normal=True):
	param_num = len(data[0])
	num_nominal = len(data)
	stdev = [0]*param_num
	senses = [0]*param_num
	#Store all the mean sensitivity across each set for each parameter
	for n in range(num_nominal):
		for p in range(param_num):
			senses[p] += float(data[n][p][feature])/float(num_nominal)
	#Calculate the sum of sqares of differences from the mean
	for n in range(num_nominal):
		for p in range(param_num):
			stdev[p] += (data[n][p][feature] - senses[p])**2
	#Divide to get the average and take the square root to get standard deviations
	for p in range(param_num):
		stdev[p]= math.sqrt(stdev[p]/float(num_nominal))
	
	#Perform actual plotting

	global PARAM_NAMES
	if feat_name == None:
		feat_name = "feature_" + str(feature)
	else:
		feat_name = sanitize(feat_name)
	if normal:
		#bound =(0, max(10, max(senses)+max(stderror) ) ) 
		make_bar(range(param_num), senses, stdev, feat_name, out_dir +"/"+feat_name, ylabel="Normalized Sensitivity (%)", xlabel="System Parameter", xnames = PARAM_NAMES)
	else:
		#bound = ( min(senses)-max(stderror) , max(senses)+max(stderror) )
		make_bar(range(param_num), senses, stdev, feat_name, out_dir +"/absolute_"+feat_name, ylabel="Absolute Sensitivity", xlabel="System Parameter", xnames = PARAM_NAMES)
	return senses, stdev


#This function does the bar graph plotting. If you would like to modify the aesthtic qualities of the bar graphs, here is the place to do it.
def make_bar(xvals, yvals, errorvals, fig_name, out_file, ylabel="Level", xlabel="Parameter", xnames=None):
	global Y_BOUNDS
	wid = 1.5
	fig = pl.figure()
	
	#If the Y_BOUNDS global variable was not set, then use autoscaling. Otherwise, fit the graph to those bounds.
	if Y_BOUNDS == None:
		ax = fig.add_subplot(111, autoscaley_on=True)
	else:
		ax = fig.add_subplot(111, ybound = Y_BOUNDS, autoscaley_on=False)			
	ticks = []
	colors = []
	for i in range(len(xvals)):
		ticks.append(xvals[i]*wid)
		colors.append([ min(1.0, abs(float(errorvals[i])/max(.00001,abs(yvals[i])) )), .4, .6]) # These colors code based on the error bar: the higher the ratio between error and bar height, the redder the bar.
	ax.bar(ticks, yvals, width=(wid -0.1), bottom=0, color = colors, yerr=errorvals, ecolor='k')
	ax.set_ylabel(ylabel)
	ax.set_xlabel(xlabel)
	ticks = []
	for i in range(len(xvals)):
		ticks.append(xvals[i]*wid + .5*wid)
	ax.set_xticks(ticks)
	if xnames == None:
		xnames = range(len(xvals))
	ax.set_xticklabels(xnames, size = "xx-small", rotation = "70", stretch = "condensed")
	fig.set_figheight( fig.get_figheight() * 1.5)
	pl.title(fig_name)
	pl.savefig(out_file + ".png", format = "png")
	pl.close()
	
#A function that simply prints out the same data that is used by make_bar to a csv file
def write_bar_data(yvals, errorvals, out_file, ynames=None, xnames=None):
	data_file = shared.openFile(out_file, "w")
	data_file.write("Feature,")
	if xnames != None:
		for name in xnames:
			data_file.write(str(name) + ',')
	else:
		data_file.write("Parameters...")
	data_file.write('\n')
	for feature in range(len(yvals)):
		data_file.write('\n')
		if ynames != None:
			data_file.write(str(ynames[feature]) + ',')
		for param in range(len(yvals[feature])):
			data_file.write(str(yvals[feature][param]) + ',')
		data_file.write('\nstdev:,')
		for param in range(len(errorvals[feature])):
			data_file.write(str(errorvals[feature][param]) + ',')
	data_file.close()
	return

#This function is used for the elasticity graphs. 
#Organization of the arrays:
#data[which nominal set][which parameter][which perturbation amount][which oscillation feature value]
#nominal_features[which nominal set][0][which oscillation feature value]
def line_plot(data, nominal_features, p, f, fig_name, param_name, image_dir, percent, points ):
	fig = pl.figure()
	if Y_BOUNDS == None:
		ax = fig.add_subplot(111, autoscaley_on=True)
	else:
		ax = fig.add_subplot(111, ybound = Y_BOUNDS, autoscaley_on=False)
		
	step = float(percent)/points
	
	xvals = [0]*(points*2 + 1)
	for i in range(points):
		xvals[i] = -1 * (points - i) * step
	for i in range(points + 1):
		xvals[i + points] = i * step
		
	for n in range(len(data)):
		yvals = [0]*(points*2 + 1)
		nominal_features[n][0][f] = num_check(nominal_features[n][0][f])
		for i in range(points):
			data[n][p][i][f] = num_check( data[n][p][i][f] )
			yvals[i] = float( data[n][p][i][f] ) / nominal_features[n][0][f]
			yvals[i] = num_check(yvals[i])
		yvals[points] = 1
		for i in range(points, 2*points):
			data[n][p][i][f] = num_check( data[n][p][i][f] )
			yvals[i+1] = float( data[n][p][i][f] ) / nominal_features[n][0][f]
			yvals[i] = num_check(yvals[i])
		
		ax.plot( xvals, yvals, linestyle = '-', marker = 'D', color = (float(n)/len(data),.1,float((n**2)%17)/17) )
	ax.set_ylabel("(Perturbed value) / (Nominal value)")
	ax.set_xlabel(param_name +" perturbation (%)")
	pl.title(fig_name)
	pl.savefig(image_dir + "/"+sanitize(fig_name) +"_"+ str(p)+"on"+str(f) + ".png", format = "png")
	pl.close()
	return

#This function is used to check a value to see if it's infinite or nan.
def num_check(value):
	if abs(value) == float('inf') :
		return 500 
	elif "nan" in str(value) or value != value:
		return 1
	else:
		#print "Value: " + str(value)
		return value

#This function just checks a string for characters that should not be put into a file name and replaces them with underscores.
def sanitize(fname):
	sane = ""
	for c in fname:
		if c.isalnum():
			sane += c
		else:
			sane += "_"
	return sane 
	
def usage():
	print 'Usage: python plot-sensitivity.py [-option [val]] ...'
	print '-n, --nominal-file     [filename]  : The file to use for getting nominal parameter sets, default=~/sensitivity-analysis/sens/sensitivity-analysis/sensitivity-analysis/nominal.params'
	print '-d, --dir              [filename]  : the directory to put all plots in, default=all-sensitivities'
	print '-D, --data-dir         [filename]  : the directory in which to store raw sensitivity data, default=sense-for-plot'
	print '-o, --output           [filename]  : the name of file(s) to use for plot images, default=sensitivity'
	print '-j, --job-name         [string]    : the name that should be used for the pbs job, default=PLOT_SA'
	print '-C, --cluster-name     [string]    : the name of the cluster on which to run simulations, if any, default=none'
	print '-y, --ymin             [float]     : set the minimum value for the axes of the plot, defualt=auto-scaling that varies between graphs'
	print '-Y, --ymax             [float]     : set the maximum value for the axes of the plot, defualt=auto-scaling that varies between graphs'
	print '-N, --nodes            [int]       : the number of nodes to be utilized. Runs are done locally unless -N > 1, default=1'
	print '-f, --feature          [int]       : index of feature, if only one feature should be plotted. Omit if all features should be plotted, default=unused'
	print '-p, --percent          [int]       : max percentage by which to perturb nominal value, default=20'
	print '-P, --points           [int]       : number of data points to collect between nominal and nominal + (max percent * nominal), default=4'
	print '-l, --ppn              [int]       : processors per node, i.e. processes to use in parallel for the analysis, default=2'
	print '-c, --nominal-count    [int]       : the number of nominal sets to use for sensitivity calculations, default=1'
	print '-e, --exec             [directory] : the path of the executable for performing sensitivity analysis, default=~/sensitivity-analysis/sens/sensitivity-analysis/sensitivity-analysis/sensitivity'
	print '-s, --sim              [directory] : the path of the executable for running simulations, default=../simulation/simulation'
	print '-g, --graph            [N/A]       : just generate graphs without running simulations, assuming the appropriate files have already been generated, default=unused'
	print '-E, --elasticity       [N/A]       : include this to plot (oscillation features)/(nominal feature value) as a scatter plot connected by lines (instead of sensitivity bar graphs)'
	print '-a, --args             [N/A]       : arguments following this will be sent to the simulation'
	print '-h, --help             [N/A]       : view usage information (i.e. this)'
	exit(0)
main()

1. command lines 

1.1 Compiling

We use SCons as our compiling tool. The compiling settings for simulation code and sres code are in the SConstruct file in simulation and sres directory respectively. If you cannont compile it with SCons, you can just use "g++" or an equivalant compiler to compile the code manually. 


Go to simulation directory and use command "scons" to compile the simulation code.

Go to sres directory and use command "scons" to compile the sres code (sequential version).

Go to sres directory and use command "scons mpi=1" to compile the sres code (parallel version).


1.2 simulation:


Sample command line for running simulation:

./simulation -x 50 -w 10 -y 4 -i set_test.params -s 2000 -S 0.01 -M 1 -V 600 -Y 600 -Z 600 -Q 600 -K 600 -r gradient1.txt -u perturb.txt -t -D output -L 1


-i, --params-file        [filename]   : the relative filename of the parameter sets input file, default=none
-R, --ranges-file        [filename]   : the relative filename of the parameter ranges input file, default=none
-u, --perturb-file       [filename]   : the relative filename of the perturbations input file, default=none
-r, --gradients-file     [filename]   : the relative filename of the gradients input file, default=none
-o, --print-passed       [filename]   : the relative filename of the passed sets output file, default=none
-t, --print-cons         [N/A]        : print concentration values to the specified output directory, default=unused
-B, --binary-cons-output [N/A]        : print concentration values as binary numbers rather than ASCII, default=unused
-f, --print-osc-features [filename]   : the relative filename of the file summarizing all the oscillation features, default=none
-V, --her1-induction     [int]        : the induction point for her1 overexpression
-Y, --her7-induction     [int]        : the induction point for her7 overexpression
-Z, --DAPT-induction     [int]        : the induction point for DAPT treatment
-Q, --mespa-induction     [int]        : the induction point for mespa overexpression
-K, --mespb-induction     [int]        : the induction point for mespb overexpression
-D, --directory-path     [directory]  : the relative directory where concentrations or anterior oscillation features files will be printed, default=none
-A, --anterior-feats     [N/A]        : print in depth oscillation features for the anterior cells over time, default=unused
-P, --posterior-feats    [N/A]        : print in depth oscillation features for the posterior cells over time, default=unused
-W, --print-conditions   [filename]   : the relative filename of the passed and failed conditions file, default=none
-E, --print-scores       [filename]   : the relative filename of the mutant scores file, default=none
-L, --print-cells        [int]        : the number of columns of cells to print for plotting of single cells on top of each other, min=0, default=0
-b, --big-granularity    [int]        : the granularity in time steps with which to store data, min=1, default=1
-g, --small-granularity  [int]        : the granularity in time steps with which to simulate data, min=1, default=1
-x, --total-width        [int]        : the tissue width in cells, min=3, default=3
-w, --initial-width      [int]        : the tissue width in cells before anterior growth, min=3, max=total width, default=3
-y, --height             [int]        : the tissue height in cells, min=1, default=1
-S, --step-size          [float]      : the size of the timestep to be used for solving the DDEs using Euler's method, default=0.01
-m, --total-time         [int]        : the number of minutes to simulate before ending, min=1, default=1200
-T, --split-time         [int]        : the number of minutes it takes for cells to split, min=1, default=6, 0=never
-G, --time-til-growth    [int]        : the number of minutes to wait before allowing cell growth, min=0, default=600
-p, --parameter-sets     [int]        : the number of parameters for which to simulate the model, min=1, default=1
-s, --seed               [int]        : the seed to generate random numbers, min=1, default=generated from the time and process ID
-X, --reset-seed         [N/A]        : reset the seed after each parameter set so the initial seed is used each time, default=unused
-d, --parameters-seed    [int]        : the seed to generate random parameter sets, min=1, default=generated from the time and process ID
-e, --print-seeds        [filename]   : the relative filename of the seed output file, default=none
-a, --max-con-threshold  [float]      : the concentration threshold at which to fail the simulation, min=1, default=infinity
-C, --short-circuit      [N/A]        : stop simulating a parameter set after a mutant fails, default=unused
-M, --mutants            [int]        : the number of mutants to run for each parameter set, min=1, max=7, default=7
-I, --pipe-in            [file desc.] : the file descriptor to pipe data from (usually passed by the sampler), default=none
-O, --pipe-out           [file desc.] : the file descriptor to pipe data into (usually passed by the sampler), default=none
-c, --no-color           [N/A]        : disable coloring the terminal output, default=unused
-v, --verbose            [N/A]        : print detailed messages about the program and simulation state, default=unused
-q, --quiet              [N/A]        : hide the terminal output, default=unused
-l, --licensing          [N/A]        : view licensing information (no simulations will be run)
-h, --help               [N/A]        : view usage information (i.e. this)



things to know:

"gradient1.txt" is the default gradient file. "gradient25.txt" should be used when the mesp genes start expressing at 25th column(by default they start at 30th column). "gradient60.txt" should be used when the total width of the tissue is set to 60.

The defualt perturbation level is 5%.

The step size can be set to 0.02, any larger step size may lead to segmentation fault.


1.3 sres sequential version

Sample command line:

./sres -r fguess.ranges -g 1200 -p 100 -P 15 -a -x 50 -w 10 -y 4 -M 1 -V 600 -Y 600 -Z 600 -Q 600 -K 600 -r ../simulation/gradient1.txt -u ../simulation/perturb.txt

-r, --ranges-file        [filename]   : the relative filename of the ranges input file, default=none
-f, --simulation         [filename]   : the relative filename of the simulation executable, default=../simulation/simulation
-d, --dimensions         [int]        : the number of dimensions (i.e. rate parameters) to explore, min=1, default=45
-P, --parent-population  [int]        : the population of parent simulations to use each generation, min=1, default=3
-p, --total-population   [int]        : the population of total simulations to use each generation, min=1, default=20
-g, --generations        [int]        : the number of generations to run before returning results, min=1, default=1750
-s, --seed               [int]        : the seed used in the evolutionary strategy (not simulations), min=1, default=time
-e, --printing-precision [int]        : how many digits of precision parameters should be printed with, min=1, default=6
-a, --arguments          [N/A]        : every argument following this will be sent to the simulation
-c, --no-color           [N/A]        : disable coloring the terminal output, default=unused
-v, --verbose            [N/A]        : print detailed messages about the program state
-q, --quiet              [N/A]        : hide the terminal output, default=unused
-l, --licensing          [N/A]        : view licensing information (no simulations will be run)
-h, --help               [N/A]        : view usage information (i.e. this)


1.4 sres (on cluster):

sample job submission file:

#PBS -N sres_15_one_mutant
#PBS -l nodes=1:ppn=10
#PBS -q biomath
#PBS -M xxxxx@colgate.edu
#PBS -m bea
#PBS -j oe
#PBS -r n

cd $PBS_O_WORKDIR


mpirun -np 10 ./sres -r fguess.ranges -g 1200 -p 100 -P 15 -a -x 50 -w 10 -y 4 -M 1 -V 600 -Y 600 -Z 600 -Q 600 -K 600 -r ../simulation/gradient1.txt -u ../simulation/perturb.txt > output2.txt

-np   [int]   : the number of processors needed for running the code.

"> output2.txt" will redirect the output to "output2.txt".



things to know:

fguess.ranges is the default range file.

biomath cluster can only handle simulation with less than 3 mutants due to insufficient memory. If you need to run with more mutants, ask Patrick Holloway for permission to another machine.

You can choose which mutant to be simulated by changing that mutant index to 1 in macros.hpp. (Wildtype mutant(index 0) must be simulated first) 
For example, if you want to run Wildtype and Her7 overexpressiono only, change MUTANT_HER7OVER to 1.
In "macros.hpp":
#define MUTANT_WILDTYPE		0
#define MUTANT_DELTA		1
#define MUTANT_HER7OVER		2
#define MUTANT_HER1OVER		3
#define MUTANT_DAPT         4
#define MUTANT_MESPAOVER	5
#define MUTANT_MESPBOVER	6




Sres jobs on biomath will be terminated anytime(randomly) after 400 generations, but usually the scores are okay after 400 generations, so this may not be a big problem. 








2. printing output

To change the output species for .cons files, change the print_con veriable in struct mutant_data in struct.cpp.

struct mutant_data {
	...
    ...
		memset(this->num_conditions, 0, sizeof(this->num_conditions));
		memset(this->cond_scores, 0, sizeof(this->cond_scores));
		memset(this->conds_passed, 0, sizeof(this->conds_passed));
		memset(this->max_cond_scores, 0, sizeof(this->max_cond_scores));
		memset(this->secs_passed, false, sizeof(this->secs_passed));
		this->print_con = CMH1;
	}
	...
	...
};



To change the output species for .cells files, change TWO lines (392 and 428) in the function print_cell_columns in io.cpp.

For example, to change from printing mRNA of mespa to print mRNA level of her1:
time_point[line * ip.num_colls_print + col] = cl.cons[CMMESPA][time][line * sd.width_total + cur_col];
-->
time_point[line * ip.num_colls_print + col] = cl.cons[CMH1][time][line * sd.width_total + cur_col];





3. Python scripts for plotting.

plot_two.py: plot two different species in the same graph, (usually) use .cells file:
python plot_two.py -c filename1 filename2 folder outputname labelname



plot-snapshots-two.py: create snapshots of the tissue that contains expression level of two species, use .cons file:
python plot-snapshots-two.py filename1 filename2 folder

ffmpeg: create movie using snapshots created plot-tissue-snapshots.py/plot-snapshots-two.py:
ffmpeg -y -r 38 -i directory/%04d.png -qscale 5 -b 9600 name_of_movie 


refine-ranges.py: refine ranges based on specified range file and parameter sets file.
python refine-ranges.py -s params -c ranges -n new_ranges


things to know: make sure you have required software packages, such as scipy, ffmpeg. 





4. Sample study


4.1 run sres on cluster:

Create a new job scrpt on the cluster:

#PBS -N sres_15_one_mutant
#PBS -l nodes=1:ppn=10
#PBS -q biomath
#PBS -M xxxxx@colgate.edu
#PBS -m bea
#PBS -j oe
#PBS -r n

cd $PBS_O_WORKDIR


mpirun -np 10 ./sres -r fguess.ranges -g 1200 -p 100 -P 15 -a -x 50 -w 10 -y 4 -M 1 -V 600 -Y 600 -Z 600 -Q 600 -K 600 -r ../simulation/gradient1.txt -u ../simulation/perturb.txt > output2.txt

Submit the job script by using command "qsub sres2.run"


To see the output, use the command "cat output2.txt" or "vim output2.txt".



4.2 Create oscillation information.

Store some good parameter sets from "output2.txt" in a params file "set.params".

Modify structs.cpp and io.cpp so that concentration level mRNA of her1 will be printed.

Select one parameter set from set.params and put it into a new file "set_test.params"

Use the parameter set and run simulation:

./simulation -x 50 -w 10 -y 4 -i set_test.params -s 2000 -S 0.01 -M 1 -V 600 -Y 600 -Z 600 -Q 600 -K 600 -r gradient1.txt -u perturb.txt -t -D output -L 1

Now, you have set_0.cells and set_0.cons in output directory.

Rename them to mh1.cells and mh1.cons.

Modify structs.cpp and io.cpp so that concentration level of mRNA of mespa will be printed.

Run the simulation again:

./simulation -x 50 -w 10 -y 4 -i set_test.params -s 2000 -S 0.01 -M 1 -V 600 -Y 600 -Z 600 -Q 600 -K 600 -r gradient1.txt -u perturb.txt -t -D output -L 1

Now, you have new set_0.cells and set_0.cons in output directory.

Rename them to mespa.cells and mespa.cons.



4.3 Plot graphs and create movies

To create a graph to show the complementary expression of her1 and mespa, use plot_two.py:
python plot_two.py -c mh1.cells mespa.cells test_graph test1 her1_mespa

To take snapshot of the simulation of her1 and mespa, use plot-snapshots-two.py:
python plot-snapshots-two.py mh1.cons mespa.cons snapshots

To create movie for the simulation, use ffmpeg:
ffmpeg -y -r 38 -i snapshots/%04d.png -qscale 5 -b 9600 name_of_movie 



4.4 refine ranges

Refine ranges with current ranges file and a parameter set files:
python refine-ranges.py -s set.params -c fguess.ranges -n new.ranges


4.5 Submit new job on cluster with new ranges file.








5. Things to do 

1. Make sure the simulation model is correct.

2. Make sure all the indecies are correct.

3. Modify perturb_rates_all(rs) so that in sim.cpp:422 it will update(knockout) the specified rate in existing cells without perturbing the entire tissue. Or you can create a new function for this. 

4. Find out the problem of calculating the sycronization score of mespb (it could be due to bugs in code or bad parameter set)

5. Find out the problem in overexpression of her7 mutant that causes her1 and mespa to have strange amplitude (it could be due to bugs in code or bad parameter set)

6. Find out the problem of the non-travelling first wave of mesp expression. 

7. Fix the step size problem

8. Create simulation option to print out two species at the same time.

9. Clean up the code, delete unused functions or lines.

10. Find and fix the problem that cause the jobs in the cluster to be terminated unexpectedly.




6. comment in code

I mark most of my comments with "151221" so you can find them more easily. 

















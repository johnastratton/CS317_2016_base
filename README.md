Zebrafish somitogenesis simulation
==================================

Table of contents
-----------------

| 0: Compatibility and system requirements  
|__ 0.0: Tested systems and portability  
|__ 0.1: MPI dependency  
|__ 0.2: SCons dependency  
|__ 0.3: Python and SciPy dependencies  
|__ 0.4: FFmpeg dependency  
| 1: Compilation  
|__ 1.0: Compiling with and without SCons  
|__ 1.1: Compilation options  
|__ 1.2: Compiling for MPI  
|__ 1.3: Random number generation discrepancies  
| 2: Running simulations  
|__ 2.0: Biological and computational description of the simulation  
|__ 2.1: Setting up a simulation  
|__ 2.2: Input and output  
|____ 2.2.0: Methods of input  
|____ 2.2.1: Parameter sets  
|____ 2.2.2: Perturbations and gradients  
|____ 2.2.3: Command-line arguments  
|____ 2.2.4: Input file formats  
|______ 2.2.4.0: Parameter sets format  
|______ 2.2.4.1: Perturbations format  
|______ 2.2.4.2: Gradients format  
|______ 2.2.4.3: Ranges format  
|____ 2.2.5: Output file formats  
|______ 2.2.5.0: Passed sets format  
|______ 2.2.5.1: Concentrations text format  
|______ 2.2.5.2: Concentrations binary format  
|______ 2.2.5.3: Oscillation features format  
|______ 2.2.5.4: Conditions format  
|______ 2.2.5.5: Scores format  
|______ 2.2.5.6: Seeds format  
|____ 2.2.6: Piping in parameter sets from other applications  
|____ 2.2.7: Generating random parameter sets  
|__ 2.3: Modifying the code  
|____ 2.3.0: Adding command-line arguments  
|____ 2.3.1: Adding input and output files  
|______ 2.3.1.0: Input files  
|______ 2.3.1.1: Output files  
|____ 2.3.2: Adding mutants  
|____ 2.3.3: Adding genes  
|____ 2.3.5: Other modifications  
| 3: Finding parameter sets with SRES  
|__ 3.0: How SRES finds parameters  
|____ 3.0.0: How evolutionary algorithms work  
|____ 3.0.1: How SRES works  
|____ 3.0.2: How SRES-gradients works  
|__ 3.1: Setting up SRES  
|____ 3.1.0: Searching for parameters  
|____ 3.1.1: Searching for gradients  
|____ 3.1.2: Using MPI  
|__ 3.2: Input and output  
|____ 3.2.0: Methods of input  
|____ 3.2.1: Ranges  
|____ 3.2.2: Simulation calls  
|____ 3.2.3: Command-line arguments  
|____ 3.2.4: Input file formats  
|____ 3.2.5: Output file formats  
|__ 3.3: Modifying the code  
|____ 3.3.0: Adding command-line arguments  
|____ 3.3.1: Changing which arguments SRES sends to simulations  
|____ 3.3.2: Adding input and output files  
|____ 3.3.3: Modifying libSRES  
|____ 3.3.4: Other modifications  
| 4: Performing sensitivity analysis  
|__ 4.0: Overview  
|____ 4.0.0. Overview of Local Sensitivity Analysis  
|____ 4.0.1. Overview of the program  
|__ 4.1: Using sensitivity  
|____ 4.1.0. Command-line arguments  
|____ 4.1.1: Input file format  
|____ 4.1.2: Simulation output file format  
|____ 4.1.3: Sensitivity output file format  
|____ 4.1.4: Example program call  
|__ 4.2: Modifying the code  
|____ 4.2.0: Adding command-line arguments  
|____ 4.2.1: Modifying the simulation program  
| 5: Analysis and parsing scripts  
|__ 5.0: Running simulations over multiple seeds (run-multiple-seeds.py)  
|____ 5.0.0: Overview  
|____ 5.0.1: Command-line arguments  
|____ 5.0.2: Example program calls  
|__ 5.1: Checking robustness (check-robustness.py)  
|____ 5.1.0: Overview  
|____ 5.1.1: Command-line arguments  
|____ 5.1.2: Example program calls  
|__ 5.2: Running simulations over multiple gradients (run-multiple-gradients.py)  
|____ 5.2.0: Overview  
|____ 5.2.1: Command-line arguments  
|____ 5.2.2: Example program calls  
|__ 5.3: Refining ranges (refine-ranges.py)  
|____ 5.3.0: Overview  
|____ 5.3.1: Command-line arguments  
|____ 5.3.2: Example program calls  
|__ 5.4: Plotting oscillation features (plot-features.py)  
|____ 5.4.0: Overview  
|____ 5.4.1: Command-line arguments  
|____ 5.4.2: Example program calls  
|__ 5.5: Plotting synchronization (plot-synchronization.py)  
|____ 5.5.0: Overview  
|____ 5.5.1: Command-line arguments  
|____ 5.5.2: Example program calls  
|__ 5.6: Plotting cells over time (plot-cells.py)  
|____ 5.6.0: Overview  
|____ 5.6.1: Command-line arguments  
|____ 5.6.2: Example program calls  
|__ 5.7: Plotting sensitivity analysis (plot-sensitivities.py)  
|____ 5.7.0: Overview  
|____ 5.7.1: Command-line arguments  
|____ 5.7.2: Example program calls  
|__ 5.8: Plotting densities (plot-densities.py)  
|____ 5.8.0: Overview  
|____ 5.8.1: Command-line arguments  
|____ 5.8.2: Example program calls  
|__ 5.9: Plotting tissue snapshots (plot-tissue-snapshots.py)  
|____ 5.9.0: Overview  
|____ 5.9.1: Command-line arguments  
|____ 5.9.2: Example program calls  
| 6: Debugging, profiling, and memory tracking  
|__ 6.0: Debugging  
|__ 6.1: Profiling  
|__ 6.2: Memory tracking  
| 7: Case study  
|__ 7.0: Prerequisites  
|__ 7.1: Find biologically realistic parameters  
|____ 7.1.0: Find posterior parameters  
|____ 7.1.1: Find anterior parameters  
|__ 7.2: Perform sensitivity analysis  
|__ 7.3: Find anterior gradients  
|__ 7.4: Create plots  
|__ 7.5: Create snapshots and movies  
| 8: Authorship and licensing  

0: Compatibility and system requirements
----------------------------------------

**0.0: Tested systems and portability**

This package has been designed for Unix based machines. It has been tested on Fedora 18 with GCC version 4.7.2 and OSX 10.7 (Lion) with GCC 4.2.1 using 64-bit processors. Because this package's only external dependencies are the C++98 standard libraries, Python, and SciPy, most of its functionality is platform-independent. However, various file I/O functions rely on Unix system commands making full compatibility with Windows and other non-Unix operating systems unlikely.

***********************
**0.1: MPI dependency**

Due to the computationally intensive nature of the simulations, we recommend either a very powerful, multicore processor or a cluster of many processors so that multiple simulations can be run in parallel. To achieve paralellization, most applications in this package that run multiple simulations are parallelized using MPI (Message Passing Interface), which can be downloaded at http://www.open-mpi.org/software/ompi/v1.6/. While all applications have serial versions for testing purposes, replicating our results serially would take several weeks of computation time, making the installation of MPI essentially a requirement.

*************************
**0.2: SCons dependency**

The software constructor tool SCons 2.3.0 is used to simplify compilation. SCons can be downloaded at http://www.scons.org/download.php but this tool is not necessary and instructions for direct compilation are provided in Section 1.

**************************************
**0.3: Python and SciPy dependencies**

Scripts for creating figures and parsing data files require Python 2.7.3 and SciPy, which can be downloaded at http://www.python.org/download/ and http://www.scipy.org/install.html, respectively. These scripts are independent from the simulation code but are useful for parsing and visualizing results.

**************************
**0.4: FFmpeg dependency**

Movies can be made from tissue snapshots using FFmpeg 1.0.7, which can be downloaded at http://www.ffmpeg.org/download.html. If you have no need to create movies, FFmpeg's intallation can be skipped.

1: Compilation
--------------

**1.0: Compiling with and without SCons**

To compile an application in its default configuration, open a terminal window and navigate to the package's root directory. If SCons is installed on the machine, simply enter 'scons' to compile the source. If SCons cannot be installed on the machine, each application can be compiled manually by entering its associated g++ compilation statement:
* simulation: 'g++ -O2 -Wall -o simulation main.cpp init.cpp sim.cpp feats.cpp io.cpp memory.cpp debug.cpp'
* sres: 'g++ -O2 -Wall -o sres main.cpp init.cpp sres.cpp io.cpp memory.cpp'
* sensitivity: 'g++ -O2 -Wall -o sensitivity source/analysis.cpp source/init.cpp source/io.cpp source/memory.cpp finite-difference/finite-difference.cpp'

If g++ is not install on the machine, you need to install it or an equivalent compiler.

****************************
**1.1: Compilation options**

All applications come with at least three compilation options, 'profile', 'debug', and 'memtrack'. By entering 'scons profile=1', 'scons debug=1', or 'scons memtrack=1', the application is compiled with compile and link flags designed for profiling, debugging, and memory tracking, respectively. Profiling adds the '-pg' compile and link flags, which adds extra code that enables gprof profiling analysis. Debugging adds the '-g' compile flag, which adds extra code that enables GDB debugging. Memory tracking adds the '-D MEMTRACK' compile flag, which adds a custom macro indicating the program should track its heap memory allocation. For more information on these options, see Section 6.

**************************
**1.2: Compiling for MPI**

When MPI-compatible applications are compiled with 'scons mpi=1' then the mpic++ compiler is used instead of g++ and a custom macro indicating the program should parallelize with MPI is added via '-D MPI'.

***********************************************
**1.3: Random number generation discrepancies**

As of this publication, Linux and Mac use different versions of GCC whose standard random number generators happen to produce different random numbers even when given the same seed. Because the modeled system is robust, parameter sets should receive similar scores regardless of the initial seed but it is worth noting that an identically configured simulation may produce different results on different operating systems when random number generation is incorporated via perturbations.

2: Running simulations
----------------------

**2.0: Biological and computational description of the simulation**

*******************************
**2.0.0: The biological model**

This simulation is a one or two dimensional, gene-and-cell simulation of the presomitic mesoderm in zebrafish embryos. It includes the genes Her1, Her7, Hes6, and DeltaC. Hes6 is referred to as Her13 in the code for legacy reasons and DeltaC is referred to as Delta for concision. Delta performs all of the functionality of the DeltaC-Notch intercellular signaling pathway. Her1, Her7, and Hes6 homodimerize and heterodimerize, with Her1Her1 and Her7Hes6 competitively binding to DNA regulatory region to repress _her1_ and _her7_ mRNA transcription. The simulation uses delayed differential equations solved to model mRNA and protein synthesis and degradation as well as dimer association, dissociation, and degradation. The equations are numerically solved via Euler's method.

********************************
**2.0.1: Possible tissue sizes**

The cell tissue can be either two cells, a one dimensional chain of three or more cells, or a two dimensional hexagonal grid of cells, the width of which must be an even number greater than three for wrapping purposes. In the 2-cell version, the cells are neighbors with each other. In the 1D version, cells are neighbors with the two cells immediately adjacent to them. However, the most posterior and anterior cells have only one neighbor. In the 2D version, cells are neighbors with the six cells immediately adjacent to them, except for the most posterior and anterior cells, which have four neighbors.

**************************************
**2.0.2: Rates, perturbations, and gradients**

The simulation is controlled by a set of parameters, each representing a biological rate such as _her1_ mRNA synthesis. Each rate can be altered by random perturbations and spatial gradients, which control how much a rate is expressed as a cell moves through the PSM. These rates determine the concentration levels of mRNA, proteins, and dimers in the system.

******************
**2.0.3: Mutants and scores**

Parameter sets are tested with wild type conditions and several mutants, each of which either knocks down or overexpresses genes. Each mutant (including wild type) has conditions to pass and reports a score based on its success. A parameter set's score is the sum of all its mutants' scores over the maximum possible score. Most conditions involve checking features of a certain concentration levels' oscillatory features, such as period or amplitude.

********************************
**2.1: Setting up a simulation**

After compiling (see Section 1), there should be an executable named "simulation" in the simulation directory. The simulation has many configurations but most have default values and do not necessarily have to be specified. To run a simulation, navigate to the simulation directory in the terminal and enter "./simulation ..." where "..." contains all of the command-line options. Once the simulation starts, it will not ask for further input and will print out various milestones when it is finished. Some results, like scores, are printed to the terminal but all results can stored in files you specify (you must explicitly ask for any output files). If the program cannot interpret a part of your input, it will let you know with as much specific information as possible; it will not silently fail.

*************************
**2.2: Input and output**

***************************
**2.2.0: Methods of input**

The only interaction with a simulation is through input you give it before it starts and output it gives you after it ends. There are four aspects of input:
* Parameter sets
* Perturbations
* Gradients
* Command-line arguments

***********************************
**2.2.1: Parameter sets**

Parameter sets are inputted in one of three ways: an input file containing each set, the file descriptor of a pipe that will send them in, or an input file containing valid ranges for each parameter and the number of random sets to generate based off of these ranges. The file formats for parameter sets and ranges files are covered in Section 2.2.4 and piping is covered in Section 2.2.6.

************************************************
**2.2.2: Perturbations and gradients**

Perturbations and gradients are inputted via input files specified by the command-line arguments --perturb-file and --gradients-file, respectively. If files are not specified via the command-line, no perturbations or gradients are applied. Only one of each file can be inputted but the files can contain multiple perturbations and gradients. The file formats for both are covered in Section 2.2.4.

*********************************
**2.2.3: Command-line arguments**

Each command-line argument can be entered in a short or long form. Short forms begin with a single dash (-) followed by a single letter. Long forms begin with a double dash (--) followed by a word or phrase. Lower-case letters are considered distinct from their upper-case equivalents. The short and long forms are equivalent - short forms are for convenience and long forms are for clarity. The following is a comprehensive list of all command-line options and their uses:

-i, --params-file        [filename]   : the relative filename of the parameter sets input file, default=none
-R, --ranges-file        [filename]   : the relative filename of the parameter ranges input file, default=none
-u, --perturb-file       [filename]   : the relative filename of the perturbations input file, default=none
-r, --gradients-file     [filename]   : the relative filename of the gradients input file, default=none
-o, --print-passed       [filename]   : the relative filename of the passed sets output file, default=none
-t, --print-cons         [N/A]        : print concentration values to the specified output directory, default=unused
-B, --binary-cons-output [N/A]        : print concentration values as binary numbers rather than ASCII, default=unused
-f, --print-osc-features [filename]   : the relative filename of the file summarizing all the oscillation features, default=none
-D, --directory-path     [directory]  : the relative directory where concentrations or anterior oscillation features files will be printed, default=none
-A, --anterior-feats     [N/A]        : print in depth oscillation features for the anterior cells over time, default=unused
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
-M, --mutants            [int]        : the number of mutants to run for each parameter set, min=1, max=11, default=11
-I, --pipe-in            [file desc.] : the file descriptor to pipe data from (usually passed by the sampler), default=none
-O, --pipe-out           [file desc.] : the file descriptor to pipe data into (usually passed by the sampler), default=none
-c, --no-color           [N/A]        : disable coloring the terminal output, default=unused
-v, --verbose            [N/A]        : print detailed messages about the program and simulation state, default=unused
-q, --quiet              [N/A]        : hide the terminal output, default=unused
-l, --licensing          [N/A]        : view licensing information (no simulations will be run)
-h, --help               [N/A]        : view usage information (i.e. this)

*****************************
**2.2.4: Input file formats**

**********************************
**2.2.4.0: Parameter sets format**

A parameter sets file consists of a list of parameter sets. Each parameter set is placed on its own line, with each parameter separated by a comma. There is not a comma after the final parameter set. Each parameter is included and has a floating point or integral value. Blank lines and lines beginning with "#" are ignored. There must be at least one parameter set per file. There is no limit to the number of parameter sets allowed in a single file, although because every parameter set must be loaded into memory at once (to calculate the maximum delay size used to determine the size of various concentration levels structs ahead of time to avoid repeated reallocation), placing millions of parameter sets in a single file may exhaust your computer's RAM.

The following three lines represent an example file:
```
# This is a comment
63.049748,34.955167,0,42.993889,0,44.600336,0.429514,0.453927,0,0.279120,0,0.335253,45.227915,30.430273,0,46.391646,0,22.217098,0.157011,0.338070,0,0.307667,0,0.274870,0.019982,0.000370,0,0.026413,0,0.008877,0,0.027406,0,0,0,0,0.005284,0,0,0.219352,0.032981,0,0.051730,0,0.175033,0,0.116019,0,0,0,0,0.006626,0,0,0.278811,0.269840,0,0.279624,0,0.285013,0,0.238326,0,0,0,0,0.242239,0,0,11.017495,9.139970,0,0.000000,0,7.329024,0.426847,1.210969,0,0.596138,0,12.461520,361.441083,485.384224,0,0,0,0,499.577384
63.000647,33.757737,0,43.849951,0,47.332097,0.434420,0.455262,0,0.274844,0,0.346678,43.338772,30.019011,0,54.822609,0,25.281511,0.141475,0.315663,0,0.345098,0,0.269280,0.018546,0.003612,0,0.028153,0,0.008334,0,0.025200,0,0,0,0,0.011394,0,0,0.170959,0.041615,0,0.044836,0,0.237797,0,0.248760,0,0,0,0,0.017808,0,0,0.280718,0.310334,0,0.343655,0,0.210100,0,0.233876,0,0,0,0,0.214772,0,0,10.916983,9.443056,0,0.000000,0,7.742257,0.445980,1.035695,0,0.578762,0,12.446215,231.836670,477.034572,0,0,0,0,540.815524
```

*********************************
**2.2.4.1: Perturbations format**

A perturbations file consists of a list of perturbed concentration ranges. Each line contains one floating point perturbation factor followed by space-separated indices of the lower and upper bounds of range of concentrations that should be perturbed. A perturbation factor of 100 indicates a maximum absolute perturbation of 100%. Indices are integral. The list of indices and their associated concentrations can be found in source/macros.hpp. Blank lines and lines beginning with "#" are ignored. Perturbations are applied sequentially down the file so if a concentration is found within multiple ranges then the perturbation farthest down the file is the one applied.

The following two lines represent an example file:
```
5 3 6
8.2 20 40
```

*****************************
**2.2.4.2: Gradients format**

A gradients file consists of a list of concentration gradients. Each line contains an index of the concentration that should be perturbed followed by a list of (cell index, factor) pairs that define the gradient. Each pair is enclosed in parentheses separated by spaces and the values in each pair are separated by a space. Cell indices are integral and are relative to the active start of the PSM, with the start of the posterior at 0 and the end of the anterior at the maximum width of the simulation. A gradient factor of 100 indicates 100% expression level, the default. All cell indices to the left of the first specified factor are set to 100% expression level and all cell indices to the right of the last specified factor are set to that factor. All intermediate indices not explicitly given a factor are linearly interpolated using the factors to the left and right. Blank lines and lines beginning with "#" are ignored. Gradients are applied sequentially down the file so if a concentration is given multiple gradients then the gradient farthest down the file is the one applied.

The following two lines represent an example file:
```
4 (7 50) (10 25) (14 0)
14 (0 0) (20 275)
```

**************************
**2.2.4.3: Ranges format**

A ranges files consists of a list of ranges for every parameter in the simulation. Each line contains a string identifying the parameter (used by humans only) and, after whitespace, a (lower bound, upper bound) pair that defines the range. Each pair is enclosed in brackets, [ and ], and the values in each pair are separated by a comma. Both values are floating point or integral and also nonnegative since no parameter represents a value that should be negative. The ranges are read sequentially, with the first range read applied to the first parameter and so on. The number of ranges matches the number of parameters exactly. Blank lines and lines beginning with "#" are ignored. Text after the closing bracket is ignored and can thus be used for further comments.

The following lines represent an excerpt from an example file, where "..." represents missing lines:
```
msh1 [30.0,65.0]
msh7 [30.0,65.0]
msh13 [30.0,65.0]

...

critph1h1 [150.0,900.0]
critph7h13 [150.0,900.0]
critpdelta [200.0,800.0]
```

******************************
**2.2.5: Output file formats**

*******************************
**2.2.5.0: Passed sets format**

A passed sets file consists of a list of parameter sets. Its format is identical to a parameter sets input file (see Section 2.2.4.0) although it never contains blank lines or comments (lines beginning with "#"). Only parameter sets that receive a perfect score are added to the passed sets file.

****************************************
**2.2.5.1: Concentrations text format**

Concentrations are stored as text unless the -B or --binary-cons-output option is specified via the command-line. This section explains the text format; the binary format is explained in Section 2.2.5.2. Text files end with ".cons" while binary files end with ".bcons". Text files are easily readable by humans but take twice as many bytes to store (and therefore more time to read and write) as their binary equivalents.

A concentrations file is stored in the directory given by -D or --directory-path in a subdirectory named for the mutant its results come from. Its name is "set\_?" where X is the parameter set index its results come from. For example, concentrations from the third set's Her1 mutant run with "-D cons" are stored in "cons/her1/set_2.cons" since parameter set indices are zero based.

The first line of the file contains the simulation's tissue width and height separated by a space. Each following line starts with the time step it represents followed by each cell's concentration value at that time, each value separated by a space. Which concentration value is printed depends on the mutant; most mutants print _her1_ mRNA but any mutant can print any concentration by editing its print_con property in the create\_mutant\_data function in source/init.cpp. Cell indices that represent cells not yet grown receive concentrations of 0 every time step until they are created.

If the small and big granularities (specified via the command-line with -g or --small-granularity and -b or --big-granularity, respectively) are equal, as they are by default, then the concentrations file prints every time step simulated. If the big granularity is larger than the small granularity, every X time steps are printed, where X is the big granularity over the small granularity.

*****************************************
**2.2.5.2: Concentrations binary format**

Concentrations are stored as text unless the -B or --binary-cons-output option is specified via the command-line. This section explains the binary format; the text format is explained in Section 2.2.5.1. Text files end with ".cons" while binary files end with ".bcons".

A concentrations file is stored in the directory given by -D or --directory-path in a subdirectory named for the mutant its results come from. Its name is "set\_?" where X is the parameter set index its results come from. For example, concentrations from the third set's Her1 mutant run with "-D cons" are stored in "cons/her1/set_2.bcons" since parameter set indices are zero based.

The first values in the file are ints that equal the simulation's tissue width and height. These ints are adjacent in memory. Each following set of values starts with the time step it represents followed by each cell's concentration value at that time, with the time step stored as an int and each concentration as a float. Which concentration value is printed depends on the mutant; most mutants print _her1_ mRNA but any mutant can print any concentration by editing its print_con property in the create\_mutant\_data function in source/init.cpp. Cell indices that represent cells not yet grown receive concentrations of 0 every time step until they are created. Time steps and their associated concentrations are grouped in the file based on the tissue size. For example, if the tissue is 6x4 cells then each group consists of (1 * sizeof(int) + 6 * 4 * sizeof(float)) = 100B on a 64-bit machine.

If the small and big granularities (specified via the command-line with -g or --small-granularity and -b or --big-granularity, respectively) are equal, as they are by default, then the concentrations file prints every time step simulated. If the big granularity is larger than the small granularity, every X time steps are printed, where X is the big granularity over the small granularity.

****************************************
**2.2.5.3: Oscillation features format**

An oscillation features file consists of a list of numbers representing various oscillatory features of each parameter set simulated. The first line contains a header of comma separated descriptions of each feature (except the first column, which is the parameter set index). Each following line corresponds sequentially to each parameter set run and contains the set index and every described feature's value separated by commas. The final column indicates whether or not the set passed all conditions with "passed" or "failed", respectively.

******************************
**2.2.5.4: Conditions format**

A conditions file consists of a list of numbers representing whether or not each parameter set reached and passed each condition test. The first line contains a header of comma separated identifiers of each condition (except the first column, which is the parameter set index). Each following line corresponds sequentially to each parameter set run and contains the set index and every identified condition test's result separated by commas. If the set passed the test then a 1 is printed. If the set failed the test then a 0 is printed. If the set did not reach the test then a -1 is printed. Each mutant's columns of conditions are prefaced by a column with the mutant's name in both the header line and each set's line.

**************************
**2.2.5.5: Scores format**

A scores file consists of a list of numbers representing the score each parameter set received for each mutant run. The first line contains a header of comma separated mutant names (except the first column, which is the parameter set index). Each following line corresponds sequentially to each parameter set run and contains the set index and the score of every mutant run separated by commas. The final column contains the total score of every mutant run.

*************************
**2.2.5.6: Seeds format**

A seeds file consists of a list of every seed used for random number generation by the program unless the seed is given by the user. Each line contains one seed prefaced with its context. If parameter sets are randomly generated based on a ranges file (via the command-line with -R or --ranges-file) and the parameters seed is not specified (via the command-line with -d or --parameters-seed) then the first line contains "pseed: X" where X is the seed. If a simulation seed is not specified (via the command-line with -s or --seed) then each seed generated is printed. If the seed is set to reset for each parameter set run (via the command-line with -X or --reset-seed) then only one seed is generated and is printed "set 0: X" where X is the seed. If the seed is not set to reset then a new seed is generated for each parameter set and receives its own line in the seeds file prefaced by its corresponding set index.

***********************************************************
**2.2.6: Piping in parameter sets from other applications**

Because it is often useful to run simulations many times with different parameter sets, the program has the ability to read parameter sets from one end of a pipe and write its resulting score to the other end. This is accomplished by passing to the simulation the file descriptors identifying the two pipe ends via the command-line with -I or --pipe-in for the end to read from and -O or --pipe-out for the end to write to.

To send parameters to the program, write to the input pipe the number of parameters per set (as an int), the number of sets (as an int), and then each parameter of each set as a sequence of doubles. Sending the number of parameters per set is important because the program needs to know how many doubles to read in a row before moving on to the next set. After reading in the number of sets specified, the program stops reading from the pipe and begins simulating. After it finishes every given parameter set, it writes to the output pipe the maximum possible score a set can achieve (as an int) and then the score of each set as a sequence of ints. After writing all of the scores, the program closes the pipe.

To record any received parameter sets, enter a filename to store them via the command-line with -P or --print-sets.

*******************************************
**2.2.7: Generating random parameter sets**

Parameter sets can be generated by the program using a ranges file (via the command-line with -R or --ranges-file) and run on the fly. For details on how to construct a ranges file, see Section 2.2.4.3. To record any generated parameter sets, enter a filename to store them via the command-line with -P or --print-sets.

***************************
**2.3: Modifying the code**

While the program is designed to be flexible and easily configurable, it is impossible to anticipate every need. Furthermore, modeling a similar but distinct system with the program likely requires more changes than can be anticipated. With that in mind, below is included some brief explanations of how to change key aspects of the program. While some of the recommendations are stylistic, following them ensures clean and consistent code and often avoids encountering problems difficult to anticipate without more knowledge of the code base.

****************************************
**2.3.0: Adding command-line arguments**

Adding a command-line argument requires changing the following structs/functions/macros:
* In source/structs.hpp:
  * input\_params
* In source/main.cpp:
  * usage
* In source/init.cpp:
  * accept\_input\_params
  * check\_input\_params

In _usage_, the command-line argument should be added to the usage message. Its short and long form, data type, and description should be included in this message.

In the _input\_params_ struct, a variable to store the argument's value should be added. Some arguments may need more than one variable to store their functionality. For example, input and output files store the name of the file and whether or not the file should be read from / written to.

In _accept\_input\_params_, the argument should be added in the same format the other arguments are. The existence and/or value of the argument should be stored in the _input\_params_ struct variable mentioned above. Note that arguments requiring an option and value (e.g. --params-file input.params) should include a call to _ensure\_empty_ to avoid trying to read a nonexistent value and arguments that should not have a value (e.g. --no-color) should decrement the loop variable, _i_, to avoid off-by-one errors. Look at how other arguments are accepted before adding your own to see if you can reuse existing functionality (e.g. _store\_filename_ to store a filename).

To perform checks that require knowing every given argument and to avoid bloating _accept\_input\_params_, semantic checks on arguments should be placed in _check\_input\_params_. In other words, making sure an argument is an integer is an easily done type check and can be placed in _accept\_input\_params_. However, making sure the argument is between a certain range or satisfies some property can be more verbose and should be placed in _check\_input\_params_. Additionally, if checking the argument's validity requires knowing if another argument was entered, the check cannot be done in _accept\_input\_params_ because arguments can be entered in any order. Checking in _check\_input\_params_ ensures that every argument has been received.

If the command-line argument affects other parts of the program then those parts must obviously be changed as well. If your command-line argument affects biological aspects of the code (i.e. something in source/sim.cpp) then it is usually easier to copy the value over to the _sim\_data_ struct in source/structs.hpp via its constructor since _sim\_data_ is passed into most areas of the simulation while _input\_params_ is used only in initialization, I/O, etc.

****************************************
**2.3.1: Adding input and output files**

Adding an input file requires changing the following structs/functions/macros:
* In source/structs.hpp:
  * input\_params
* In source/main.cpp:
  * main
* In source/init.cpp:
  * accept\_input\_params
  * check\_input\_params

Input and output files should not be hard-coded into the program; they should be added via a command-line argument (see Section 2.3.0 for how to do this). After an argument to read from / write to the file is added (requiring changes to the _input\_params_ struct, _accept\_input\_params_, and _check\_input\_params_), one variable and two functions must be added.

************************
**2.3.1.0: Input files**

For input files, an _input\_data_ struct instance that accepts the _input\_params_ filename should be added to main where the rest of the instances are located. A _read\_X\_file_ function must be added to source/init.cpp, where X is what kind of file is being read (e.g. parameters, perturbations, etc.). Its header must be added to source/init.hpp. This function is usually a thin wrapper for the _read\_file_ function that opens the file and copies its contents to the given _input\_data_ struct. Always include a check in this function to ensure the user wants this file printed (other _read\_X\_file_ functions reference a boolean stored in the _input\_params_ struct indicating whether or not the file was specified via the command-line). A call to this _read\_X\_file_ function should be added in main where the rest of the files are read.

Additionally, a _fill\_X_ function must be added to source/init.cpp, where X is what kind of data the file supplies (e.g. parameters, perturbations, etc.). Its header must be added to source/init.hpp. This file accepts a previously filled _input\_data_ struct and parses its contents into useful information. How this is done is up to you, of course, but there are numerous functions with similar purposes from which to take code and ideas. A call to this _fill\_X_ function should be added in main where the rest of the files are parsed.

*************************
**2.3.1.1: Output files**

For output files, an _ofstream_ instance should be added to main where the rest of the output files are initialized. It should be initialized by a call to _create\_X\_file_, where X is what kind of file is being created. _create\_X\_file_ must be added to source/init.cpp. Its header must be added to source/init.hpp. This file should create the output file stream on the heap with "new ofstream()", open the file for writing with a call to _open\_file_, and print any header information the file should include. There are numerous functions with similar purposes from which to take code and ideas.

A call to _delete\_file_ should be added to main where the rest of the output files are deleted. _delete\_file_ closes the file and frees its associated stream from memory.

*************************
**2.3.2: Adding mutants**

Adding a mutant requires changing the following structs/functions/macros:
* In source/macros.hpp:
  * mutant macros
* In source/init.cpp:
  * create\_mutant\_data

To add a mutant, its name must be added to the list of mutants in source/macros.hpp. It must be given a unique index and the _NUM\_MUTANTS_ macro must be incremented. Its data should be added to _create\_mutant\_data_ in the same way other mutants' data are. If the new mutant has more conditions than allowed by a _MAX\_CONDS\_X_ macro, where X is POST, ANT, or WAVE, then the corresponding macro should be incremented to fit the new number of conditions for that section. If the exact format of the existing mutants in _create\_mutant\_data_ is copied for the new mutant then this is the only function requiring modification.

Each mutant has functions that test the conditions for each section (posterior, anterior, sometimes wave). These functions are located in source/tests.cpp. Copy the format of other mutants' tests for the new mutant and be sure to reference the new functions in the mutant's section in _create\_mutant\_data_.

***********************
**2.3.3: Adding genes**

Adding a gene requires changing the following structs/functions/macros:
* In source/macros.hpp:
  * concentration macros
  * rate macros
  * index macros
* In source/sim.cpp:
  * protein\_synthesis
  * dimer\_proteins
  * mRNA\_synthesis
  * transcription

To add a gene, macros for its associated concentration levels, rates, and indices must be added to soure/macros.hpp. Indices for its mRNA, protein, and dimer concentration levels should be added where the rest of the genes' are. Make sure _NUM\_CON\_LEVELS_, _NUM\_CON\_STORE_, _MIN\_CON\_LEVEL_, and _MAX\_CON\_LEVEL_ are updated accordingly. If a gene's mRNA needs to be analyzed for oscillation features, NUM\_CON\_STORE should be incremented to fit it. If rates are added or changed, ensure that _MIN\_DELAY_ and _MAX\_DELAY_ are still accurate.

Additionally, indices for its mRNA synthesis, mRNA degradation, protein synthesis, protein degradation, dimer associations, dimer dissociations, dimer degradations, mRNA transcription delay, protein translation delay, and (if biologically appropriate) critical number of protein molecules to inhibit mRNA transcription must be added where the rest of the genes' are.

Indices for calculations involving the new gene's mRNA and protein concentrations must also be added where the rest of the genes' are. If the gene behaves like the existing Her genes then it should be given a her gene index and _NUM\_HER\_INDICES_ should be incremented. If it needs to calculate its neighbors' Delta protein concentrations then its index should be lower than the genes' indices that don't need to calculate them and _NUM\_DD\_INDICES_ should be incremented.

The structure of _protein\_synthesis_ is highly dependent on which genes are added to the simulation. A gene that dimerizes with other genes in the system (as of now the Her genes) must call _dim\_int_ for each heterodimer and then _con\_protein\_her_, both in source/sim.cpp, to complete the calculations. Each gene that dimerizes with the new gene must also add a call to _dim\_int_ for their own calculations. If the gene does not dimerize then it should call only _con\_protein\_delta_. A gene that does not fit either the Her or Delta class of genes must have its own calculation functions.

The structure of _dimer\_proteins_ is also highly dependent on which genes are added. However, the calculations are more straightforward; _con\_dimer_ must be called for every dimer concentration. Note that, for example, Her1Her7 is the same dimer as Her7Her1 and should not be calculated twice.

_mRNA\_synthesis_ has very specific functionality and should be carefully analyzed to ensure biological accuracy. Genes whose concentration calculations require its neighbors' Delta protein concentrations should be included in set of genes defined by the _NUM\_DD\_INDICES_ macro, explained above. The final loop of the function that calculates the concentration values performs different behavior for different mRNA concentrations and may need to be modified if the gene being added demonstrates unique behavior.

If the gene being added forms dimers that inhibit _her1_ and _her7_ transcription (like Her1Her1, Her7Her13, etc. do), the gene's relevant dimers need to be added to the calculations in _transcription_ in the same way every other inhibiting dimer is.

These instructions assume that the gene being added behaves in the same ways existing genes do and do not guarantee biological accuracy if you add an exotic gene.

******************************
**2.3.5: Other modifications**

We apologize if instructions on how to make your desired changes are not included in this README. Our only advice for changing more fundamental aspects of the program is to be sure to check every source file to ensure that your changes propagate appropriately; while the code is designed to be as modular as possible, practical limitations have caused unexpected dependencies throughout the code. In other words, read all of the code before changing it and good luck!

3: Finding parameter sets with SRES
-----------------------------------

**3.0: How Evolutionary Strategy finds parameters**

Evolution Strategy is used to find biologically realistic parameters by repeatedly running simulations with different parameter sets or configurations. While the theory behind evolutionary algorithms and the details of the library used are beyond the scope of this README, a brief explanation of them follows with the hope that it is enough to reproduce our results.

*******************************************
**3.0.0: How evolutionary algorithms work**

Evolutionary algorithms "evolve" parameters to perform a particular function better. A population is created, each member of which gets its own random set of parameters and performs a particular function that returns a fitness score. Members with the best fitness are allowed to continue to the next generation, at which point they are mutated (a parameter value is randomly modified by a small amount) and crossed over (two members create a new member with a combination of both parents' parameters). The new members perform the same function with their new parameters and new fitness scores are returned. This continues for a specified number of generations, hopefully evolving better sets of parameters.

*************************
**3.0.1: How SRES works**

SRES (Stochastically Ranked Evolution Strategy) uses the libSRES library, which can be downloaded at http://rich.yunda.org/uga/science/libSRES/. libSRES uses a stochastically ranked evolution strategy that, unlike the basic evolutionary strategy, stochastically ranks members when deciding which ones to propogate to the next generation. The full explanation can be found in the paper at https://notendur.hi.is/~tpr/software/sres/Tec311r.pdf, but in short, this algorithm allows there to be a chance that a worse performing member continues to the next generation instead of a better performing one. This prevents the algorithm from getting stuck in local maxima. If only the best members are used then once they find a range of parameter sets that perform better than all sets around them in the problem space, the algorithm has no incentive to branch out to find better maxima because every direction looks worse than its current location.

In our case, members of the population are simulations and the parameter sets picked, mutated, etc. are the usual parameter sets used in the simulation. SRES pipes in a parameter set to the simulation and uses the score it receives as its fitness score, replicating this process dozens of times for hundreds of generations. By evolving the parameter sets, simulations receive increasingly better scores on condition tests until they pass every test, at which point there can obviously be no more improvement.

***********************************
**3.0.2: How SRES-gradients works**

SRES-gradients is a variant on SRES that evolves gradients instead of parameters. When given start point, end point, and gradient factor ranges, the program searches within the ranges using a specified parameter set and tries to find the best gradient configuration. Multiple gradients can be given concurrently, although they all receive the same start point, end point, and factor. The program is intended to be used once a suitable parameter set has been found and can aid in finding the gradients required to meet further conditions that gradient-less simulations cannot achieve.

************************
**3.1: Setting up SRES**

After compiling (see Section 1), there should be an executable named "sres" in the sres directory. SRES has many configurations but most have default values and do not necessarily have to be specified. The only parameter that must be specified is the file to read ranges from (-r or --ranges-file). To begin evolving parameter sets, navigate to the sres directory in the terminal and enter "./sres ..." where "..." contains all of the command-line options. Once the process starts, it will not ask for further input and will print out various milestones and when it is finished. Each generation, the best fitness score and its associated parameter set are printed to the terminal. If the program cannot interpret a part of your input, it will let you know with as much specific information as possible; it will not silently fail.

***********************************
**3.1.0: Searching for parameters**

To use SRES to search for parameters, create or use an existing ranges file to select from. This ranges file determines how low and high each parameter may be set when searching for the best values. Call sres with this ranges file (via the command-line with -r or --ranges-file) and any other configurations desired. To send arguments to the simulation, enter -a or --arguments and then the arguments to pass. For example, "./sres -r input.ranges -a -x 2 -w 2 -y 1". Note that SRES automatically adds the required piping arguments, which take precedence over any given input file. We recommend running simulations in quiet mode (-q or --quiet) to avoid flooding the terminal with unnecessary output. The default arguments for SRES should be fine for most purposes but reducing the population size or number of generations is recommended if you're just testing out the functionality of the program since each generation takes a long time with a sufficiently sized population.

After each generation finishes, the best fitness score and its associated parameter set are printed to the terminal. The program continues until the given number of generations has been completed, regardless of the score each generation receives. You can specify to print sets that receive a good enough to an output file. Set the filename with -o or --good-sets-file and set the threshold with -G or --good-set-threshold where 0.0 is a perfect score and 1.0 is a complete failure.

**********************************
**3.1.1: Searching for gradients**

To use SRES-gradients to search for gradients, create a ranges file with three dimensions: start index, end index, and gradient factor. The start index is the cell index in the PSM at which to start applying each gradient. The end index is the cell index in the PSM at which to stop applying the gradient. The cell index 0 represents the start of the posterior and (simulation width - 1) represents the end of the anterior. The ranges file should be formatted as specified in Section 2.2.4.3. Here is an example file
```
gradient start  [0,49]
gradient end    [30,49]
gradient amount [5,500]
```

As usual, the strings "gradient start", "gradient end", and "gradient amount" are for readability and are ignored by the program. Although SRES creates parameters as doubles, the index parameters are always converted to integers and the start and end are swapped if the start is more anterior than the end.

Each parameter specified with -i or --gradient-index gets the SRES chosen gradient each time the simulation is run (i.e. for every member of the population for every generation). If you would like to add immutable gradients to each simulation as well, specify a gradients file to be appended to the SRES chosen gradients with -I or --gradients-file. To specify the parameter sets to run each simulation with, give the filename with -S or --parameter-set. If multiple parameter sets are given, their scores are averaged.

********************
**3.1.2: Using MPI**

Due to how computationally expensive SRES is, running hundreds of simulations for hundreds of generations on a single processor is impractical. To reduce the runtime, SRES can be distributed among multiple processors via MPI (Message Passing Interface). A full explanation of MPI can be found at http://www.open-mpi.org/. Basically, MPI allows multiple processes to communicate via messages, allowing multiple processes to work on different parts of the same task and report their results back to a process that organizes the results and doles out more tasks.

SRES uses MPI to distribute each simulation (i.e. population member) to a different process, each of which is ideally run on a different processor. This enables each generation to complete much faster than it would serially and allows productive evolutions to take place via large populations for many generations. Because libSRES, the library SRES uses, has separate files for MPI and non-MPI versions, SRES must be compiled with or without MPI and cannot run both versions with one compilation. For details on how to compile with MPI, see Section 1.

There are two possible commands to run SRES with MPI: mpiexec and mpirun. mpiexec is the standardized command and is usually the better choice. mpirun is implementation specific and may not work as expected. Since the correct command is environment specific, we recommend asking your server / cluster / network administrator. Using the command of your choice, set the number of processes to use via the command-line with "-np X", where X is the number of processes. This should be passed to mpiexec/mpirun and is automatically communicated to SRES. Here is an example call:
```
mpiexec -np 24 ./sres -d 45 -r input.ranges -g 1000 -p 200 -P 30 -a -x 8 -w 8 -y 4 -G 600 -M 8 -q
```

*************************
**3.2: Input and output**

***************************
**3.2.0: Methods of input**

The only interaction with a run is through input you give it before it starts and output it gives you after it ends. There are three aspects of input:
* Ranges
* Simulation calls
* Command-line arguments

*****************
**3.2.1: Ranges**

A ranges file must be passed via the command-line with -r or --ranges-file each time SRES is called. This ranges file defines the parameter space SRES looks throughout. If SRES fails to find parameter sets that receive high enough scores, consider generating a more restricted ranges file based on the best results so far. This may enable SRES to focus on more relevant regions in the space. Be aware, however, that the parameter space may contain peaks in unexpected locations; an alternative to reducing the space is to expand it in the hopes that the highest peaks are outside your current ranges. There is no known formal methodology for picking ranges for such an interconnected system beyond adopting experimental results found _in vivo_.

***************************
**3.2.2: Simulation calls**

Each SRES run needs to know where the simulation executable is located. The location can be specified via the command-line with -f or --simulation but defaults to "simulation" if no argument is given. Ensure that any executable you specify matches the ranges file and number of dimensions given and that you have permission to execute the file.

*********************************
**3.2.3: Command-line arguments**

Each command-line argument can be entered in a short or long form. Short forms begin with a single dash (-) followed by a single letter. Long forms begin with a double dash (--) followed by a word or phrase. Lower-case letters are considered distinct from their upper-case equivalents. The short and long forms are equivalent - short forms are for convenience and long forms are for clarity. The following is a comprehensive list of all command-line options and their uses:

-r, --ranges-file        [filename]   : the relative filename of the ranges input file, default=none
-f, --simulation         [filename]   : the relative filename of the simulation executable, default=../simulation/simulation
-o, --print-good-sets    [filename]   : the relative filename of the good sets output file, default=none
-G, --good-set-threshold [float]      : the worst score a set must receive to be printed to the good sets file, default=0.0
-d, --dimensions         [int]        : the number of dimensions (i.e. rate parameters) to explore, min=1, default=3
-P, --parent-population  [int]        : the population of parent simulations to use each generation, min=1, default=3
-p, --total-population   [int]        : the population of child simulations to use each generation, min=1, default=20
-g, --generations        [int]        : the number of generations to run before returning results, min=1, default=2000
-s, --seed               [int]        : the seed used in the evolutionary strategy (not simulations), min=1, default=time
-e, --printing-precision [int]        : how many digits of precision parameters should be printed with, min=1, default=6
-a, --arguments          [N/A]        : every argument following this will be sent to the simulation
-c, --no-color           [N/A]        : disable coloring the terminal output, default=unused
-v, --verbose            [N/A]        : print detailed messages about the program state
-q, --quiet              [N/A]        : hide the terminal output, default=unused
-l, --licensing          [N/A]        : view licensing information (no simulations will be run)
-h, --help               [N/A]        : view usage information (i.e. this)

SRES-gradients also contains the following arguments:

-R, --gradients-file     [filename]   : the relative filename of the gradients file that supplies base gradients to every generated gradients file, default=none
-i, --gradient-index     [int]        : the index of a parameter to apply gradients to, can be entered multiple times, min=1, max=# of dimensions, default=none
-S, --parameter-sets     [filename]   : the relative filename of the parameter sets input file, default=none
-n, --number-of-sets     [int]        : the number of parameter sets to run each simulation

Note that -i / --gradient-index can be entered multiple times to specify multiple parameters to apply a gradient to. The default number of dimensions in SRES-gradients is 3, not 45.

*****************************
**3.2.4: Input file formats**

**************************
**3.2.4.0: Ranges format**

For details on the format for a ranges file, see Section 2.2.4.3.

*****************************
**3.2.4.1: Gradients format**

SRES-gradients accepts a base gradients file whose gradients are appended to the gradients it generates. For details on the format for a gradients file, see Section 2.2.4.2.

******************************
**3.2.5: Output file formats**

***********************************
**3.2.5.0: Terminal output format**

SRES prints its latest results after each generation finishes. Each result consists of two lines. The first line contains two values: current generation and best fitness score. They are printed in the format "current generation: ?, best fitness: ?", where each question mark contains the values. Generation values are integral and start at 1. Fitness values are floating point where 0.0 is a perfect score and 1.0 is a complete failure. The second line contains the parameter set with the best fitness score so far. It is printed in the format "best individual: ?,?,?", with a value for each dimension / parameter (here represented as question marks).

Note that the precision of the printed parameters is not as high as the precision of the parameters stored in memory that are sent to the simulation. Therefore, if the system is not robust, rerunning a parameter set taken from the terminal output with the same configuration may return different results because of slightly different values. Because most biological systems are quite robust, if 10^-8 percent changes in parameters produces different results then there may be a flaw in the simulation used. The simulation presented in this package represents a robust system and does not produce different results because of reduced precision except in the corner cases in which the parameter is on the edge of acceptability and the change in precision pushes it over. Regardless, the output precision can be changed via the command-line with -e or --printing-precision (this affects terminal output and good sets files).

*****************************
**3.2.5.1: Good sets format**

A good sets file consists of a list of parameter sets. Its format is identical to a parameter sets input file (see Section 2.2.4.0) although it never contains blank lines or comments (lines beginning with "#"). Only parameter sets that receive a "good" score are added to the good sets file. By default, "good" means a perfect score of 0.0 but this can be changed via the command-line with -G or --good-set-threshold. Note that any parameter set that receives a "good" enough score is added to this file, even when the set is not the best fit in a generation and therefore may not appear in the terminal output.

***************************
**3.3: Modifying the code**

While the program and the library it is based on, libSRES, are designed to be flexible and easily configurable, it is impossible to anticipate every need. With that in mind, below is included some brief explanations of how to change key aspects of the program. While some of the recommendations are stylistic, following them ensures clean and consistent code and often avoids encountering problems difficult to anticipate without more knowledge of the code base.

****************************************
**3.3.0: Adding command-line arguments**

Adding a command-line argument requires changing the following structs/functions/macros:
* In source/structs.hpp:
  * input\_params
* In source/main.cpp:
  * usage
* In source/init.cpp:
  * accept\_input\_params
  * check\_input\_params

Because adding a command-line argument to SRES is virtually identical to adding one to the simulation, see Section 2.3.0 for details. The only difference is that because libSRES, the library SRES uses, contains an intricate hierarchy of functions that are not easily configurable, it is impractical to pass data specified by the user into the actual library code and thus into the simulation calls. Therefore, the input_params struct has been made global so that the simulation calling code can access it. While global variables are obviously dangerous and discouraged, this seems to be the simplest solution and should be kept in mind when adding command-line arguments.

*************************************************************
**3.3.1: Changing which arguments SRES sends to simulations**

Changing a simulation argument requires changing the following structs/functions/macros:
* In source/macros.hpp:
  * NUM\_IMPLICIT\_SIM\_ARGS
* In source/init.cpp:
  * init\_sim\_args
* In source/io.cpp:
  * simulate\_set

To change an implicit simulation argument, the macro _NUM\_IMPLICIT\_SIM\_ARGS_ must be changed to accomodate the change in arguments. Its current value is 6: the first argument must be the program name, specifying the two pipes takes 4 arguments, and there must be a NULL terminator to mark the end of the array. When changing the arguments, remember that an argument option that takes a value requires 2 arguments, not 1.

The implicit arguments are initialized in _init\_sim\_args_. Note that every string in the argument array is freed so constant strings must be copied onto the heap with _copy\_str_. Follow the format of the existing implicit arguments when adding more. Because each simulation needs unique pipe file descriptors, each simulation gets its own copy of the simulation arguments, copied in _simulate\_set_. Arguments that must be unique per simulation must be initialized in _simulate\_set_, not _init\_sim\_args_.

****************************************
**3.3.2: Adding input and output files**

Adding an input file requires changing the following structs/functions/macros:
* In source/structs.hpp:
  * input\_params
* In source/main.cpp:
  * main
* In source/init.cpp:
  * accept\_input\_params
  * check\_input\_params

Because adding an input or output file to SRES is virtually identical to adding one to the simulation, see Section 2.3.1 for details.

****************************
**3.3.3: Modifying libSRES**

Modifying libSRES is feasible but discouraged. Mostly this is because it works very well as is. However, we have made modifications (specifically to the terminal output format and memory tracking) with few complications. The only inconvenient aspect is that because of the intricate hierarchy of functions, it is impractical to pass data to certain areas of the code. We made the _input\_params_ struct to get around this so consider adding variables required by simulation calls to the struct if possible. Make sure to read all of the code before modifying any to account for unexpected dependencies.

******************************
**3.3.4: Other modifications**

We apologize if instructions on how to make your desired changes are not included this README. Our only advice for changing more fundamental aspects of the program is to be sure to check every source file to ensure that your changes propagate appropriately; while the code is designed to be as modular as possible, practical limitations have caused unexpected dependencies throughout the code. In other words, read all of the code before changing it and good luck!

4: Performing sensitivity analysis
----------------------------------

**4.0: Overview**

*********************************************
**4.0.0: Overview of Local Sensitivity Analysis**

The goal of Local Sensitivity Analysis is to quantify the degree to which a simulation is influenced by an input parameter. 

If Y is the output function (amplitude, period, etc), p\_j is the j’th parameter, and p’ is the nominal parameter set, then non-dimensional sensitivity S\_j can be evaluated by:

```
S_j = (p’_j/Y(p’)) * (dY(p’)/dp_j)
```

Here, d refers to taking the delta of a partial derivative, but is approximate because we are evaluating the output at finitely many points.

To normalize the sensitivities across the parameter set, for m parameters, the following gives a quantitative measure of ranking:
```
N_j = (S_j) / (sum_1:m{|S_j|})
```

******************************
**4.0.1: Overview of the program**

This program accomplishes the above calculations through the following steps:

* Read in the nominal parameter set from an input file.
* Based input values for the max percentage by which to perturb values and the number of perturbed points to simulate, calculate the (decimal) amount by which to perturb each parameter.
* For each parameter:
  * Create an array of values that can hold all points between \[nominal value - (nominal value * max percentage) \] and \[nominal value + (nominal value * max percentage) \].
  * Based on the nominal parameter value, fill in this array.
* Generate simulation data (e.g. oscillation features) by piping parameter sets to instances of the simulation program:
  * Send the nominal (unperturbed) set to get Y(p') as above.
  * For each individual parameter, send the nominal parameter set with the perturbed value inserted appropriately. This would be p\_jabove.
* To get the absolute sensitivity for each feature (i.e. output function) returned in the simulation data, perform the following for each parameter (j):
* Use the output values of the perturbed sets to calculate the finite difference, i.e. the derivative around the nominal parameter point (∆Y(p’)/∆p\_j above).
* Calculate the non-dimensionalized sensitivity (S\_j above) using the finite difference and the nominal parameter value and output ( p’\_j/Y(p’) above ).
* To get the normalized sensitivity for each feature, perform the following for each parameter (j):
  * Sum the non-dimensionalized sensitivity of the feature with respect to all parameters, (sum\_1:m{|S\_j|} above). 
  * Divide each parameter's sensitivity value by that sum. Optionally, multiply by 100 to give N\_j as a percentage.
* Write out these calculated values to files.

**************************
**4.1: Using sensitivity**

*****************************
**4.1.0: Command-line arguments**

The following arguments may be passed when calling the sensitivity program:

* -n, --nominal-file   [filename]   : the relative name of the file from which the nominal parameter set should be read, default=nominal.params
* -d, --sense-dir      [filename]   : the relative name of the directory to which the sensitivity results will be stored, default=sensitivities
* -D, --data-dir       [filename]   : the relative name of the directory to which the raw simulation data will be stored, default=sim-data
* -p, --percentage     [float]      : the maximum percentage by which nominal values will be perturbed (+/-), min=0, max=100, default=5
* -P, --points         [int]        : the number of data points to collect on either side (+/-) of the nominal set, min=1, default=10
* -c, --nominal-count  [int]        : the number of nominal sets to read from the file, min=1, default=1
* -k, --skip           [int]        : the number of lines in the nominal sets file to skip over (excluding comments), min=0, default=0
* -s, --random-seed    [int]        : the seed to generate random numbers, min=1, default=generated from the time and process ID
* -l, --processes      [int]        : the number of processes to which parameter sets can be sent for parallel data collection, min=1, default=2
* -y, --recycle        [N/A]        : include this if the simulation output has already been generated for exactly the same configuration used now, default=unused
* -g, --generate-only  [N/A]        : generate oscillations features files for perturbed parameter values without calculating sensitivity, default=unused
* -z, --delete-data    [N/A]        : delete oscillation features data, specified by -D or --data-dir, when the program exits, default=unused
* -q, --quiet          [N/A]        : hide the terminal output, default=unused
* -e, --exec           [directory]  : the relative directory of the simulation executable, default=../simulation/
* -a, --sim-args       [N/A]        : arguments following this will be sent to the simulation
* -l, --licensing      [N/A]        : view licensing information (no simulations will be run)
* -h, --help           [N/A]        : view usage information (i.e. this)

**********************************
**4.1.1: Input file format**

Sensitivity analysis requires at least one nominal parameter set as input. For strictly local analysis, this set should be one that successfully expresses the features desired for the system. For more global analysis it is recommended the nominal parameter sets be plentiful and varying -- using well-distributed parameter sampling algorithms such as Latin Hypercube Sampling is preferable. However, the nominal parameter sets should not yield undefined behavior (e.g. infinite or non-numerical feauture values) as this is likely to render sensitivity calculations futile. Assuming the appropriate nominal parameter sets have been collected, the following describes the format in which they may be passed to the sensitivity program:

A parameter sets file consists of a list of parameter sets. Each parameter set is placed on its own line, with each parameter separated by a comma. There is not a comma after the final parameter set. Each parameter is included and has a floating point or integral value. Blank lines and lines beginning with "#" are ignored. There must be at least one parameter set per file. There is no limit to the number of parameter sets allowed in a single file. Recall that you may specify which parameter set to start with using '-k' or '--skip' and how many sets to use with '-c' or '--nominal-count'.

The following three lines represent an example file:

```
# This is a comment
63.049748,34.955167,0,42.993889,0,44.600336,0.429514,0.453927,0,0.279120,0,0.335253,45.227915,30.430273,0,46.391646,0,22.217098,0.157011,0.338070,0,0.307667,0,0.274870,0.019982,0.000370,0,0.026413,0,0.008877,0,0.027406,0,0,0,0,0.005284,0,0,0.219352,0.032981,0,0.051730,0,0.175033,0,0.116019,0,0,0,0,0.006626,0,0,0.278811,0.269840,0,0.279624,0,0.285013,0,0.238326,0,0,0,0,0.242239,0,0,11.017495,9.139970,0,0.000000,0,7.329024,0.426847,1.210969,0,0.596138,0,12.461520,361.441083,485.384224,0,0,0,0,499.577384
63.000647,33.757737,0,43.849951,0,47.332097,0.434420,0.455262,0,0.274844,0,0.346678,43.338772,30.019011,0,54.822609,0,25.281511,0.141475,0.315663,0,0.345098,0,0.269280,0.018546,0.003612,0,0.028153,0,0.008334,0,0.025200,0,0,0,0,0.011394,0,0,0.170959,0.041615,0,0.044836,0,0.237797,0,0.248760,0,0,0,0,0.017808,0,0,0.280718,0.310334,0,0.343655,0,0.210100,0,0.233876,0,0,0,0,0.214772,0,0,10.916983,9.443056,0,0.000000,0,7.742257,0.445980,1.035695,0,0.578762,0,12.446215,231.836670,477.034572,0,0,0,0,540.815524
```

**********************************
**4.1.2: Simulation output file format**

This program reads results of simulations from the output files created by the simulation program. If the sensitivity program is run with the standard simulation program, this file format is already created properly and can be read by the simulation without any user modifications. The creation of this format is explicitly defined in section 2.2.5.3, but a summary of what is necessary for the sensitivity program to read such a file properly is as follows:

Some requirements of the simulation output file:

* The number of features can be arbitrary, but:
* The maximum number of features that can be read is set by the macro _MAX\_NUM\_FEATS_ in source/macros.hpp and is 150 by default.
* All values must be comma-seperated with no spaces.
* The names line must contain a name for every feature.
* The last value in non-name lines must be followed by a comma, but can have any string after that before the new line.
* If a trailing column should be ignored it should not be given a name in the first line.

The following three lines represent an example file:

```
set,post sync wildtype,post per wildtype,post amp wildtype,post per wildtype/wt,post amp wildtype/wt,
0,0.999999999999999888977697537484,29.8797435897436045593167364132,56.0505555846975624945116578601,1,1,
1,1,30.2323076923076676791879435768,166.255079755418790909970994107,1,1,
```

**********************************
**4.1.3: Sensitivity output file format**

The final output of the sensitivity calculation comes in two files with the same format. Both files are stored in a directory that is by default named "SA-data/" though this can be modified with the commands '-d' or '--sense-dir'. In this directory there will be two files for every nominal set that was sent to the sensitivity program: "LSA\_n" and "normalized\_n" where "_n" refers to the nth nominal sent. The former file contains the absolute, dimensionless sensitivity values while the latter contains the normalized sensitivities as percentages (S\_j and N\_j as described in section 2.0). 

The format of these files is consistent with the format of the simulation output file format, with the sensitivity/normalized-senstivity values in place of the original feature values. The one difference is that the first column will refer to which parameter the sensitiviy value is for. 

The following lines are an example of an absolute sensitivity output file:

```
parameter,post sync wildtype,post per wildtype,post amp wildtype,post per wildtype/wt,post amp wildtype/wt,
0,-0.00743137894005018763421421823523,0.00504250156260815669134744965163,1.00289462409731400249768284993,0,0,
1,-0.00918263623745127141595467890056,0.0275346684530147697844704168801,-0.15316488780164577709896889246,0,0,
```

The following line are an example of a normalized sensitivity output file:

```
parameter,post sync wildtype,post per wildtype,post amp wildtype,post per wildtype/wt,post amp wildtype/wt,
0,1.44836748675782578388293586613,0.422484791108310275831172475591,18.6174946777228882410781807266,0,0,
1,1.78968558545324518682662073843,2.30698563107465703936327372503,2.84331615201126997050096179009,0,0,
```

*************************************
**4.1.4: Example program call**
 
As an example, the following may be a valid call to program:

```
~/sensitivity-analysis/sensitivity -c 2 -k 4 --processes 6 --percentage 100 -P 10 --random-seed 112358 -n ~/sensitivity-analysis/nominal.params -d	~/sensitivity-analysis/sensitivity_data -D	~/sensitivity-analysis/simulation_data -e ~/sogen-deterministic/simulation --sim-args -u ~/sogen-deterministic/input.perturb
```

where the short and long names may be interchanged with their long/short counterparts.

Some notes about the assumption made by this command:

* This example assumes the file system supports the "~/" path prefix -- for safety, it may be necessary to use full paths for file and directory arguments.
* Becuase '-c 2' is included, the program will look for two nominal parameter sets to run. If the file specified by '-n' does not contain two parameter sets, the program will exit with an error message.
* The option '-k 4' specifies that the fourth line of the file should contain the first nominal parameter to use -- if there are less than four lines in the file the program will exit with an error message.
* The option '--processes 6' states that six instances of the simulation program (not including sensitivity itself) may be run simultaneously. Each instance will be given the perturbed parameter sets for a single perturbed parameter. If your system has >= 6 processors, these simulations should run simultaneously. However, even if fewer processors exist the program will not fail, there will just be less effective parallelization.
* Running more processes requires more system memory, so it is possible that, even if the quantity specified by '--processes' is less than the number of system processors, system memory may create a bottleneck. Again, the program will not fail, but it will have less effective parallelization.
* If any of the files specified do not exist, an appropriate error message will be returned. In such a case, re-check the path names and consider using full paths.

*************************
**4.2: Modifying the code**

While the program is designed to be flexible and easily configurable, it is impossible to anticipate every need. With that in mind, below is included some brief explanations of how to change key aspects of the program. While some of the recommendations are stylistic, following them ensures clean and consistent code and often avoids encountering problems difficult to anticipate without more knowledge of the code base.

**************************************
**4.2.0: Adding command-line arguments**

Parsing of command-line argument is handled entirely in 'sensitivity-analysis/source/init.cpp' in the function:

```
void accept_params (int num_args, char** args, input_params& ip)
```

If you would like to add a comand-line argument to the senstivity program simply add an "else if" statement to the main conditional like so:

```
...
} else if (strcmp(option, "-?") == 0 || strcmp(option, "--your-option") == 0) {
	ensure_nonempty(option, value);
	your_variable = value;
} else ...
```

You should replace 'your-option' with the command you would like to add and '?' with a single-character short version. Please ensure that these strings are unique, i.e. they do not conflict with any previously assigned commands. Please do not ovewrite the default commands.

You should also replace 'your\_variable' with the variable/object you would like to use to store the value passed as an argument. The most convenient method of passing this variable throughout the program is by making it an instance variable of the input\_params struct. To do this, simply open 'sensitivity-analysis/source/init.hpp' and add your instance variable to the input\_params struct. You may also want to set a default value for you variable, in which case you should add this assignment to the input_params constructor and destrctor. For example:

```
struct input_params{
	TYPE your_variable_name;
	bool sim_args;
	bool quiet;	
	...
	input_params(){
		your_variable_name = default_value;
		quiet = false;
		sim_args = false;
		...
	}
	~input_params(){
		your_destruction_method( your_variable_name );
		if(nominal != NULL) delete[] nominal;
	}
};
```

You may then reference your variable in all functions that take 'input_params& ip' as a parameter by entering:

```
ip.your_variable_name
```

**************************************
**4.2.1: Modifying the simulation program**

The code has been made with as much modularity as possible. If you would like to use an alternative simulation program you have two options:

**Low-Modification Option:**

Make a simulation program executible that takes the the same command-line arguments as the default simulation. This program should also create the same output file format as is specified in section 4.3.2. 

While this requires you to conform to very specific bounds, if you create a simulation program in this way you will only need to call 'sensitivity-analysis/sensitivity' with the argument '-e' or '--executable' followed by your simulation program and the sensitivity program will run with the appropriate behavior.

**High-Modification Option:**

If you are willing to modify the source code of 'sensitivity-analysis/sensitivity' it is possible create custom simulation program calls and file formats by modifying source/io.cpp and source/io.hpp. The functions that would need to be rewritten for you own needs are as follows.

For changing how simulations are called:

* void simulate\_samples(int, input_params&, sim_set&)
* void simulate\_nominal(input_params&)

For changing how simulation results are read from file:

* double** load\_output(int, int*, char*, char***)

For changing how sensitivity calculations are written to file:

* void write\_sensitivity(int, int, char **, double **, char*)

Please store a backup of the original package before any modifications and proceed with caution.

5: Analysis and parsing scripts
-------------------------------

**5.0: Running simulations over multiple seeds (run-multiple-seeds.py)**

*******************
**5.0.0: Overview**

run-multiple-seeds.py tests parameter sets for robustness by running them with multiple seeds. It should be run from a cluster that supports PBS. By giving it the number of nodes, processors per node, and the number of seeds to run, it runs multiple simulations with different seeds. By using check-robustness.py, these runs can be checked for robustness.

*********************************
**5.0.1: Command-line arguments**

* -i, --input-file   [filename]  : the relative filename of the parameter sets to run the simulation with, required
* -n, --num-params   [int]       : the number of parameters to be tested for robustness, required
* -p, --pars-per-job [int]       : the number of parameters to be run for each job, required
* -d, --directory    [directory] : the relative path of the directory in which all the output files will be placed, required
* -s, --simulation   [filename]  : the relative filename of the simulation to run, required
* -S, --seeds        [int]       : the number of seeds to run the simulation for, required
* -a, --arguments    [N/A]       : every argument following this will be sent to the simulation
* -h, --help         [N/A]       : view usage information (i.e. this)

********************************
**5.0.2: Example program calls**

```
python run-multiple-seeds.py --input-file good.params --seeds 10 --num-params 5000 --pars-per-job 500 --directory output --simulation ../simulation/simulation --arguments -x 10 -w 10 -y 6 -m 600 -G 600 -s 2000 -u input.perturb
```

Note that the simulation arguments be passed in with -a or --arguments should match the arguments that validated the given sets.

To run this without a cluster, the PBS job section of the script must be modified and one processor on one node should be specified like this:

```
python run-multiple-seeds.py --input-file good.params --seeds 10 --num-params 5000 --pars-per-job 1 --directory output --simulation ../simulation/simulation --arguments -x 10 -w 10 -y 6 -m 600 -G 600 -s 2000 -u input.perturb
```

**************************************************
**5.1: Checking robustness (check-robustness.py)**

*******************
**5.1.0: Overview**

check-robustness.py counts how many parameter sets pass with a threshold number of seeds. This script is usually run after running run-multiple-seeds.py but can also be run independently. It takes a directory containing scores files in the format scoreX-Y.csv, where X is the seed and Y is the parameter set. The sets that pass enough of the seeds (specififed by -t or --threshold) are printed to the specified output file (with -o or --output-file).

*********************************
**5.1.1: Command-line arguments**

* -S, --seeds       [int]      : the number of seeds for which the parameters were run
* -n, --num-params  [int]      : the number of sets that were tested for robustness
* -o, --output-file [filename] : the name of the output file to which the robust sets will be printed
* -f, --num-files   [int]      : the number of scores files which were created by the robustness runs
* -d, --directory   [filename] : the relative path of the directory in which the scores files are located
* -m, --max_score   [int]      : the score obtained by a passing parameter set
* -i, --input_file  [filename] : the relative path of the file containing the original paremeter sets which are being tested for robustness
* -t, --threshold   [int]      : the number of seeds that a parameter has to pass in order to be deemed robust
* -h, --help        [N/A]      : view usage information (i.e. this)

********************************
**5.1.2: Example program calls**

The following accepts only parameter sets that pass at least 18/20 seeds:

```
python check-robustness.py --seeds 20 --num-params 5000 --output-file robust.params --num-files 10 --directory output --max-score 85 --input-file good.params --threshold 18
```

********************************************************************************
**5.2: Running simulations over multiple gradients (run-multiple-gradients.py)**

*******************
**5.2.0: Overview**

run-multiple-gradients.py brute-force searches different gradient configurations. While we have found SRES-gradients very successful in searching for biologically realistic gradients, this script provides a more simplistic method of finding them by simply searching all combinations within the given range. We recommend trying SRES-gradients first but this is a useful backup.

*********************************
**5.2.1: Command-line arguments**

* -s, --sets           [filename]   : the relative filename of the parameter sets to run the simulations with, required
* -g, --base-gradients [filename]   : the relative filename of the gradients to run each simulation on top of the changing ones, default=none
* -s, --starts         [int list]   : a comma separated list specifying which cell indices to start gradients at, required
* -e, --ends           [int list]   : a comma separated list specifying which cell indices to end gradients at, required
* -f, --factors        [float list] : a comma separated list specifying which factors to adjust gradients to, required
* -S, --simulation     [filename]   : the relative filename of the simulation to run, default=simulation
* -a, --arguments      [N/A]        : every argument following this will be sent to the simulation

********************************
**5.2.2: Example program calls**

```
python run-multiple-gradients.py -s robust.params -g input.gradients --starts 0,5,10 --ends 10,15,20,25 --factors 5,25,50,75,100 -S ../simulation/simulation --arguments -x 10 -w 10 -y 6 -m 600 -G 600 -s 2000 -u input.perturb
```

This script should be run from a cluster that supports PBS. input.gradients should contain any gradients that should be applied to all simulations (i.e. gradients that you know work). Every combination of starts, ends, and factors are tried.

*******************************************
**5.3: Refining ranges (refine-ranges.py)**

*******************
**5.3.0: Overview**

refine-ranges.py refines existing ranges into more precise ones based on parameter sets. Possible biologically realistic ranges are quite wide. After running enough parameter sets that produce valid results, more precise ranges may be inferred by creating ranges based on the average values for each parameter in the sets. More precisely, by giving this script a list of good parameter sets, your current ranges, and a filename to place the new, more refined ranges, this script finds the mean and standard deviation of every parameter and creates a new range by taking the values X standard deviations from the mean, where X is a positive integer you specify. However, if the new range is not as precise as the one found in the given current ranges file, the current range is used instead.

*********************************
**5.3.1: Command-line arguments**

* -s, --sets           [filename] : the relative filename of the parameter sets to run the simulations with, required
* -c, --current-ranges [filename] : the relative filename of the ranges file to base new ranges on, required
* -n, --new-ranges     [filename] : the relative filename of the ranges file to stores the new ranges in, required
* -d, --standard-dev   [int]      : how many standard deviations to stray from each mean range, default=2
* -r, --round-to       [int]      : how many digits to round each new range to, default=5
* -h, --help           [N/A]      : view usage information (i.e. this)

********************************
**5.3.2: Example program calls**

Assuming there is a parameter sets file named good.params and a ranges file named current.ranges, the following command refines the ranges using the parameter sets, using two standard deviations from the mean and (by default) rounding to 5 digits:

```
python refine-ranges.py -s good.params -c current.ranges -n new.ranges -d 2
```

*********************************************************
**5.4: Plotting oscillation features (plot-features.py)**

*******************
**5.4.0: Overview**

plot-features.py plots the period and/or amplitude change from posterior to anterior for the wildtype and every mutant. After running the simulation with the -A option, the simulation creates files in the directory specified by the -D option. These files are called set\_X\_\Y\_\Z\.feats, where X is the parameter set index, Y is the concentration for which to create plots (currently mh1 or mdelta), and Z is the oscillation feature you want to plot: "period" or "amplitude". The script can plot either the period, the amplitude or both of the features, depending on which command line arguments you provide it. You can specify from how many parameter sets you want to extract data for the plotting. The script will then divide the width of the PSM into posterior and 8 slices of the anterior. For each of these sections, the mean and standard error of the values are calculated and then plotted onto an errorbar plot. This process is repeated for all the mutants that were simulated. The command line arguments allow for more customization of the plots. If you want to change the number of data points on the graph, you should change the variable 'buckets' on line 76. 

*********************************
**5.4.1: Command-line arguments**

plot-features.py takes 7 arguments, described below:

```
python plot-features.py <directory with period files> <number of parameter sets> <name of image> <feature(s) to plot> <directory to save image(s)> <width of the posterior> <name of excel file>
```

Directories can be absolute or relative. The period files directory should be structured as it is produced by a standard simulation. The image name should not include the file extension, which is .png. The feature(s) to plot can be "period", "amplitude", or any other string to plot both. The width of the posterior should be in cells. The excel file name should not include the file extension, which is .csv.

********************************
**5.4.2: Example program calls**

```
python plot-features.py periods 10 period period results 60 period-data
```

***********************************************************
**5.5: Plotting synchronization (plot-synchronization.py)**

*******************
**5.5.0: Overview**

plot-synchronization produces plots (and their accompanying data) of cell synchronization. Cell values are averaged by column so the spatial relationships between columns are displayed.

*********************************
**5.5.1: Command-line arguments**

plot-synchronization takes 5 arguments, described below:

```
python plot-synchronization.py <directory with synchronization files> <number of parameter sets> <directory to store the image> <name of image> <name of excel file>
```

Directories can be absolute or relative. The synchronization files directory should be structured as it is produced by a standard simulation. The image name should not include the file extension, which is .png. The excel file name should not include the file extension, which is .csv.

********************************
**5.5.2: Example program calls**

```
python plot-synchronization.py syncs 5 results sync sync-data
```

*************************************************
**5.6: Plotting cells over time (plot-cells.py)**

*******************
**5.6.0: Overview**

plot-cells produces plots (and their accompanying data) of cell mRNA values over time. The x-axis is time (in minutes) and the y-axis is the level of mRNA (whichever type was printed to the file). Each cell receives its own plot-line so the graph becomes less useful if too many cells are plotted on top of each other.

*********************************
**5.6.1: Command-line arguments**

plot-cells takes 4 arguments, described below:

```
python plot-cells.py <file with concentration levels> <directory to store plots> <plot name> <step size>
```

The concentration levels file and plot storing directory can be absolute or relative. The concentration levels directory should be structured as it is produced by a standard simulation. The plot name should not include the file extension, which is .png.

********************************
**5.6.2: Example program calls**

```
python plot-cells.py set.cons plots her1levels 0.01
```

**************************************************************
**5.7: Plotting sensitivity analysis (plot-sensitivities.py)**

*******************
**5.7.0: Overview**

plot-sensitivities.py includes two plotting methods: sensitivity bar graphs, and output value scatter-line plots.

0: Sensitivity bar graphs

These bar graphs display the sensitivity of a simulation output to each simulation parameter. The script generates these graphs by:

0: making system calls to sensitivity (repeatedly if necessary),
1: parsing the sensitivity output files,
2: averaging the sensitivity values for each parameter across nominal parameter sets,
3: using the average to calculating the standard error for each parameter,
4: creating a bar graph for each simulation output in which the height of each bar is the average sensitivity to a particular input parameter accross nominal parameter sets and the error bars are the standard error values.

This method will create two images for each feature: a bar graph of the normalized sensitivities and a bar graph of the absolute sensitivities. These graphs will be stored in files titled "\[feature name\].png" and "absolute\_\[feature name\].png", respectively. The feature names will come from the original simulation output data, with the exception that all special characters will be converted to underscores in the file name (though the full feature name will be used as the title on the graph, verbatim).

1: Scatter-Line plots

These are plots of points connected by lines where each line is a single nominal parameter set. The script generates these plots by:

0: making system calls to sensitivity for every nominal parameter set with the '--generate-only' argument so that the sensitivity values are not calculated,
1: parsing the simulation output files, i.e. the oscillation features of the zebrafish somitogenesis simulations,
2: For every simulation parameter, plotting the simulation output value at successive perturbations of the parameter -- each output value is plotted as the perturbed value divided by the output vale of the nominal (unperturbed) parameter set.

These plots display the ratio between an simulation output value at a perturbed parameter value. This method will create an image for every parameter's effect on every simulation output feature. The files are titled "\[index of parameter\]on\[index of feature\]\_\[feature name\].png". For example, the file "10on2\_posterior\_amplitude\_wildtype.png" would refer the the effect of parameter #10 on feature #2 which is named "posterior amplitude wildtype". As before, feature names come from the original simulation output with special characters (e.g. white space) replaced with underscores.

*********************************
**5.7.1: Command-line arguments**

The following arguments may be passed in a command-line call to the python script. Some of them are the same as for sensitivity, but most are distinct.

* -n, --nominal-file     [filename]  : The file to use for getting nominal parameter sets, default=~/sensitivity-analysis/sens/sensitivity-analysis/sensitivity-analysis/nominal.params
* -d, --dir              [filename]  : the directory to put all plots in, default=all-sensitivities
* -D, --data-dir         [filename]  : the directory in which to store raw sensitivity data, default=sense-for-plot
* -o, --output           [filename]  : the name of file(s) to use for plot images, default=sensitivity
* -j, --job-name         [string]    : the name that should be used for the pbs job, default=PLOT_SA
* -C, --cluster-name     [string]    : the name of the cluster on which to run simulations, if any, default=none
* -y, --ymin             [float]     : set the minimum value for the axes of the plot, defualt=auto-scaling that varies between graphs
* -Y, --ymax             [float]     : set the maximum value for the axes of the plot, defualt=auto-scaling that varies between graphs
* -N, --nodes            [int]       : the number of nodes to be utilized. Runs are done locally unless -N > 1, default=1
* -f, --feature          [int]       : index of feature, if only one feature should be plotted. Omit if all features should be plotted, default=unused
* -p, --percent          [int]       : max percentage by which to perturb nominal value, default=20
* -P, --points           [int]       : number of data points to collect between nominal and nominal + (max percent * nominal), default=4
* -l, --ppn              [int]       : processors per node, i.e. processes to use in parallel for the analysis, default=2
* -c, --nominal-count    [int]       : the number of nominal sets to use for sensitivity calculations, default=1
* -e, --exec             [directory] : the path of the executable for performing sensitivity analysis, default=~/sensitivity-analysis/sens/sensitivity-analysis/sensitivity-analysis/sensitivity
* -s, --sim              [directory] : the path of the executable for running simulations, default=../simulation/simulation
* -g, --graph            [N/A]       : just generate graphs without running simulations, assuming the appropriate files have already been generated, default=unused
* -E, --elasticity       [N/A]       : include this to plot (oscillation features)/(nominal feature value) as a scatter plot connected by lines (instead of sensitivity bar graphs)
* -a, --args             [N/A]       : arguments following this will be sent to the simulation
* -h, --help             [N/A]       : view usage information (i.e. this)

********************************
**5.7.2: Example program calls**

To generate bar graphs of the sensitivity of all feautures to all parameters, the following may be a valid call to the script:

```
python plot-sensitivity.py -n ~/sensitivity-analysis/nominal.params -d ~/sogen-scripts/sensitivity-plots -C biomath --percent 10 -P 2 -N 5 -c 10 --ppn 6 --exec ~/sensitivity-analysis/sensitivity --sim ~/sogen-deterministic/simulation --args -s 112358 -a -M 1 -u ~/sogen-deterministic/input.perturb
```

To generate line-connected scatter plots of oscillation features for various perturbations of input parameters:

```
python plot-sensitivity.py --elasticity -n ~/sensitivity-analysis/nominal.params -d ~/sogen-scripts/sensitivity-plots -C biomath -p 100 --points 10 -N 5 -c 10 --ppn 6 --exec ~/sensitivity-analysis/sensitivity --sim ~/sogen-deterministic/simulation --args -s 112358 -a -M 1 -u ~/sogen-deterministic/input.perturb
```

***********************************************
**5.8: Plotting densities (plot-densities.py)**

*******************
**5.8.0: Overview**

plot-densities.py prints a spatiotemporal figure depicting cell oscillations through time, similar to Figure 4 in Uriu _et al._ 2009. It produces figures in which the x-axis is time and the y-axis is cell position, with colors representing different concentration levels (darker colors represent higher levels). The script uses concentration files (see Section 2.2.5.1) but does not plot data before growth since it is irrelevant to the figure. Although most command-line options have defaults and thus do not need to be entered explicitly, be sure that each option matches the given concentration file.

*********************************
**5.8.1: Command-line arguments**

* -c, --cons-file        [filename] : the relative filename that stores the concentrations, required
* -f, --figure-name      [string]   : the name to give the figure, not including its extension, default=densities
* -i, --image-format     [string]   : the format with which to save the image, default=png
* -w, --image-width      [int]      : the image width in pixels, default=1000
* -h, --image-height     [int]      : the image height in pixels, default=250
* -G, --steps-til-growth [int]      : the number of steps until growth starts, default=60000
* -S, --steps-to-split   [int]      : the number of steps each cell split takes, default=600
* -n, --initial-width    [int]      : the initial number of cells in the PSM, default=10
* -g, --granularity      [int]      : the number of time steps to jump between each value used, default=1
* -s, --start-step       [int]      : the time step to start at, relative to the steps until growth, default=0
* -e, --end-step         [int]      : the time step to end at, relative to the steps until growth, default=60000
* -h, --help             [N/A]      : view usage information (i.e. this)

********************************
**5.8.2: Example program calls**

```
python plot-densities.py -c set.cons -i png -w 1000 -h 250
```

This example uses mostly default values but be sure your call matches the given concentration file, as mentioned above. Colors can be changed by editing the _shades_ array in _main_.

*************************************************************
**5.9: Plotting tissue snapshots (plot-tissue-snapshots.py)**

*******************
**5.9.0: Overview**

plot-tissue-snapshots.py produces an image representing the tissue concentration levels for every time step in a concentration levels file. Each cell is represented as a hexagon (as in a standard simulation) with a color range from dark red to white. Darker shades represent higher levels of concentration. Snapshot files are named sequentially, one for each time step in the concentrations file, and can therefore easily be turned into a movie with ffmpeg.

*********************************
**5.9.1: Command-line arguments**

plot-tissue-snapshots.py takes 2 arguments, described below:

```
python plot-tissue-snapshots.py <file with concentration levels> <directory to store images>
```

The file and directory can be relative or absolute.

********************************
**5.9.2: Example program calls**

```
python plot-tissue-snapshots.py set.cons snapshots
```

6: Debugging, profiling, and memory tracking
--------------------------------------------

**6.0: Debugging**

Due to the complexity of this code base, we have implemented tools to aid in debugging. While the current versions of the code produce no compiler warnings, memory leaks, or memory access errors, it is virtually impossible to write bug-free code. Additionally, this code is meant to be expandable and configurable not only through command-line arguments and input files but through code modification as well. Therefore, it is worth including basic debugging information.

The code has been extensively commented with the hope that others can understand it without having to exert an unreasonable amount of effort. Each function includes a comment header above it detailing the structure and purpose of the function. Unavoidable counterintuitive and tricky behavior is recorded in the notes sections as much as possible. The use of external dependencies has been minimized so that there are as few black boxes as possible. In fact, the simulation and sensitivity analysis require nothing but standard C and C++ libraries and SRES requires only those and libSRES.

If simulations produce unexpected results that you suspect are erroneous, there is a small suite of debugging functions in source/debug.cpp that can be injected anywhere in the simulation to help understand its current state. Each function is explained in its comment header.

If a program is causing segmentation faults, crashing unexpectedly, etc., we recommend running it through Valgrind, which can be downloaded at http://valgrind.org/downloads/current.html. Valgrind reports, among many other things, memory leaks and memory access errors and provides a variety of tools for identifying and fixing these kinds of problems. The current code produces no Valgrind errors so if a modified version does then the errors are almost certainly caused by the modifications. If a program is compiled with debug mode on (see Section 1), you can pass "--db-attach=yes" as a command-line argument to Valgrind (remember to place it before the executable so it is not interpreted as an argument to the executable) and when Valgrind encounters an error it will pause and give the option to enter the GDB debugger. GDB has in-program help and more information about it can be found at http://www.gnu.org/software/gdb/.

******************
**6.1: Profiling**

Simulations take time and identifying performance bottlenecks can save you days of simulation time. We have spent a good amount of time optimizing the program but it can always be faster and any modifications may introduce their own overhead or decrease the performance of existing features. Therefore, compiling a program on Linux with profiling mode on (see Section 1) causes an execution of the program to produce a "gmon.out" file. This file, when analyzed with gprof, reveals how long the program spends in each function, among other things. This is a useful method for identifying performance bottlenecks that can then be optimized. To analyze the file with gprof, call "gprof simulation gmon.out X", where simulation is the simulation executable and X is the file to write the profile information to.

************************
**6.2: Memory tracking**

While SRES and sensitivity analysis require less than a megabyte of memory, the simulation can take gigabytes given a large enough cell tissue and number of genes. While manually adding up the memory requirements of the largest data structure used can give a broad estimate of the memory footprint, having a more exact number can be important, especially in the context of clusters with limited memory and long submission queues. To achieve this, all programs in this package can be compiled with memory tracking (see Section 1 for compilation instructions). The memory tracker is a very basic but easy way to calculate memory usage on the fly. Compiling with it adds a _print\_heap\_usage_ function that can be called at any time. This function reports the current heap usage, which is the sum of allocations not yet deallocated, and the total heap usage, which is the sum of all allocations regardless of deallocations.

For the memory tracker to work, calls to _malloc_ must be replaced with calls to _mallocate_ and calls to _free_ with calls to _mfree_. _new_, _new[]_, _delete_, and _delete[]_ are automatically overrided. The memory tracker adds a small amount of memory to the heap but this is not included in the messages of _print\_heap\_usage_. Because _malloc_ introduces overhead for each allocation and often allocates slightly more than needed for technical reasons, the memory tracker may report a slightly smaller footprint than is actually the case but the difference is very small.

Note that _malloc_ is not the only function that allocates memory on the heap so if modifications use other functions like _calloc_, _realloc_, or _strdup_, those functions also require wrappers to work properly with the memory tracker. Wrappers for _calloc_ and _realloc_ can be found in SRES's source/memory.cpp. _copy\_str_, found in each program's source/init.cpp, serves as a comparable equivalent to _strdup_.

7: Case study
-------------

**7.0: Prerequisites**

This case study is intended to provide a concrete example of how to use the programs in this package. We assume you have read the rest of the README and have a basic understanding of the programs in this package. This case study assumes you have already downloaded and compiled the programs as outlined in Section 1.

This case study should allow you to follow the same process we used to obtain our results. However, it is not a comprehensive guide to the programs and a more sophisticated understanding may be necessary for your own analysis.

***********************************************
**7.1: Find biologically realistic parameters**

First, determine the size of your parameter space. This includes both determining how many parameters are relevant to the system and the ranges from which values can be selected. You should choose the narrowest ranges with experimental evidence behind them since larger parameter spaces take more time to search. If a parameter's biological value is known, it should not be considered a parameter for this search. We'll assume you've chosen 45 dimensions, the current number we use.

Having established the required parameters and their associated ranges, create a ranges file in the format described in Section 2.2.4.3. We'll assume it is called input.ranges and is located in the sres directory. If you have access to many processors (e.g. a cluster) then we recommend using MPI as detailed in Section 3.1.2. We'll assume you have 12 processors available and therefore use 12 processes when running MPI. We'll also assume that the simulation executable is in the simulation directory and the SRES executable is in the sres directory. Your directory structure should look like this:

|Zebrafish
|__ simulation
|____ simulation*
|__ sres
|____ sres*
|__ scripts
???

Items with a \* represent executables.

*********************************
**7.1.0: Find posterior parameters**

Initially we will run the simulation with the width of the posterior, -w or --initial-width, equal to the width of the tissue, -x or --total-width, and with a time, -m or --total-time, of 600 minutes. We focus on posterior conditions first because they are more easily satisfied and therefore can help us narrow the parameter space. Once we have found an adequate number of parameter sets that satisfy this simplified case we will narrow our parameter space and perform further runs of SRES with a full (posterior and anterior) simulation. To add perturbations to the simulation, a necessary component for biological accuracy, create a perturbations file, "input.perturb", with the following content:

```
# 5% perturbations for all rates, starting at 0 and ending at 44
5 0 44
```

The following command, when in the sres directory, runs SRES for 1750 generations with a total population of 20, 3 of which are parents:

```
mpiexec -np 12 ./sres -f ../simulation/simulation -d 45 --ranges-file input.ranges --total-population 20 --parent-population 3 -g 2000 -s 1234 -a -x 10 -w 10 -y 6 -m 600 -G 600 -s 2000 -M 6 -u input.perturb --quiet
```

Remember, you can see the full list of command-line arguments for SRES in Section 3.2.3 and for the simulation in Section 2.2.3. If you're not using MPI, use the following command:

```
./sres -f ../simulation/simulation -d 45 --ranges-file input.ranges --total-population 20 --parent-population 3 -g 2000 -s 1234 -a -x 10 -w 10 -y 6 -m 600 -G 600 -s 2000 -M 6 -u input.perturb --quiet
```

Note that we recommend reducing the population size and number of generations for the serial version. It takes a very long time!

The previous commands print their output to the terminal window. If you want to view the results in a file you can redirect standard output in the following way in most shells:

```
mpiexec -np 12 ./sres ... > output.results
```

Replace "..." with the rest of the appropriate arguments. If you want to store only parameter sets that receive a good enough score (e.g. 0.05), run the program with the following arguments:

```
mpiexec -np 12 ./sres -o good.params -G 0.05 ...
```

Replace "..." with the rest of the appropriate arguments. Note that -G or --good-set-threshold sets the threshold to the given number. The output file, here "good.params", is printed in the same format as a parameter sets input file that can be passed directly to the simulation if desired. For more information about the parameter sets format, see Section 2.2.4.0.

After running SRES for enough generations, it should have found parameter sets that pass all posterior conditions. We have found that SRES usually evolves perfect sets after about 100 generations but there is no guarantee it will find any on a particular run. If none were found, consider rerunning with more generations, a larger population, and/or different seeds (SRES and the simulation use different ones, as shown above).

**********************************
**7.1.1: Narrow parameter ranges**

To narrow your current ranges using the newly found parameter sets in good.params, run the following command in the scripts directory:

```
python refine-parameters.py -s ../sres/good.params -c ../sres/input.ranges -n ../sres/new.ranges
```

This script creates a new ranges filed in the sres directory named new.ranges that contains more precise ranges which you can use to help guide SRES in better directions. This script picks 2 standard deviations from the mean of each parameter and rounds each range to 5 digits by default. For more information, see Section 5.3.

*************************************
**7.1.2: Find robust parameter sets**

Once you have narrowed your paramater ranges sufficiently and are satisfied with the results that SRES has produced, you may want to ensure that your parameter sets are robust. We require robustness in our simulation because real biological systems demonstrate robustness to changes in the environment. Since seeds determine perturbations, a good parameter set should pass all conditions with slightly varying perturbation values. Due to the computationally intensive nature of our simulations, running multiple seeds is very time consuming, so this script is designed to run on a cluster. If you do not have access to a cluster you can simply run the simulation repeatedly while changing the -s or --seed command-line option.

Assuming you have access to a cluster, copy your local directory structure to it. If you have 5000 sets stored in good.params, to test for robustness run the following command, which spawns jobs of 500 parameter sets on your cluster and creates data necessary for checking robustness in the output directory:

```
python run-multiple-seeds.py --input-file ../sres/good.params --seeds 20 --num-params 5000 --pars-per-job 500 --directory ../simulation/output --simulation ../simulation/simulation --arguments -x 10 -w 10 -y 6 -m 600 -G 600 -s 2000 -u ../sres/input.perturb
```

The arguments specified to simulation are the same as the ones you used to create the 5000 sets you are testing for robustness. The seeds generated start at 1000 and increase in increments of 1000 until the script has run as many seeds as you have requested.

To analyze the data created by run-multiple-seeds.py, run the following command from the scripts directory:

python check-robustness.py --seeds 20 --num-params 5000 --output-file ../simulation/robust.params --num-files 10 --directory ../simulation/output --max-score 85 --input-file ../sres/good.params --threshold 18

The max-score is set to 85 based on the simulations that we ran but if you decide to change the scores for each condition you should adjust this number accordingly. 

This script is not a perfect robustness test; also consider testing the parameter sets with different tissue sizes, perturbation percentages (e.g. 10% or 15% instead of the 5% used), etc. It is up to you to define robustness.

*************************************
**7.2: Perform sensitivity analysis**

By this point you have run your simulations and found parameter sets that pass the desired conditions and are robust to small perturbations. As specified above, the parameter sets are stored in simulation/robust.params. Let's assume you have found 100 robust parameter sets.

Performing sensitivity analysis on these sets allows you to quantify how a simulation output feature changes when an input parameter changes, as well as rank which input parameters are the most important influences on each simulation output, as discussed in Section 4.0. In this case, we are using sensitivity analysis to determine which parameters should be chosen for gradient testing; parameter that have a large effect in the posterior should be tested with various gradients applied in the anterior to meet anterior conditions.

Sensitivity analysis requires running many simulations so we recommend parallelizing the task. If you do not have access to many processors, set --nodes to 1 and --ppn to 1, letting the program know that it should not divide the task among different threads. Assuming you have, say, 10 nodes with 6 processors per node, navigate to the scripts directory and run the following command:

```
python plot-sensitivities.py --nominal-file ../sensitivity-analysis/robust.params -d ../sensitivity-analysis/sensitivity-graphs --nodes 10 --ppn 6 -C biomath --percent 10 --points 4 -nominal-count 100 --exec ../sensitivity-analysis/sensitivity --sim ../simulation/simulation --args -s 1234 -a -u ../simulation/input.perturb
```

The above command distributes the work of generating sensitivity analysis data accross your 10 nodes. The sensitivity data is generated by finding the derivative around each nominal parameter set using 4 points within +/- 10 percent of the nominal value. These values are then normalized and written to sensitivity-analysis/sensitivity-graphs and sensitivity_X.output, where X is each submitted job.

After all requested sensitivity data has been gathered, the python script loads the data from each sensitivity output file (Section 4.1.3). This data is then averaged across each nominal parameter set and used to create bar graphs displaying the sensitivity of each simulation output feature to each input parameter. Graphs are created for both the normalized and absolute sensitivity data (Section 6.3.1) with error bars that represent the standard error.

********************************
**7.3: Find anterior gradients**

Now that we have precise ranges, robust sets, and know the sensitivities of each parameter, we can find gradients that allow the parameter sets satisfy both posterior and anterior conditions. Because we know that a gradient for the synthesis of _hes6_ mRNA is necessary for biological accuracy, create a gradients file in the sres-gradients directory named input.gradients with the following content:

```
# Posterior to anterior gradient for the hes6 mRNA synthesis rate
2 (9 100) (29 0)
```

Now let's create a ranges file in the sres-gradients directory named input.ranges with the following content:

```
gradient start [11,11]
gradient end [59,59]
gradient amount [0,1000]
```

Let's assume that the most important rates are the _her1_, _her7_, and _delta_ transcription delays, with indices 34, 35, and 37, respectively. Run the following command to search for the best gradients for these rates:

```
mpirun -np 12 ./sres -f ../simulation/simulation -i 34 -i 35 -i 37 -S ../simulation/robust.params -n 1 -o passed.params -d 3 --ranges-file input.ranges --total-population 20 --parent-population 3 -g 2000 -s 1234 -a -x 10 -w 10 -y 6 -m 1200 -s 2000 -M 1 -u ../sres/input.perturb --quiet
```

Note that we are running simulations only for the wild type (-M 1) and for 1200 minutes (-m 1200). After running SRES-gradients for enough generations, it should find gradients that cause simulations to pass all conditions, stored in passed.params. Congratulations, you've found biologically realistic parameter sets!

*********************
**7.4: Create plots**

Now that you’ve found parameter sets and gradients which make the model run as is expected biologically, you can use the available plotting scripts to create various figures. 

In order to create the spatial-profiling of the oscillation features, you can use the plotting scripts described in Section 5.4. Assuming that you’ve run the simulation using the -A or --anterior-feats and the -D or --directory-path option, you will have all the necessary data for running these scripts in the output directory you’ve created. The plots rely on the fact that you have run the simulation and stored data for all the mutants. Run the following command from the scripts folder, assuming that your output directory was called output and you have data for 10 parameter sets and the width of your posterior PSM was 10:

```
python plot-feats.py ../simulation/output 10 period-amplitude both ../simulation/plots 10 period-amplitude-data
```

To create a similar plot for synchronization, run:

```
python plot-sync ../simulation/output 10 ../simulation/plots sync sync-data
```

The plots directory will now contain all the figures you’ve created, as well as .csv files summarizing the data behind them.

**************************************
**7.5: Create snapshots and movies**

To create snapshots of the PSM as it grows through time, as well as a movie of the entire process, you can use tissue-snapshots.py as described in Section 5.9. and then use ffmpeg to create a movie.

To create a movie for the first set, go to the scripts folder and run the following command:

```
python tissue-snapshots.py ../simulation/output/set_0.cons ../simulation/images
```

After the script has finished creating the snapshots, which can take a while if the simulation was run for 1200 minutes, go to simulation/images and run the following command, assuming you’ve installed ffmpeg:

```
ffmpeg -y -qscale 5 -r 38 -b 9600 -i ../simulation/images/%04d.png ../simulation/movie.mp4
```

This will create movie.mp4 in the simulation directory, which you can view to see the movement in the PSM.

8: Authorship and licensing
---------------------------

Copyright (C) 2013 Ahmet Ay (aay@colgate.edu), Jack Holland (jholland@colgate.edu), Adriana Sperlea (asperlea@colgate.edu), and Sebastian Sangervasi (ssangervasi@colgate.edu).

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.


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
tests.hpp contains function declarations for tests.cpp.
*/

#ifndef TESTS_HPP
#define TESTS_HPP

#include "structs.hpp"

double test_wildtype_post(mutant_data&, features&);
double test_her7_mutant_post(mutant_data&, features&);
double test_her13_mutant_post(mutant_data&, features&);
double test_delta_mutant_post(mutant_data&, features&);
double test_her7her13_mutant_post(mutant_data&, features&);
double test_her1_mutant_post(mutant_data&, features&);
double test_her7delta_mutant_post(mutant_data&, features&);
double test_her1delta_mutant_post(mutant_data&, features&);
double test_her7over_mutant_post(mutant_data&, features&);
double test_her1over_mutant_post(mutant_data&, features&);
double test_deltaover_mutant_post(mutant_data&, features&);
double test_her1her7_mutant_post(mutant_data&, features&);
double test_DAPT_mutant_post(mutant_data&, features&);
double test_MESPAOVER_mutant_post (mutant_data& md, features& wtfeat);
double test_MESPBOVER_mutant_post (mutant_data& md, features& wtfeat);
double test_wildtype_ant(mutant_data&, features&);
double test_her7_mutant_ant(mutant_data&, features&);
double test_her13_mutant_ant(mutant_data&, features&);
double test_delta_mutant_ant(mutant_data&, features&);
double test_her7her13_mutant_ant(mutant_data&, features&);
double test_her1_mutant_ant(mutant_data&, features&);
double test_her7delta_mutant_ant(mutant_data&, features&);
double test_her1delta_mutant_ant(mutant_data&, features&);
double test_her7over_mutant_ant(mutant_data&, features&);
double test_her1over_mutant_ant(mutant_data&, features&);
double test_deltaover_mutant_ant(mutant_data&, features&);
double test_her1her7_mutant_ant(mutant_data&, features&);
double test_DAPT_mutant_ant(mutant_data&, features&);
double test_MESPAOVER_mutant_ant (mutant_data& md, features& wtfeat);
double test_MESPBOVER_mutant_ant (mutant_data& md, features& wtfeat);
int test_wildtype_wave (pair<int, int>[], int, mutant_data&, int, int);
int test_her1_wave (pair<int, int>[], int, mutant_data&, int, int);

#endif


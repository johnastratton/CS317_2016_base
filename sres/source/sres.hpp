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
sres.hpp contains function declarations for sres.cpp.
*/

#ifndef SRES_HPP
#define SRES_HPP

#include "structs.hpp"

int get_rank();
void init_sres(input_params&, sres_params&);
void run_sres(sres_params&);
void free_sres(sres_params&);
void fitness(double*, double*, double*);
double transform(double);

#endif


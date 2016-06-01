/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
//   <license>
//     This file was originally written by Bradley S. Meyer.
//
//     This is free software; you can redistribute it and/or modify it
//     under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     Please see the src/README.txt file in this distribution for more
//     information.
//   </license>
//   <description>
//     <abstract>
//       Header file for use with my_infall_functions.c.
//     </abstract>
//   </description>
// </file>
//
//////////////////////////////////////////////////////////////////////////////*/

#include <WnSimpleGce.h>

/*##############################################################################
// User data structure.
//############################################################################*/

typedef struct {
  WnSimpleGce *pGce;
  double dOmega1;
} my_infall_data;

typedef struct {
  WnSimpleGce *pGce;
  double dGamma;
} my_infall_primary_metallicity_data;

typedef struct {
  WnSimpleGce__Species *pSpecies;
  double dMassFraction;
} my_infall_mass_fraction_data;


/*##############################################################################
// prototypes.
//############################################################################*/

double
my_infall_cycle_function(
  double, void *
);

double
my_infall_cycle_integrand(
  double, void *
);

double
my_infall_primary_metallicity(
  double, void *
);

double
my_infall_mass_fraction(
  double, void *
);

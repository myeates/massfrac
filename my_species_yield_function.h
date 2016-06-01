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
//       Header file for use with my_species_yield_function.c.
//     </abstract>
//   </description>
// </file>
//
//////////////////////////////////////////////////////////////////////////////*/

#include <gsl/gsl_sf_erf.h>

/*##############################################################################
// User data structure.
//############################################################################*/

typedef struct {
  double dAlpha1;
  double dAlpha2;
  double dZBreak;
  double dWidth;
} user_yield_data;

/*##############################################################################
// prototype.
//############################################################################*/

double
my_yield_function(
  double,
  user_yield_data *
);


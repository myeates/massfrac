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
//       Example to demonstrate user-defined species yield functions.
//     </abstract>
//   </description>
// </file>
//
//////////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>
#include "my_species_yield_function.h"

double
my_yield_function(
  double d_primary_metallicity,
  user_yield_data * p_my_data
)
{

  return
    p_my_data->dAlpha1 +
    p_my_data->dAlpha2 *
    (
      1.
      +
      gsl_sf_erf(
        ( d_primary_metallicity - p_my_data->dZBreak )
        /
        p_my_data->dWidth
      )
    ) / 2.;

}


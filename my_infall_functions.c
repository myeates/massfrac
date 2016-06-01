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
//       Example to demonstrate user-defined infall functions.
//     </abstract>
//   </description>
// </file>
//
//////////////////////////////////////////////////////////////////////////////*/

#include <stdlib.h>
#include <math.h>
#include "my_infall_functions.h"

/*##############################################################################
// my_infall_cycle_function().  Appropriate for Clayton (1984) standard model 2.
//############################################################################*/

double
my_infall_cycle_function(
  double d_time,
  void *p_user_data
)
{

  double d_x;

  my_infall_data * p_data =
    ( my_infall_data * ) p_user_data;

  if( WnSimpleGce__getInfallKValue( p_data->pGce ) == 0 ) return 0.;

  d_x = p_data->dOmega1 * d_time + WnSimpleGce__getInfallDelta( p_data->pGce );

  if( d_x > M_PI_2 )
    return
      log(
        pow(
          sin( WnSimpleGce__getInfallDelta( p_data->pGce ) ),
          -(double) WnSimpleGce__getInfallKValue( p_data->pGce )
        )
      );
  else
    return
      log(
        pow(
          sin( d_x ) / sin( WnSimpleGce__getInfallDelta( p_data->pGce ) ),
          (double) WnSimpleGce__getInfallKValue( p_data->pGce )
        )
      );

} 

/*##############################################################################
// my_infall_cycle_integrand(). Appropriate for Clayton (1984) standard model 2.
//############################################################################*/

double
my_infall_cycle_integrand(
  double d_time,
  void *p_user_data
)
{

  double d_x;
    double d_result;

  my_infall_data * p_data =
    ( my_infall_data * ) p_user_data;

  if( WnSimpleGce__getInfallKValue( p_data->pGce ) == 0 ) return 0.;

  d_x = p_data->dOmega1 * d_time + WnSimpleGce__getInfallDelta( p_data->pGce );

  if( d_x > M_PI_2 )
    return 0.;
  else
    d_result =
      (double) WnSimpleGce__getInfallKValue( p_data->pGce ) *
      p_data->dOmega1 *
      cos( d_x ) /
      sin( d_x );

    return d_result;

} 

/*##############################################################################
// my_infall_primary_metallicity().
//############################################################################*/

double
my_infall_primary_metallicity(
  double d_time,
  void *p_user_data
)
{

  my_infall_primary_metallicity_data *p_data =
    (my_infall_primary_metallicity_data *) p_user_data;

  return
    p_data->pGce->dInitialPrimaryMetallicity +
      p_data->dGamma * 
      p_data->pGce->dAlpha *
      p_data->pGce->dOmega *
      d_time;

}

/*##############################################################################
// my_infall_mass_fraction().
//############################################################################*/

double
my_infall_mass_fraction(
  double d_time,
  void *p_user_data
)
{

  my_infall_mass_fraction_data *p_data =
    (my_infall_mass_fraction_data *) p_user_data;

  return
    p_data->dMassFraction * exp( -p_data->pSpecies->dDecayRate * d_time );

}


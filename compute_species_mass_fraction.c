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
//       Example to compute species mass fraction as a function of time
//       in a simple GCE model from input yield data.
//     </abstract>
//   </description>
// </file>
//////////////////////////////////////////////////////////////////////////////*/


#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h> 

#include "rappture.h"
#include "WnSimpleGce.h"

#include <boost/lexical_cast.hpp>

#define D_INTERVAL 0.1

int main( int argc, char **argv ) {

  RpLibrary* lib = NULL;
  char line [100];
  WnSimpleGce *p_model;
  WnSimpleGce__Species *p_species;
  double d_t;
  double k = 0;
  double delta = 0; 
  double omega = 0;
  double alpha = 0;
  double alpha_i = 0;
  const char* species = NULL;
  double decay_rate = 0;

  lib = rpLibrary(argv[1]);

  if( lib == NULL )
  {
    /* cannot open file or out of memory */
    printf("FAILED creating Rappture Library\n");
    return EXIT_FAILURE;
  }

  if( rpGetDouble(lib,"input.number(k).current",&k) )
  {
    printf ("Error while retrieving k.\n");
    return EXIT_FAILURE;
  }

  if( rpGetDouble(lib,"input.number(Delta).current",&delta) )
  {
    printf ("Error while retrieving Delta.\n");
    return EXIT_FAILURE;
  }

  if( rpGetDouble(lib,"input.number(omega).current",&omega) )
  {
    printf ("Error while retrieving omega.\n");
    return EXIT_FAILURE;
  }

  if( rpGetDouble(lib,"input.number(alpha).current",&alpha) )
  {
    printf ("Error while retrieving alpha.\n");
    return EXIT_FAILURE;
  }

  if( rpGetString(lib,"input.number(species).current", &species ) )
  {
    printf ("Error while retrieving species.\n");
    return EXIT_FAILURE;
  }

  if( rpGetDouble(lib,"input.number(decay_rate).current",&decay_rate) )
  {
    printf ("Error while retrieving decay rate.\n");
    return EXIT_FAILURE;
  }

  if( rpGetDouble(lib,"input.number(alpha_i).current",&alpha_i) )
  {
    printf ("Error while retrieving alpha_i.\n");
    return EXIT_FAILURE;
  }

  /*============================================================================
  // Create model.
  //==========================================================================*/

  p_model = WnSimpleGce__new( );

  /*============================================================================
  // Update model.
  //==========================================================================*/

  WnSimpleGce__updateInfallKValue( p_model, (unsigned int)  k);
  
  WnSimpleGce__updateInfallDelta( p_model, delta);
  
  WnSimpleGce__updateOmega( p_model, omega);
  
  WnSimpleGce__updatePrimaryMetallicityYield( p_model, alpha);

  /*============================================================================
  // Create species.
  //==========================================================================*/

  p_species = WnSimpleGce__Species__new(species);

  /*============================================================================
  // Assign the decay rate.
  //==========================================================================*/

  WnSimpleGce__Species__updateDecayRate( p_species, decay_rate);

  /*============================================================================
  // Get species yield data.
  //==========================================================================*/
  
  WnSimpleGce__Species__updateYieldCoefficient( p_species, 0, alpha_i );

  /*============================================================================
  // Compute species mass fraction.
  //==========================================================================*/

  d_t = 0.;

  while( d_t < 20.0 + D_INTERVAL ) {
  
    sprintf(
      line,
      "%f %.15f\n",
      d_t,
      WnSimpleGce__computeSpeciesMassFraction( p_model, p_species, d_t)
    );

    rpPutString( lib,"output.curve(tx).component.xy",line,RPLIB_APPEND );
    d_t += D_INTERVAL;

  }

  /*============================================================================
  // Do cleanup.
  //==========================================================================*/

  WnSimpleGce__Species__free( p_species );
  WnSimpleGce__free( p_model );

  /*============================================================================
  // Done.
  //==========================================================================*/

  rpResult(lib);
  return EXIT_SUCCESS;

}


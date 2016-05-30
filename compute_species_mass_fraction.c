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
 
#include <WnSimpleGce.h>

#define D_INTERVAL 0.1

int
main( int argc, char **argv ) {

  WnSimpleGce *p_model;
  WnSimpleGce__Species *p_species;
  double d_t;
  int i;

  /*============================================================================
  // Check input.
  //==========================================================================*/

   if ( argc < 8 ) {
      fprintf(
        stderr,
        "\nUsage: %s k Delta omega alpha species decay_rate alpha_i ... \n\n",
        argv[0]
      );
      fprintf(
        stderr,
        "k = Clayton infall parameter k\n\n"
      );
      fprintf(
        stderr,
        "Delta = Clayton infall parameter Delta\n\n"
      );
      fprintf(
        stderr,
        "omega = gas consumption rate (per Gyr)\n\n"
      );
      fprintf(
        stderr,
        "alpha = primary metallicity yield\n\n"
      );
      fprintf(
        stderr,
        "species = name of species\n\n"
      );
      fprintf(
        stderr,
        "decay_rate = decay rate (per Gyr)\n\n"
      );
      fprintf(
        stderr,
        "alpha_i = alpha parameter for species\n\n"
      );
      return EXIT_FAILURE;
   }

  /*============================================================================
  // Create model.
  //==========================================================================*/

  p_model = WnSimpleGce__new( );

  /*============================================================================
  // Update model.
  //==========================================================================*/

  WnSimpleGce__updateInfallKValue( p_model, (unsigned int) atoi( argv[1] ) );
  
  WnSimpleGce__updateInfallDelta( p_model, atof( argv[2] ) );
  
  WnSimpleGce__updateOmega( p_model, atof( argv[3] ) );
  
  WnSimpleGce__updatePrimaryMetallicityYield( p_model, atof( argv[4] ) );

  /*============================================================================
  // Create species.
  //==========================================================================*/

  p_species = WnSimpleGce__Species__new( argv[5] );

  /*============================================================================
  // Assign the decay rate.
  //==========================================================================*/

  WnSimpleGce__Species__updateDecayRate( p_species, atof( argv[6] ) );

  /*============================================================================
  // Get species yield data.
  //==========================================================================*/

  for( i = 0; i < argc - 7; i++ )
  {
    if( 
       WnSimpleGce__Species__updateYieldCoefficient(
         p_species,
         (unsigned int) i,
         atof( argv[i+7] )
       ) != 0
    ) {
      fprintf( stderr, "Couldn't add yield data!\n" );
      return EXIT_FAILURE;
    }
  }

  /*============================================================================
  // Compute species mass fraction.
  //==========================================================================*/

  fprintf(
    stdout,
    " t (Gyr)\t  X(%s)\n\n",
    WnSimpleGce__Species__getName( p_species )
  );

  d_t = 0.;

  while( d_t < 20.0 + D_INTERVAL ) {

    fprintf(
      stdout,
      "%12.6e  %12.6e\n",
      d_t,
      WnSimpleGce__computeSpeciesMassFraction( p_model, p_species, d_t )
    );

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

  return EXIT_SUCCESS;

}


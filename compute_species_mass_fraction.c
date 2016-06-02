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
#include "rappture.h"
#include <boost/lexical_cast.hpp>
#define D_INTERVAL 0.1

int main( int argc, char **argv ) {

  RpLibrary* lib = NULL;
  const char* data = NULL;
//char line [100];
  WnSimpleGce *p_model;
  WnSimpleGce__Species *p_species;
  double d_t;
  int i;
  int err = 0;
  double k = 0;
  double delta = 0; 
  double omega = 0;
  double alpha = 0;
  const char* species = NULL;
  double decay_rate = 0;
  double alpha_i = 0;  

  /*============================================================================
  // Check input.
  //==========================================================================*/

//   if ( argc < 8 ) {
//      fprintf(
//        stderr,
//        "\nUsage: %s k Delta omega alpha species decay_rate alpha_i ... \n\n",
//        argv[0]
//      );
//     fprintf(
//       stderr,
//        "k = Clayton infall parameter k\n\n"
//      );
//      fprintf(
//        stderr,
//        "Delta = Clayton infall parameter Delta\n\n"
//      );
//      fprintf(
//        stderr,
//        "omega = gas consumption rate (per Gyr)\n\n"
//      );
//      fprintf(
//        stderr,
//       "alpha = primary metallicity yield\n\n"
//      );
//      fprintf(
//        stderr,
//        "species = name of species\n\n"
//      );
//      fprintf(
//        stderr,
//        "decay_rate = decay rate (per Gyr)\n\n"
//      );
//      fprintf(
//        stderr,
//        "alpha_i = alpha parameter for species\n\n"
//      );
//      return EXIT_FAILURE;
//   }


 rpGetString(lib,"input.number(k).current",&data);
  k = rpConvertDbl(data, "points", &err);
  if (err) {
        printf ("Error while retrieving input.number(points).current\n");
        return(2);
  }


  rpGetString(lib,"input.number(delta).current",&data);
  delta = rpConvertDbl(data, "coefficient", &err);
  if (err) {
        printf ("Error while retrieving input.number(a).current\n");
        return(3);
  }
  rpGetString(lib,"input.number(omega).current",&data);
  omega = rpConvertDbl(data, "points", &err);
  if (err) {
        printf ("Error while retrieving input.number(points).current\n");
        return(4);
  }


  rpGetString(lib,"input.number(alpha).current",&data);
  alpha = rpConvertDbl(data, "coefficient", &err);
  if (err) {
        printf ("Error while retrieving input.number(a).current\n");
        return(5);
  }
 rpGetString(lib,"input.number(species).current",&data);
  species = data;
  if (species==(NULL)) {
        printf ("Error while retrieving input.number(points).current\n");
        return(6);
  }


  rpGetString(lib,"input.number(decay_rate).current",&data);
  decay_rate = rpConvertDbl(data, "coefficient", &err);
  if (err) {
        printf ("Error while retrieving input.number(a).current\n");
        return(7);
  }
// rpGetString(lib,"input.integer(points).current",&data);
//  pts = rpConvertDbl(data, "points", &err);
//  if (err) {
//        printf ("Error while retrieving input.number(points).current\n");
//        return(2);
//  }


  rpGetString(lib,"input.number(alpha_i).current",&data);
  alpha_i = rpConvertDbl(data, "coefficient", &err);
  if (err) {
        printf ("Error while retrieving input.number(a).current\n");
        return(8);
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
  
  char* var[7] = {boost::lexical_cast<std::string>(k),boost::lexical_cast<std::string>(delta),boost::lexical_cast<std::string>(omega),boost::lexical_cast<std::string>(alpha),species,boost::lexical_cast<std::string>(decay_rate),boost::lexical_cast<std::string>(alpha_i)};
  for( i = 0; i <  7; i++ )
  {
    if( 
       WnSimpleGce__Species__updateYieldCoefficient(
         p_species,
         (unsigned int) i,
        atof( var[i])
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


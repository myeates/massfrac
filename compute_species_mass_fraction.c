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
  char tx [100];
  char infall [100];
  char total [100];
  char star [100];
  char gas [100];
  char output [100];
  WnSimpleGce* p_model;
  WnSimpleGce__Species* p_species;
  double d_t;
  double k = 0;
  double delta = 0; 
  double omega = 0;
  double alpha = 0;
  double alpha_i = 0;
  const char* species = NULL;
  double decay_rate = 0;
  double beta_i = 0;
  const char* choice;
  double fraction = 0;
  double time = 0;
  double mass = 0;

  lib = rpLibrary(argv[1]);
  
  if( lib == NULL )
  {
    /* cannot open file or out of memory */
    printf("FAILED creating Rappture Library\n");
    return(1);
  }


  /*============================================================================
  // Check and Retrieve inputs
  //==========================================================================*/

  if( rpGetDouble(lib,"input.group(model).number(k).current",&k) )
  {
    printf ("Error while retrieving k.\n");
    return(2);
  }


  if( rpGetDouble(lib,"input.group(model).number(delta).current",&delta) )
  {
    printf ("Error while retrieving Delta.\n");
    return(3);
  }

  if( rpGetDouble(lib,"input.group(model).number(mass).current",&mass) )
  {
    printf ("Error while retrieving Initial Mass.\n");
    return(4);
  }

  if( rpGetDouble(lib,"input.group(model).number(fraction).current",&fraction) )
  {
    printf ("Error while retrieving mass fraction.\n");
    return(5);
  }

  if( rpGetDouble(lib,"input.group(model).number(time).current",&time) )
  {
    printf ("Error while retrieving time.\n");
    return(6);
  }

  if( rpGetDouble(lib,"input.group(model).number(alpha).current",&alpha) )
  {
    printf ("Error while retrieving alpha.\n");
    return(7);
  }
  
  if( rpGetString(lib,"input.group(species).number(species).current",&species))
  {
    printf ("Error while retrieving species.\n");
    return(8);
  }

  if( rpGetDouble(lib,"input.group(species).number(decay_rate).current",&decay_rate) )
  {
    printf ("Error while retrieving decay rate.\n");
    return(9);
  }

  if (rpGetString(lib,"input.group(species).choice(coeff).current",&choice))
  {
    printf ("Error while retrieving choice.\n");
    return(10);
  }
  if( rpGetDouble(lib,"input.group(species).number(alpha_i).current",&alpha_i) )
  {
    printf ("Error while retrieving alpha_i.\n");
    return(11);
  }

  if( rpGetDouble(lib,"input.group(species).number(beta_i).current",&beta_i))
  {
    printf ("Error while retrieving beta_i.\n");
    return(12);
  }

  
  /*============================================================================
  // Create model.
  //==========================================================================*/

  p_model = WnSimpleGce__new( );

  /*============================================================================
  // Update model.
  //==========================================================================*/

  WnSimpleGce__updateInitialGasMass( p_model, mass);

  WnSimpleGce__updateInfallKValue( p_model, (unsigned int)  k);
  
  WnSimpleGce__updateInfallDelta( p_model, delta);
  
  WnSimpleGce__updatePrimaryMetallicityYield( p_model, alpha);

  /*============================================================================
  // Calculate Omega.
  //==========================================================================*/

  if(!WnSimpleGce__computeOmegaFromGasFraction(p_model,time,fraction))
  {
    printf("Error could not calculate Omega\n");
    return(13);
  }
  omega=WnSimpleGce__getOmega(p_model);
  sprintf(output,"For a k = %f and a Delta = %f \n",k,delta); 
  rpPutString( lib,"output.string(omega).current",output,RPLIB_APPEND );
  sprintf(output,"To get a gas fraction of %f at t = %f Gyr, \nYou need an omega = %6.15f",fraction,time,omega);
  rpPutString( lib,"output.string(omega).current",output,RPLIB_APPEND );

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
    
  if( strcmp(choice,"alpha_i") == 0)
  {
  WnSimpleGce__Species__updateYieldCoefficient( p_species, 0, alpha_i );
  }
  else if( strcmp(choice,"beta_i") == 0)
  {
  WnSimpleGce__Species__updateYieldCoefficient( p_species, 1, beta_i);
  }
  else if( strcmp(choice,"both") == 0)
  {
  WnSimpleGce__Species__updateYieldCoefficient( p_species, 0, alpha_i );
  WnSimpleGce__Species__updateYieldCoefficient( p_species, 1, beta_i);
  }
  else
  {
    printf ("Error");
    return(14);
  }
   
  /*============================================================================
// Compute species mass fraction.
  //==========================================================================*/

  d_t = 0.;

  while( d_t < 20.0 + D_INTERVAL ) {
  
   sprintf(tx,"%6.15f %6.15f\n",d_t,WnSimpleGce__computeSpeciesMassFraction( p_model, p_species, d_t));
   sprintf(infall,"%6.15f %6.15f\n",d_t,WnSimpleGce__computeInfallRate( p_model, d_t));
   sprintf(total,"%6.15f %6.15f\n",d_t,WnSimpleGce__computeTotalMass( p_model, d_t));
   sprintf(star,"%6.15f %6.15f\n",d_t,WnSimpleGce__computeStarMass( p_model, d_t));
   sprintf(gas,"%6.15f %6.15f\n",d_t,WnSimpleGce__computeGasMass( p_model, d_t));


    rpPutString( lib,"output.curve(tx).component.xy",tx,RPLIB_APPEND );
    rpPutString( lib,"output.curve(infall).component.xy",infall,RPLIB_APPEND );
    rpPutString( lib,"output.curve(total).component.xy",total,RPLIB_APPEND );
    rpPutString( lib,"output.curve(star).component.xy",star,RPLIB_APPEND );
    rpPutString( lib,"output.curve(gas).component.xy",gas,RPLIB_APPEND );

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


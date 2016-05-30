/*//////////////////////////////////////////////////////////////////////////////
// <file type="public">
//   <license>
//      Copyright (c) 2006-2013 Clemson University.
//
//      This distribution directory contains the source code
//      for the Clemson Webnucleo group's
//      wn_simple_gce module, originally developed by Bradley S. Meyer.
//
//      This is free software; you can redistribute it and/or modify it
//      under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This software is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this software (please see the "gnu_gpl.txt" file in the doc/
//      directory of this distribution); if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
//      USA
//
//   </license>
// </file>
//////////////////////////////////////////////////////////////////////////////*/

#ifndef WN_SIMPLE_GCE__H
#define WN_SIMPLE_GCE__H

/*##############################################################################
// Use extern "C" for C++ compilers.
//############################################################################*/

#ifdef __cplusplus
extern "C"
{
#endif

/*##############################################################################
// Standard library #include's.
//############################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*##############################################################################
// Gsl includes.
//############################################################################*/

#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_math.h>

/*##############################################################################
// Libxml includes.
//############################################################################*/

#include <libxml/hash.h>

/*##############################################################################
// Macro definitions.  To debug, compile with the environment variable WN_DEBUG
// set.
//############################################################################*/

#ifndef WN_DEBUG

#define WN_SIMPLE_GCE__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         exit( EXIT_FAILURE ); \
       } while (0)

#else

#define WN_SIMPLE_GCE__ERROR( s_reason ) \
       do { \
         fprintf( stderr, "ERROR: %s in %s on line %d\n", \
           s_reason, __FILE__, __LINE__ \
         ); \
         abort(); \
       } while (0)

#endif

/*##############################################################################
// Default model values and integration data.
//############################################################################*/

#define D_INITIAL_GAS_MASS     1.e8
#define I_K                    1
#define D_DELTA                0.1
#define D_OMEGA                0.3
#define D_ALPHA                0.012

#define D_TOLERANCE            1.e-06
#define I_MAX_STEPS            1000
#define I_WORKSPACE            1000
#define D_FACTOR               10.
#define D_CONVERGE             1.e-08
#define D_EPS_COMP             1.e-15

typedef double ( *WnSimpleGce__integrand ) (
  double, void *
);

/*##############################################################################
// Forward declarations.
//############################################################################*/

typedef struct _WnSimpleGce WnSimpleGce;

typedef struct _WnSimpleGce__Species WnSimpleGce__Species;

/*##############################################################################
// <user_routine name="WnSimpleGce__infall_cycle_integrand()">
//
//   <description>
//     <abstract>
//       User-supplied routine to calculate the infall cycle number integrand.
//       This quantity is the current infall rate divided by the current
//       gas mass.
//     </abstract>
//     <keywords>
//       WnSimpleGce, user, supplied, function, Galactic, chemical, evolution,
//       infall, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__infall_cycle_integrand(
//   double d_time,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       The time (in Gyr) at which to compute the infall cycle number integand.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the infall integrand.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the infall cycle
//       number integrand at the
//       input time.  The user sets the infall rate function for a model with
//       WnSimpleGce__updateInfallCycleFunctionsAndData().
//       The default integrand is that for standard model 1 by Clayton (1985).
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *WnSimpleGce__infall_cycle_integrand ) (
  double, void *
);

/*##############################################################################
// <user_routine name="WnSimpleGce__infall_cycle_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to calculate the infall cycle number.
//     </abstract>
//     <keywords>
//       WnSimpleGce, user, supplied, function, Galactic, chemical, evolution,
//       infall, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__infall_cycle_function(
//   double d_time,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       The time (in Gyr) at which to compute the infall cycle number.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the infall function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the infall cycle
//       number at the
//       input time.  The user sets the infall rate function for a model with
//       WnSimpleGce__updateInfallCycleFunctionsAndData().
//       The default function is that for standard model 1 by Clayton (1985).
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *WnSimpleGce__infall_cycle_function ) (
  double, void *
);

/*##############################################################################
// <user_routine name="WnSimpleGce__infallPrimaryMetallicityFunction()">
//
//   <description>
//     <abstract>
//       User-supplied routine to calculate the primary metallicity in the
//       infalling material.
//     </abstract>
//     <keywords>
//       WnSimpleGce, user, supplied, function, Galactic, chemical, evolution,
//       infall, primary, metallicity
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__infall_primary_metallicity_function(
//   double d_time,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       The time (in Gyr) at which to compute the infall rate. 
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the infall primary metallicity function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the primary metallicity
//       in the infalling material at the input time.  The user sets the
//       infall primary metallicity function for a model with
//       WnSimpleGce__setInfallPrimaryMetallicityFunction().
//       The default is that the infalling material has zero metallicity.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *WnSimpleGce__infall_primary_metallicity_function ) (
  double, void *
);

/*##############################################################################
// <user_routine name="WnSimpleGce__Species__infall_mass_fraction_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to calculate the mass fraction of a species in
//       the infalling material.
//     </abstract>
//     <keywords>
//       WnSimpleGce, user, supplied, function, Galactic, chemical, evolution,
//       infall, species, mass, fraction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__Species__infall_mass_fraction_function(
//   double d_time,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       The time (in Gyr) at which to compute the infall rate. 
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the infall mass fraction function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the mass fraction of the
//       species in the infalling material at the input time.  The user sets
//       the infall mass fraction function for the species for a model with
//       WnSimpleGce__Species__setInfallMassFractionFunction().
//       The default is that the species has zero mass fraction in the
//       infalling matter.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *WnSimpleGce__Species__infall_mass_fraction_function ) (
  double,
  void *
);

/*##############################################################################
// <user_routine name="WnSimpleGce__Species__yield_function()">
//
//   <description>
//     <abstract>
//       User-supplied routine to calculate the yield of a species for
//       a given value of the primary metallicity.
//     </abstract>
//     <keywords>
//       WnSimpleGce, user, supplied, function, Galactic, chemical, evolution,
//       infall, species, yield
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__Species__yield_function(
//   double d_primary_metallicity,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="d_primary_metallicity"
//       kind="in,positional,required"
//     >
//       The primary metallicity at which to compute the yield.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined structure containing extra data for
//       the yield function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       User's routine must return a double giving the yield for the
//       species at the input primary metallicity.  The user sets
//       the yield function for the species
//       WnSimpleGce__Species__setYieldFunction().
//       The default is that the species yield is computed from the usual
//       primary, secondary, ... power series with coefficients alpha, beta,
//       ....
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//   </usage>
//
// </user_routine>
//############################################################################*/

typedef double ( *WnSimpleGce__Species__yield_function ) (
  double,
  void *
);

typedef struct {
  double dValue;
} WnSimpleGce__Species__YieldEntry;

/*##############################################################################
// <class name="WnSimpleGce">
//
//   <description>
//     <abstract>
//       A structure storing information for a GCE model.  The contents of the
//       structure are not made public by the API but rather are accessed
//       through API routines.
//     </abstract>
//     <keywords>
//       Simple, gce, galactic, chemical, evolution
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2008/08/30"/>
//     </current>
//     <previous>
//     </previous>
//   </authors>
//   
//   <compatibility>
//     Tested in gcc (GCC) 3.4.4.
//   </compatibility>
//   
// </class>
//############################################################################*/

struct _WnSimpleGce {
  xmlHashTablePtr pSpeciesHash;
  double dInitialGasMass;
  unsigned int iK;
  double dDelta;
  double dOmega;
  double dAlpha;
  double dInitialPrimaryMetallicity;
  WnSimpleGce__infall_cycle_function pfInfallCycleFunction;
  void *pInfallCycleFunctionData;
  WnSimpleGce__infall_cycle_integrand pfInfallCycleIntegrand;
  void *pInfallCycleIntegrandData;
  WnSimpleGce__infall_primary_metallicity_function
    pfInfallPrimaryMetallicityFunction;
  void *pInfallPrimaryMetallicityUserData;
};

/*##############################################################################
// <class name="WnSimpleGce__Species">
//
//   <description>
//     <abstract>
//       A structure storing information for a species in a GCE model.
//       The contents of the structure are not made public by the API but
//       rather are accessed through API routines.
//     </abstract>
//     <keywords>
//       Simple, gce, galactic, chemical, evolution, species
//     </keywords>
//   </description>
//
//   <authors>
//     <current>
//       <author userid="mbradle" start_date="2008/08/31"/>
//     </current>
//     <previous>
//     </previous>
//   </authors>
//   
//   <compatibility>
//     Tested in gcc (GCC) 3.4.4.
//   </compatibility>
//   
// </class>
//############################################################################*/

struct _WnSimpleGce__Species {
  xmlChar *sxName;
  xmlHashTablePtr pYieldHash;
  double dDecayRate;
  double dInitialMassFraction; 
  WnSimpleGce__Species__infall_mass_fraction_function
    pfInfallMassFractionFunction;
  void *pInfallMassFractionUserData;
  WnSimpleGce__Species__yield_function pfYieldFunction;
  void *pYieldFunctionUserData;
};

/*##############################################################################
// API routines.
//############################################################################*/

/*##############################################################################
// <routine name="WnSimpleGce__new()">
//
//   <description>
//     <abstract>
//       Create a new simple GCE model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// WnSimpleGce * WnSimpleGce__new( );
//     </calling_sequence>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new WnSimpleGce structure,
//       that is, a new simple GCE model.
//       If it is not possible to allocate memory for the new structure,
//       WnSimpleGce error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a new simple GCE model pointed to by p_my_model:
//       </synopsis>
//
//       <code>
// p_my_model = WnSimpleGce__new( );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

WnSimpleGce *
WnSimpleGce__new( void );

/*##############################################################################
// <routine name="WnSimpleGce__Species__new()">
//
//   <description>
//     <abstract>
//       Create a new species for a simple GCE model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// WnSimpleGce__Species *
// WnSimpleGce__Species__new( const char *s_name );
//     </calling_sequence>
//
//     <param
//       name="s_name"
//       kind="in,positional,required"
//     >
//       A string giving the name of the new species.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       The routine returns a pointer to a new WnSimpleGce__Species structure,
//       that is, a new species in a simple GCE model.  The newly created
//       species has a decay rate of zero.   The decay rate can be updated
//       with the routine WnSimpleGce__Species__updateDecayRate().  The
//       newly created species also has yield coefficients of zero.  These
//       can be updated with the routine
//       WnSimpleGce__Species__updateYieldCoefficient().
//       If it is not possible to allocate memory for the new structure,
//       WnSimpleGce error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Create a new species, oxygen-16, for a simple GCE model:
//       </synopsis>
//
//       <code>
// p_o16 = WnSimpleGce__Species__new( "o16" );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

WnSimpleGce__Species *
WnSimpleGce__Species__new( const char * );

/*##############################################################################
// <routine name="WnSimpleGce__Species__updateDecayRate()">
//
//   <description>
//     <abstract>
//       Update the decay rate (per Gyr) for a species in a simple GCE model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, species, decay, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__Species__updateDecayRate(
//   WnSimpleGce__Species *self,
//   double d_decay_rate
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to the species whose decay rate is to be updated.
//     </param>
//
//     <param
//       name="d_decay_rate"
//       kind="in,positional,required"
//     >
//       A double giving the decay rate (per Gyr) of the species.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the decay rate of the species has been
//       updated to the input value.  If the species or the decay rate
//       is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the decay rate of al26 (pointed to by p_al26) to 1000. per
//         Gyr:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__updateDecayRate( p_al26, 1000. );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__Species__updateDecayRate(
  WnSimpleGce__Species *, double
);

/*##############################################################################
// <routine name="WnSimpleGce__Species__updateYieldCoefficient()">
//
//   <description>
//     <abstract>
//       Update the yield coefficient for a species.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, species, yield
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int
// WnSimpleGce__Species__updateYieldCoefficient(
//   WnSimpleGce__Species *self,
//   unsigned int i_n,
//   double d_coefficient
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to the species whose yield coefficient is to be updated.
//     </param>
//
//     <param
//       name="i_n"
//       kind="in,positional,required"
//     >
//       An unsigned int giving the power of the primary metallicity for
//       which the coefficient is being updated.
//     </param>
//
//     <param
//       name="d_coefficient"
//       kind="in,positional,required"
//     >
//       A double giving the value to which to update the coefficient.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns 0 if successful, -1 if not.
//       Upon successful return, the yield coefficient for the term in the
//       yield that is the primary metallicity raised to the power i_n has
//       been updated to the input value.  If the species is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the primary metallicity part (n=0) of the yield of al27
//         (pointed to by p_al27) to 0.0034:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__updateYieldCoefficient( p_al27, 0, 0.0034 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
WnSimpleGce__Species__updateYieldCoefficient(
  WnSimpleGce__Species *, 
  unsigned int,
  double
);

/*##############################################################################
// <routine name="WnSimpleGce__Species__getName()">
//
//   <description>
//     <abstract>
//       Retrieve the name of a species.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, species, name
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// const char *
// WnSimpleGce__Species__getName(
//   WnSimpleGce__Species *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to the species whose name is desired.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the name of the species.  If the input species is
//       invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//          Print the name of the species pointed to by p_species:
//       </synopsis>
//
//       <code>
// printf(
//   "The name of the species is %s\n",
//   WnSimpleGce__Species__getName( p_species )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

const char *
WnSimpleGce__Species__getName( WnSimpleGce__Species * );

/*##############################################################################
// <routine name="WnSimpleGce__computeSpeciesYield()">
//
//   <description>
//     <abstract>
//       Compute the yield for the input species for the input model at the
//       given time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, species, yield
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeSpeciesYield(
//   WnSimpleGce *self,
//   WnSimpleGce__Species *p_species,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce__Species.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the yield of
//       the species.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the yield of the species at the input time for the
//       input model.  If the model, species, or time is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model and the
//         WnSimpleGce__Species p_species, print the yield at 11 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 11.;
// printf(
//   "At %g Gyr, the yield of %s is %g.\n",
//   d_time,
//   WnSimpleGce__Species__getName( p_species ),
//   WnSimpleGce__computeSpeciesYield( p_model, p_species, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeSpeciesYield(
  WnSimpleGce *, WnSimpleGce__Species *, double
);

/*##############################################################################
// <routine name="WnSimpleGce__computeSpeciesMassFraction()">
//
//   <description>
//     <abstract>
//       Compute the mass fraction for the input species in the gas for the
//       input model at the given time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, species, mass, fraction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeSpeciesMassFraction(
//   WnSimpleGce *self,
//   WnSimpleGce__Species *p_species,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="p_species"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce__Species.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the mass
//       fraction in the gas of the species.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the mass fraction of the species at the input time
//       for the input model.  If the model, species, or time is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model and the
//         WnSimpleGce__Species p_species, print the mass fraction at 11 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 11.;
// printf(
//   "At %g Gyr, the mass fraction of %s is %g.\n",
//   d_time,
//   WnSimpleGce__Species__getName( p_species ),
//   WnSimpleGce__computeSpeciesMassFraction( p_model, p_species, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeSpeciesMassFraction(
  WnSimpleGce *, WnSimpleGce__Species *, double
);

/*##############################################################################
// <routine name="WnSimpleGce__free()">
//
//   <description>
//     <abstract>
//       Free the memory for a simple GCE model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, free
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__free( WnSimpleGce *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the model has been freed.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free the memory for the WnSimpleGce model pointed to by p_my_model:
//       </synopsis>
//
//       <code>
// WnSimpleGce__free( p_my_model );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__free( WnSimpleGce * );

/*##############################################################################
// <routine name="WnSimpleGce__Species__free()">
//
//   <description>
//     <abstract>
//       Free the memory for a species in a simple GCE model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, free, species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__Species__free( WnSimpleGce__Species *self );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a species in a simple GCE model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the memory for the species has been freed.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Free the memory for the species pointed to by p_species:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__free( p_species );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__Species__free( WnSimpleGce__Species * );

/*##############################################################################
// <routine name="WnSimpleGce__computeInfallRate()">
//
//   <description>
//     <abstract>
//       Compute the infall rate for the given model at the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeInfallRate(
//   WnSimpleGce *self,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       infall rate.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the infall rate at the input time
//       for the input model.  The infall rate is computed from
//       the currently set infall cycle integrand.
//       If the model or time is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         print the infall rate at 8.5 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 8.5;
// printf(
//   "At %g Gyr, the infall rate is %g.\n",
//   d_time,
//   WnSimpleGce__computeInfallRate( p_model, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeInfallRate( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__computeTotalMass()">
//
//   <description>
//     <abstract>
//       Compute the total mass in the disk (or Solar annulus) for the given
//       model at the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, total, mass
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeTotalMass(
//   WnSimpleGce *self,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       total mass.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the total mass at the input time
//       for the input model. If the model or time is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         print the total mass at 4.5 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 4.5;
// printf(
//   "At %g Gyr, the total mass is %g.\n",
//   d_time,
//   WnSimpleGce__computeTotalMass( p_model, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeTotalMass( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__computeGasMass()">
//
//   <description>
//     <abstract>
//       Compute the gas mass in the disk (or Solar annulus) for the given
//       model at the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, gas, mass
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeGasMass(
//   WnSimpleGce *self,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       gas mass.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the total mass at the input time
//       for the input model. If the model or time is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         print the gas mass at 4.5 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 4.5;
// printf(
//   "At %g Gyr, the gas mass is %g.\n",
//   d_time,
//   WnSimpleGce__computeGasMass( p_model, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeGasMass( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__computeStarMass()">
//
//   <description>
//     <abstract>
//       Compute the mass in the disk (or Solar annulus) locked up in stars
//       (and stellar remnants) for the given model at the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, star, mass
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeStarMass(
//   WnSimpleGce *self,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       mass in stars and stellar remnants.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the mass in stars and remnants at the input time
//       for the input model. If the model or time is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         print the star mass at 3.2 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 3.2;
// printf(
//   "At %g Gyr, the mass locked in stars is %g.\n",
//   d_time,
//   WnSimpleGce__computeStarMass( p_model, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeStarMass( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__computeInfallCycleNumber()">
//
//   <description>
//     <abstract>
//       Compute the infall cycle number (integral of the infall rate divided
//       by the gas mass from zero to the input time) for the given model at
//       the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, cycle, number
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computeInfallCycleNumber(
//   WnSimpleGce *self,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       infall cycle number.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the infall cycle number at the input time
//       for the input model.
//       If the model or time is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         print the infall cycle number at 7.3 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 7.3;
// printf(
//   "At %g Gyr, the infall cycle number is %g.\n",
//   d_time,
//   WnSimpleGce__computeCycleNumber( p_model, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computeInfallCycleNumber( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__computePrimaryMetallicity()">
//
//   <description>
//     <abstract>
//       Compute the primary metallicity for the given model at the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, primary, metallicity
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__computePrimaryMetallicity(
//   WnSimpleGce *self,
//   double d_time
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       primary metallicity.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the primary metallicity at the input time
//       for the input model.
//       If the model or time is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         print the primary metallicity at 7.3 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 7.3;
// printf(
//   "At %g Gyr, the primary metallicity is %g.\n",
//   d_time,
//   WnSimpleGce__computePrimaryMetallicity( p_model, d_time )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__computePrimaryMetallicity( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__computeOmegaFromGasFraction()">
//
//   <description>
//     <abstract>
//       Compute the omega (in per Gyr) that gives the input gas fraction
//       (ratio of gas mass to total mass) at the input time.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, gas, ratio, omega
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// int 
// WnSimpleGce__computeOmegaFromGasFraction(
//   WnSimpleGce *self,
//   double d_time,
//   double d_gas_fraction
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_time"
//       kind="in,positional,required"
//     >
//       A double giving the time (in Gyr) at which to compute the
//       omega that gives the desired gas fraction.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the omega for the model has been updated
//       to the value that gives the desired gas fraction at the input time.
//       and the routine returns 1 (true).
//       If the model, time, or gas fraction is invalid, error handling is
//       invoked.  If the routine is unable to find the omega, the routine
//       returns 0 (false).
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         For the simple GCE model pointed to by p_model,
//         find the omega that gives a gas fraction of 0.091 at 13 Gyr:
//       </synopsis>
//
//       <code>
// d_time = 13.;
// d_gas_fraction = 0.091;
//
// WnSimpleGce__computeOmegaFromGasFraction(
//   p_model, d_time, d_gas_fraction
//  );
//
// printf(
//   "To get a gas fraction of %g at %g Gyr, omega must be %g per Gyr.\n",
//   d_gas_fraction,
//   d_time,
//   WnSimpleGce__getOmega( p_model )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

int
WnSimpleGce__computeOmegaFromGasFraction(
  WnSimpleGce *, double, double
);

/*##############################################################################
// <routine name="WnSimpleGce__getOmega()">
//
//   <description>
//     <abstract>
//       Retrieve the current value of omega (per Gyr) for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, omega
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__getOmega(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the omega (in per Gyr) for the input model.
//       If the model is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the omega for the simple GCE model pointed to by p_model:
//       </synopsis>
//
//       <code>
// printf(
//   "Omega for the model is %g per Gyr.\n",
//   WnSimpleGce__getOmega( p_model )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__getOmega( WnSimpleGce * );

/*##############################################################################
// <routine name="WnSimpleGce__updateInfallCycleFunctionsAndData()">
//
//   <description>
//     <abstract>
//       Set the infall cycle number function and integrand for the model.
//       If the user does not set these, the model uses the default model
//       (Clayton's standard model 1).
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateInfallCycleFunctionsAndData(
//   WnSimpleGce *self,
//   WnSimpleGce__infall_cycle_integrand pf_integrand,
//   void *p_integrand_data,
//   WnSimpleGce__infall_cycle_function pf_function,
//   void *p_function_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="pf_integrand"
//       kind="in,positional,required"
//     >
//       The name of the user's infall cycle number integrand.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined data structure containing extra data for
//       the user's infall cycle number integrand.
//     </param>
//
//     <param
//       name="pf_function"
//       kind="in,positional,required"
//     >
//       The name of the user's infall cycle number function.  This may
//       be NULL if the user wants the integrand numerically integrated.
//     </param>
//
//     <param
//       name="p_function_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined data structure containing extra data for
//       the user's infall cycle function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall integrand and function and their
//       associated data have updated.
//       If the model is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Set the infall cycle integrand for the simple GCE model p_model to 
//         my_infall_cycle_integrand, the user-supplied data structure
//         my_data containing the extra data to be applied by
//         my_infall_cycle_integrand, the infall cycle function to 
//         my_infall_cycle_function, and the extra data to be applied by
//         my_infall_cycle_function to my_data as well:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateInfallCycleFunctionsAndData(
//   p_model,
//   (WnSimpleGce__infall_cycle_integrand) my_infall_cycle_integrand,
//   &my_data,
//   (WnSimpleGce__infall_cycle_function) my_infall_cycle_function,
//   &my_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateInfallCycleFunctionsAndData(
  WnSimpleGce *,
  WnSimpleGce__infall_cycle_integrand,
  void *,
  WnSimpleGce__infall_cycle_function,
  void *
);

/*##############################################################################
// <routine name="WnSimpleGce__clearInfallCycleFunctionsAndData()">
//
//   <description>
//     <abstract>
//       Restore the infall cycle functions and data to their defaults.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__clearInfallCycleFunctionsAndData(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall integrand and function and their
//       associated data have reset to the default values.
//       If the model is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Reset the infall cycle functions and data for simple GCE model
//         pointed to by p_gce:
//       </synopsis>
//
//       <code>
// WnSimpleGce__clearInfallCycleFunctionsAndData(
//   p_model
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__clearInfallCycleFunctionsAndData(
  WnSimpleGce *
);

/*##############################################################################
// <routine name="WnSimpleGce__setInfallPrimaryMetallicityFunction()">
//
//   <description>
//     <abstract>
//       Set the infall primary metallicity function for the model.
//       If the user does not set this function, the infalling material is
//       assumed to have zero metallicity.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, primary, metallicity
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__setInfallPrimaryMetallicityFunction(
//   WnSimpleGce *self,
//   WnSimpleGce__infall_primary_metallicity_function pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="pf_func"
//       kind="in,positional,required"
//     >
//       The name of the user's infall primary metallicity function.
//       This must be cast as a
//       WnSimpleGce__infall_primary_metallicity_function.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined data structure containing extra data for
//       the user's infall primary metallicity function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall primary metallicity function has
//       been set to the input function. If the model is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Set the infall primary metallicity function for the simple GCE
//         model p_model to my_infall_primary_metallicity function and also
//         pass the user-supplied data structure
//         my_data containing the extra data to be applied by
//         my_infall_primary_metallicity_function:
//       </synopsis>
//
//       <code>
// WnSimpleGce__setInfallPrimaryMetallicityFunction(
//   p_model,
//   (WnSimpleGce__infall_primary_metallicity_function)
//      my_infall_primary_metallicity_function,
//   &my_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__setInfallPrimaryMetallicityFunction(
  WnSimpleGce *,
  WnSimpleGce__infall_primary_metallicity_function,
  void *
);

/*##############################################################################
// <routine name="WnSimpleGce__Species__setInfallMassFractionFunction()">
//
//   <description>
//     <abstract>
//       Set the infall mass fraction function for the species for the model.
//       If the user does not set this function, the species is
//       assumed to have zero mass fraction in the infalling matter.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, mass, fraction, species
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__Species__setInfallMassFractionFunction(
//   WnSimpleGce__Species *self,
//   WnSimpleGce__Species__infall_mass_fraction_function pf_func,
//   void *p_user_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce__Species structure.
//     </param>
//
//     <param
//       name="pf_func"
//       kind="in,positional,required"
//     >
//       The name of the user's infall mass fraction function for a species.
//       This must be cast as a
//       WnSimpleGce__Species__infall_mass_fraction_function.
//     </param>
//
//     <param
//       name="p_user_data"
//       kind="in,positional,required"
//     >
//       A pointer to a user-defined data structure containing extra data for
//       the user's infall mass fraction function.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall mass fraction function for the
//       species been set to the input function. If the species is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Set the infall mass fraction function for the species
//         p_species to my_infall_mass_fraction_function and also
//         pass the user-supplied data structure
//         my_data containing the extra data to be applied by
//         my_infall_mass_fraction_function:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__setInfallMassFractionFunction(
//   p_species,
//   (WnSimpleGce__Species__infall_mass_fraction_function)
//      my_infall_mass_fraction_function,
//   &my_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__Species__setInfallMassFractionFunction(
  WnSimpleGce__Species *,
  WnSimpleGce__Species__infall_mass_fraction_function,
  void *
);

/*##############################################################################
// <routine name="WnSimpleGce__clearInfallPrimaryMetallicityFunction()">
//
//   <description>
//     <abstract>
//       Clear the infall primary metallicity function for the model.
//       The routine is reset to the default--that is, zero infall primary
//       metallicity.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, primary, metallicity
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__clearInfallPrimaryMetallicityFunction(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall primary metallicity function has
//       been cleared and reset to the default.  If the input model is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Clear the infall primary metallicity function for the simple GCE
//         model p_model:
//       </synopsis>
//
//       <code>
// WnSimpleGce__clearInfallPrimaryMetallicityFunction(
//   p_model
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__clearInfallPrimaryMetallicityFunction(
  WnSimpleGce *
);

/*##############################################################################
// <routine name="WnSimpleGce__Species__clearInfallMassFractionFunction()">
//
//   <description>
//     <abstract>
//       Clear the infall mass fraction function for the species.
//       The routine is reset to the default--that is, zero mass fraction
//       for the species in the infall.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, species, mass, fraction
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__Species__clearInfallMassFractionFunction(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall mass fraction function for the
//       species been cleared and reset to the default.  If the input species
//       is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Clear the infall mass fraction function for the species p_species:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__clearInfallMassFractionFunction(
//   p_model
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__Species__clearInfallMassFractionFunction(
  WnSimpleGce__Species *
);

/*##############################################################################
// <routine name="WnSimpleGce__updateInitialGasMass()">
//
//   <description>
//     <abstract>
//       Update the initial gas mass for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, initial, gas, mass
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateInitialGasMass(
//   WnSimpleGce *self,
//   double d_initial_gas_mass
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_initial_gas_mass"
//       kind="in,positional,required"
//     >
//       A double giving the value to which the model's initial gas mass is to
//       be updated.  The units are arbitrary, but should be consistent with
//       the infall rate. 
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the initial gas mass of the model has been
//       updated to the input value.  If the model or the initial gas mass
//       is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the initial gas mass of the simple GCE model p_my_model
//         to 1.e5 (Solar masses):
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateInitialGasMass( p_my_model, 1.e5 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateInitialGasMass( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__updateInfallKValue()">
//
//   <description>
//     <abstract>
//       Update the k value for the default infall rate function (Clayton's
//       standard model 1).
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, k
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateInfallKValue(
//   WnSimpleGce *self,
//   unsigned int i_k
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="i_k"
//       kind="in,positional,required"
//     >
//       An unsigned int giving the value to which the infall k value is to
//       be updated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall k value of the model has been
//       updated to the input value.  If the model is invalid, error handling
//       is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the infall k value of the simple GCE model p_my_model
//         to 2:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateInfallKValue( p_my_model, 2 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateInfallKValue( WnSimpleGce *, unsigned int );

/*##############################################################################
// <routine name="WnSimpleGce__updateInfallDelta()">
//
//   <description>
//     <abstract>
//       Update the Delta (the time offset) for the default infall rate
//       function (Clayton's standard model 1).
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, Delta
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateInfallDelta(
//   WnSimpleGce *self,
//   double d_Delta
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_Delta"
//       kind="in,positional,required"
//     >
//       A double giving the value to which the infall Delta value (in Gyr)
//       is to be updated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall Delta value of the model has been
//       updated to the input value.  If the model is invalid, error handling
//       is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the infall Delta value of the simple GCE model p_my_model
//         to 0.2 Gyr:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateInfallDelta( p_my_model, 0.2 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateInfallDelta( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__updateOmega()">
//
//   <description>
//     <abstract>
//       Update the omega (the gas consumption rate--in per Gyr) for the
//       input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, omega, gas, consumption, rate
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateOmega(
//   WnSimpleGce *self,
//   double d_omega
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_omega"
//       kind="in,positional,required"
//     >
//       A double giving the value to which the omega (in per Gyr)
//       is to be updated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the omega of the model has been
//       updated to the input value.  If the model is invalid, error handling
//       is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the omega of the simple GCE model p_my_model
//         to 0.5 per Gyr:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateOmega( p_my_model, 0.5 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateOmega( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__updatePrimaryMetallicityYield()">
//
//   <description>
//     <abstract>
//       Update the primary metallicity yield for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, primary, metallicity, yield
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updatePrimaryMetallicityYield(
//   WnSimpleGce *self,
//   double d_primary_metallicity_yield
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_primary_metallicity_yield"
//       kind="in,positional,required"
//     >
//       A double giving the value to which the primary metallicity yield
//       is to be updated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the primary metallicity yield of the model has
//       been updated to the input value.  If the model is invalid, error handling
//       is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the primary metallicity yield of the simple GCE model p_my_model
//         to 0.015:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updatePrimaryMetallicityYield( p_my_model, 0.015 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updatePrimaryMetallicityYield( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__updateInitialPrimaryMetallicity()">
//
//   <description>
//     <abstract>
//       Update the initialprimary metallicity for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, primary, metallicity, initial
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateInitialPrimaryMetallicity(
//   WnSimpleGce *self,
//   double d_initial_primary_metallicity
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_initial_primary_metallicity"
//       kind="in,positional,required"
//     >
//       A double giving the value to which the primary metallicity yield
//       is to be updated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the initial primary metallicity of the model
//       has been updated to the input value.  If the model is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the initial primary metallicity of the simple GCE model
//         p_my_model to 0.015:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateInitialPrimaryMetallicity( p_my_model, 0.015 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateInitialPrimaryMetallicity( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__updateInfallPrimaryMetallicity()">
//
//   <description>
//     <abstract>
//       Update the infall primary metallicity for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, primary, metallicity, infall
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__updateInfallPrimaryMetallicity(
//   WnSimpleGce *self,
//   double d_infall_primary_metallicity
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       name="d_infall_primary_metallicity"
//       kind="in,positional,required"
//     >
//       A double giving the value to which the infall primary metallicity
//       is to be updated.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Upon successful return, the infall primary metallicity of the model has
//       been updated to the input value.  If the model is invalid,
//       error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the infall primary metallicity of the simple GCE model
//         p_my_model to 0.005:
//       </synopsis>
//
//       <code>
// WnSimpleGce__updateInfallPrimaryMetallicity( p_my_model, 0.005 );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__updateInfallPrimaryMetallicity( WnSimpleGce *, double );

/*##############################################################################
// <routine name="WnSimpleGce__getInfallKValue()">
//
//   <description>
//     <abstract>
//       Retrieve the current value of the infall k for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, k
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// unsigned int
// WnSimpleGce__getInfallKValue(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the current infall k value for the input model.
//       If the model is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the k for the simple GCE model pointed to by p_model:
//       </synopsis>
//
//       <code>
// printf(
//   "The infall k for the model is %d per Gyr.\n",
//   WnSimpleGce__getInfallKValue( p_model )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

unsigned int
WnSimpleGce__getInfallKValue( WnSimpleGce * );

/*##############################################################################
// <routine name="WnSimpleGce__getInitialGasMass()">
//
//   <description>
//     <abstract>
//       Retrieve the current value of the initial mass.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, initial, mass
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__getInitialGasMass(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the current initial mass for the input model.
//       If the model is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the initial mass for the simple GCE model pointed to by
//         p_model:
//       </synopsis>
//
//       <code>
// printf(
//   "The initial mass for the model is %g solar masses.\n",
//   WnSimpleGce__getInitialGasMass( p_model )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__getInitialGasMass( WnSimpleGce * );

/*##############################################################################
// <routine name="WnSimpleGce__getInfallDelta()">
//
//   <description>
//     <abstract>
//       Retrieve the current value of Delta (in Gyr) for the input model.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, infall, Delta
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__getInfallDelta(
//   WnSimpleGce *self
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce model.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns the current Delta (in Gyr) for the input model.
//       If the model is invalid, error handling is
//       invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the Delta for the simple GCE model pointed to by p_model:
//       </synopsis>
//
//       <code>
// printf(
//   "Delta for the model is %g per Gyr.\n",
//   WnSimpleGce__getInfallDelta( p_model )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__getInfallDelta( WnSimpleGce * );

/*##############################################################################
// <routine name="WnSimpleGce__Species__updateYieldFunction()">
//
//   <description>
//     <abstract>
//       Update the yield function for a species to the input function.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, yield, function
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__Species__updateYieldFunction(
//   WnSimpleGce__Species * self,
//   WnSimpleGce__Species__yield_function pf_func
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce species.
//     </param>
//
//     <param
//       name="pf_func"
//       kind="in,positional,required"
//     >
//       The user-defined function. 
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the yield function for the species has been
//       set to the input value.  If the input yield function is NULL,
//       the function has been reset to the default function.
//       If the species is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Set the yield function for p_species to my_yield_function:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__updateYieldFunction(
//   p_species, 
//   (WnSimpleGce__Species__yield_function) my_yield_function
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__Species__updateYieldFunction(
  WnSimpleGce__Species *,
  WnSimpleGce__Species__yield_function
);

/*##############################################################################
// <routine name="WnSimpleGce__Species__updateUserYieldFunctionData()">
//
//   <description>
//     <abstract>
//       Update the data for a user-defined species yield function.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, yield, data
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// void
// WnSimpleGce__Species__updateUserYieldFunctionData(
//   WnSimpleGce__Species * self,
//   void * p_data
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce species.
//     </param>
//
//     <param
//       name="p_data"
//       kind="in,positional,required"
//     >
//       A pointer to the user-defined data structure.
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       On successful return, the yield function data for the species has been
//       updated to the input value.
//       If the species is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Update the yield function data for p_species to p_my_data:
//       </synopsis>
//
//       <code>
// WnSimpleGce__Species__updateUserYieldFunctionData(
//   p_species, 
//   p_data
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

void
WnSimpleGce__Species__updateUserYieldFunctionData(
  WnSimpleGce__Species *,
  void *
);

/*##############################################################################
// <routine name="WnSimpleGce__Species__getYieldCoefficient()">
//
//   <description>
//     <abstract>
//       Retrieve the current yield coefficient for the default yield function.
//     </abstract>
//     <keywords>
//       Galactic, chemical, evolution, model, yield, coefficient
//     </keywords>
//   </description>
//
//   <usage>
//
//     <calling_sequence>
// double
// WnSimpleGce__Species__getYieldCoefficient(
//   WnSimpleGce__Species * self,
//   unsigned int i_n
// );
//     </calling_sequence>
//
//     <param
//       name="self"
//       kind="in,positional,required"
//     >
//       A pointer to a WnSimpleGce species.
//     </param>
//
//     <param
//       name="i_n"
//       kind="in,positional,required"
//     >
//       An unsigned integer giving the coefficient of the term in the default
//       yield power series:
//     </param>
//
//     <param
//       kind="return"
//       doc="result"
//     />
//
//     <doc kind="post" id="result">
//       Routine returns current value for the coefficient.  If the coefficient
//       is not set, routine returns zero.
//       If the species is invalid, error handling is invoked.
//     </doc>
//
//     <doc kind="bug" >
//     </doc>
//
//     <doc kind="example" id="example">
//       <synopsis>
//         Print the currently set primary yield coefficient for p_species:
//       </synopsis>
//
//       <code>
// printf(
//   "The alpha coefficient is: %g\n",
//   WnSimpleGce__Species__getYieldCoefficient(
//     p_species, 
//     0
//   )
// );
//       </code>
//     </doc>
//
//   </usage>
//
// </routine>
//############################################################################*/

double
WnSimpleGce__Species__getYieldCoefficient(
  WnSimpleGce__Species *,
  unsigned int
);

/*##############################################################################
// Non-API routines.
//############################################################################*/

void
WnSimpleGce__Species__YieldEntry__free( WnSimpleGce__Species__YieldEntry * );

double
WnSimpleGce__defaultInfallCycleFunction( double, void * );

double
WnSimpleGce__defaultInfallCycleIntegrand( double, void * );

double
WnSimpleGce__starMassIntegrand( double, void * );

double
WnSimpleGce__Species__massFractionIntegrand( double, void * );

double
WnSimpleGce__primaryMetallicityIntegrand( double, void * );

double
WnSimpleGce__infallPrimaryMetallicityIntegrand( double, void * );

void
WnSimpleGce__computeSpeciesYieldCallback(
  WnSimpleGce__Species__YieldEntry *,
  void *,
  xmlChar *
);

void
WnSimpleGce__Species__YieldEntry__free(
  WnSimpleGce__Species__YieldEntry *
);

double
WnSimpleGce__computeOmegaFromGasFractionHelper( double, void * );

int
WnSimpleGce__bracket_root_of_function(
  gsl_function, double *, double *, void *
);

double
WnSimpleGce__compute_Clayton_integral(
  unsigned int, double, double, double
);

double
WnSimpleGce__integrator( WnSimpleGce__integrand, double, double, void * );

#ifdef __cplusplus
}
#endif

#endif  /* WN_SIMPLE_GCE__H */

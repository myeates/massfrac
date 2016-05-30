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
//
//////////////////////////////////////////////////////////////////////////////*/

#include <WnSimpleGce.h>

/*##############################################################################
// WnSimpleGce__new().
//############################################################################*/

WnSimpleGce *
WnSimpleGce__new( void )
{

  WnSimpleGce * self;

  self = ( WnSimpleGce * ) malloc( sizeof( WnSimpleGce ) );

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Couldn't allocate memory" );

  /*============================================================================
  // Set data.
  //==========================================================================*/

  self->dInitialGasMass = D_INITIAL_GAS_MASS;
  self->iK = I_K;
  self->dDelta = D_DELTA;
  self->dAlpha = D_ALPHA;
  self->dOmega = D_OMEGA;
  self->dInitialPrimaryMetallicity = 0.;

  /*============================================================================
  // Set infall functions and data.
  //==========================================================================*/

  self->pfInfallCycleFunction =
    (WnSimpleGce__infall_cycle_function)
    WnSimpleGce__defaultInfallCycleFunction;
  self->pInfallCycleFunctionData = self;

  self->pfInfallCycleIntegrand =
    (WnSimpleGce__infall_cycle_integrand)
    WnSimpleGce__defaultInfallCycleIntegrand;
  self->pInfallCycleIntegrandData = self;

  self->pfInfallPrimaryMetallicityFunction = NULL;
  self->pInfallPrimaryMetallicityUserData = NULL;

  return self;

}

/*##############################################################################
// WnSimpleGce__updateInitialGasMass().
//############################################################################*/

void
WnSimpleGce__updateInitialGasMass(
  WnSimpleGce *self, double d_initial_gas_mass
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model structure" );

  if( d_initial_gas_mass < 0 )
    WN_SIMPLE_GCE__ERROR( "Initial gas mass should be non-negative" );

  self->dInitialGasMass = d_initial_gas_mass;

}

/*##############################################################################
// WnSimpleGce__updateInfallKValue().
//############################################################################*/

void
WnSimpleGce__updateInfallKValue(
  WnSimpleGce *self, unsigned int i_k
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model structure" );

  self->iK = i_k;

}

/*##############################################################################
// WnSimpleGce__updateInfallDelta().
//############################################################################*/

void
WnSimpleGce__updateInfallDelta(
  WnSimpleGce *self, double d_Delta
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model structure" );

  if( d_Delta < 0 )
    WN_SIMPLE_GCE__ERROR( "Infall Delta should be non-negative" );

  self->dDelta = d_Delta;

}

/*##############################################################################
// WnSimpleGce__updateOmega().
//############################################################################*/

void
WnSimpleGce__updateOmega(
  WnSimpleGce *self, double d_omega
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model structure" );

  if( d_omega < 0 )
    WN_SIMPLE_GCE__ERROR( "Omega should be non-negative" );

  self->dOmega = d_omega;

}

/*##############################################################################
// WnSimpleGce__updatePrimaryMetallicityYield().
//############################################################################*/

void
WnSimpleGce__updatePrimaryMetallicityYield(
  WnSimpleGce *self, double d_alpha
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model structure" );

  self->dAlpha = d_alpha;

}

/*##############################################################################
// WnSimpleGce__free().
//############################################################################*/

void
WnSimpleGce__free( WnSimpleGce *self )
{

  free( self );

}

/*##############################################################################
// WnSimpleGce__Species__new().
//############################################################################*/

WnSimpleGce__Species *
WnSimpleGce__Species__new( const char *s_name )
{

  WnSimpleGce__Species *self =
    ( WnSimpleGce__Species * ) malloc( sizeof( WnSimpleGce__Species ) );

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Couldn't allocate memory for species" );

  self->sxName = xmlCharStrdup( s_name );

  self->dDecayRate = 0.;
  self->dInitialMassFraction = 0.;

  self->pfInfallMassFractionFunction = NULL;
  self->pInfallMassFractionUserData = NULL;

  self->pfYieldFunction = NULL;
  self->pYieldFunctionUserData = NULL;

  self->pYieldHash = xmlHashCreate( 0 );

  return self;

}

/*##############################################################################
// WnSimpleGce__Species__updateDecayRate().
//############################################################################*/

void
WnSimpleGce__Species__updateDecayRate(
  WnSimpleGce__Species *self, double d_decay_rate
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid input model structure" );

  if( d_decay_rate < 0. )
    WN_SIMPLE_GCE__ERROR( "Decay rate must be non-negative" );
    
  self->dDecayRate = d_decay_rate;

}

/*##############################################################################
// WnSimpleGce__Species__free().
//############################################################################*/

void
WnSimpleGce__Species__free( WnSimpleGce__Species *self )
{
 
  xmlFree( self->sxName );

  xmlHashFree(
    self->pYieldHash,
    (xmlHashDeallocator) WnSimpleGce__Species__YieldEntry__free
  );

  free( self );

} 

/*##############################################################################
// WnSimpleGce__Species__YieldEntry__free().
//############################################################################*/

void
WnSimpleGce__Species__YieldEntry__free(
  WnSimpleGce__Species__YieldEntry *self
)
{

  free( self );

}

/*##############################################################################
// WnSimpleGce__Species__updateYieldCoefficient().
//############################################################################*/

int
WnSimpleGce__Species__updateYieldCoefficient(
  WnSimpleGce__Species *self,
  unsigned int i_n,
  double d_value
)
{

  xmlChar s_n[256];

  WnSimpleGce__Species__YieldEntry *p_yield;

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid species" );

  p_yield =
    ( WnSimpleGce__Species__YieldEntry * )
    malloc( sizeof( WnSimpleGce__Species__YieldEntry ) );

  p_yield->dValue = d_value;

  xmlStrPrintf( s_n, 256, (const xmlChar *) "%d", i_n );

  return
    xmlHashUpdateEntry(
      self->pYieldHash,
      s_n,
      p_yield,
      (xmlHashDeallocator) WnSimpleGce__Species__YieldEntry__free
    );

}

/*##############################################################################
// WnSimpleGce__Species__updateYieldFunction().
//############################################################################*/

void
WnSimpleGce__Species__updateYieldFunction(
  WnSimpleGce__Species *self,
  WnSimpleGce__Species__yield_function pf_func
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid species" );

  self->pfYieldFunction = pf_func;

}

/*##############################################################################
// WnSimpleGce__Species__getYieldCoefficient().
//############################################################################*/

double
WnSimpleGce__Species__getYieldCoefficient(
  WnSimpleGce__Species *self,
  unsigned int i_n
)
{

  xmlChar sx_n[256];
  WnSimpleGce__Species__YieldEntry *p_yield_entry;

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid species" );

  xmlStrPrintf( sx_n, 256, (const xmlChar *) "%d", i_n );

  p_yield_entry =
    ( WnSimpleGce__Species__YieldEntry * )
    xmlHashLookup(
      self->pYieldHash,
      sx_n
    );
      
  if( p_yield_entry )
    return p_yield_entry->dValue;
  else
    return 0;

}

/*##############################################################################
// WnSimpleGce__Species__updateUserYieldFunctionData().
//############################################################################*/

void
WnSimpleGce__Species__updateUserYieldFunctionData(
  WnSimpleGce__Species *self,
  void * p_data
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid species" );

  self->pYieldFunctionUserData = p_data;

}

/*##############################################################################
// WnSimpleGce__Species__getName().
//############################################################################*/

const char *
WnSimpleGce__Species__getName( WnSimpleGce__Species *self ) {

  return ( const char * ) self->sxName;

}

/*##############################################################################
// WnSimpleGce__computeSpeciesYield().
//############################################################################*/

double
WnSimpleGce__computeSpeciesYield(
  WnSimpleGce *self,
  WnSimpleGce__Species *p_species,
  double d_time
)
{

  typedef struct {
    double dYield;
    double dPrimaryMetallicity;
  } extra_data;

  double d_result;

  if( !p_species->pfYieldFunction )
  {

    extra_data *p_extra_data =
      ( extra_data * ) malloc( sizeof( extra_data ) );

    p_extra_data->dYield = 0.;
    p_extra_data->dPrimaryMetallicity =
      WnSimpleGce__computePrimaryMetallicity( self, d_time );

    xmlHashScan(
      p_species->pYieldHash,
      (xmlHashScanner) WnSimpleGce__computeSpeciesYieldCallback,
      p_extra_data
    );

    d_result = p_extra_data->dYield;

    free( p_extra_data );

  }
  else
  {
    d_result =
      p_species->pfYieldFunction(
        WnSimpleGce__computePrimaryMetallicity( self, d_time ),
        p_species->pYieldFunctionUserData
      );
  }
      
  return d_result;

}

/*##############################################################################
// WnSimpleGce__computeSpeciesYieldCallback().
//############################################################################*/

void
WnSimpleGce__computeSpeciesYieldCallback(
  WnSimpleGce__Species__YieldEntry *p_yield,
  void *p_data,
  xmlChar *sx
)
{

  typedef struct {
    double dYield;
    double dPrimaryMetallicity;
  } extra_data;

  extra_data *p_extra_data = ( extra_data * ) p_data;

  p_extra_data->dYield +=
    pow(
      p_extra_data->dPrimaryMetallicity,
      atof( (char *) sx )
    ) *
    p_yield->dValue;

}

/*##############################################################################
// WnSimpleGce__computeSpeciesMassFraction().
//############################################################################*/

double
WnSimpleGce__computeSpeciesMassFraction(
  WnSimpleGce *self,
  WnSimpleGce__Species *p_species,
  double d_time
)
{

  typedef struct {
    WnSimpleGce *pGce;
    WnSimpleGce__Species *pSpecies;
    double dTime;
  } extra_data;

  size_t i;
  double d_start_time, d_end_time, d_part, d_check, d_step;
  double d_result;

  extra_data *p_extra_data = ( extra_data * ) malloc( sizeof( extra_data ) );

  p_extra_data->pGce = self;
  p_extra_data->pSpecies = p_species;
  p_extra_data->dTime = d_time;

  /*============================================================================
  // If stable, do integral in one step.
  //==========================================================================*/

  if( p_species->dDecayRate == 0. ) {
    d_result =
      WnSimpleGce__integrator(
        (WnSimpleGce__integrand) WnSimpleGce__Species__massFractionIntegrand,
        0.,
        d_time,
        p_extra_data
      );
  }

  /*============================================================================
  // If radioactive, do integral in multiple steps backwards.
  //==========================================================================*/

  else {

    d_check = 1.;

    d_step = D_FACTOR / p_species->dDecayRate;
   
    d_end_time = d_time;

    d_start_time = d_time - d_step;

    d_result = 0.;

    for( i = 0; i < 100; i++ ) 
    {

      if( d_start_time < 0. ) d_start_time = 0.;

      d_part =
        WnSimpleGce__integrator(
          (WnSimpleGce__integrand)
             WnSimpleGce__Species__massFractionIntegrand,
          d_start_time,
          d_end_time,
          p_extra_data
        );

        d_result += d_part;

        d_check = fabs( d_part / d_result );

        if( d_check < D_CONVERGE ) break;

        if( d_start_time <= 0. ) break;

        d_end_time = d_start_time;

        d_start_time = d_end_time - d_step;

    }

  }

  d_result +=
    exp(
      -d_time * p_species->dDecayRate - 
      WnSimpleGce__computeInfallCycleNumber( self, d_time )
    ) * p_species->dInitialMassFraction;

  free( p_extra_data );

  return d_result;

}

/*##############################################################################
// WnSimpleGce__Species__massFractionIntegrand().
//############################################################################*/

double
WnSimpleGce__Species__massFractionIntegrand(
  double d_time,
  void *p_data
)
{

  typedef struct {
    WnSimpleGce *pGce;
    WnSimpleGce__Species *pSpecies;
    double dTime;
  } extra_data;

  double d_result;

  extra_data *p_extra_data = ( extra_data * ) p_data;

  d_result =
    p_extra_data->pGce->dOmega *
    WnSimpleGce__computeSpeciesYield(
      p_extra_data->pGce, p_extra_data->pSpecies, d_time
    );

  if( p_extra_data->pSpecies->pfInfallMassFractionFunction )
    d_result +=
      p_extra_data->pSpecies->pfInfallMassFractionFunction(
        d_time,
        p_extra_data->pSpecies->pInfallMassFractionUserData
      ) *
      p_extra_data->pGce->pfInfallCycleIntegrand(
        d_time,
        p_extra_data->pGce->pInfallCycleIntegrandData
      );

  return
    d_result *=
      exp(
        -( p_extra_data->dTime - d_time ) * p_extra_data->pSpecies->dDecayRate
        -(
           WnSimpleGce__computeInfallCycleNumber(
             p_extra_data->pGce,
             p_extra_data->dTime
           )
           -
           WnSimpleGce__computeInfallCycleNumber(
             p_extra_data->pGce,
             d_time
           )
         )
      );

} 

/*##############################################################################
// WnSimpleGce__defaultInfallCycleFunction().
//############################################################################*/

double
WnSimpleGce__defaultInfallCycleFunction(
  double d_time,
  void *p_data
)
{

  WnSimpleGce *p_gce = ( WnSimpleGce * ) p_data;

  return
    (double) WnSimpleGce__getInfallKValue( p_gce ) *
    log(
      ( d_time + WnSimpleGce__getInfallDelta( p_gce ) ) /
      WnSimpleGce__getInfallDelta( p_gce )
    ); 

}

/*##############################################################################
// WnSimpleGce__defaultInfallCycleIntegrand().
//############################################################################*/

double
WnSimpleGce__defaultInfallCycleIntegrand(
  double d_time,
  void *p_data
)
{

  WnSimpleGce *p_gce = ( WnSimpleGce * ) p_data;

  return
    (double) WnSimpleGce__getInfallKValue( p_gce ) /
    ( d_time + WnSimpleGce__getInfallDelta( p_gce ) );

}

/*##############################################################################
// WnSimpleGce__computeTotalMass().
//############################################################################*/

double
WnSimpleGce__computeTotalMass(
  WnSimpleGce *self,
  double d_time
)
{

  return
    WnSimpleGce__computeGasMass( self, d_time ) +
    WnSimpleGce__computeStarMass( self, d_time );

}

/*##############################################################################
// WnSimpleGce__computeInfallRate().
//############################################################################*/

double
WnSimpleGce__computeInfallRate(
  WnSimpleGce *self,
  double d_time
)
{

  return
    self->pfInfallCycleIntegrand(
      d_time,
      self->pInfallCycleIntegrandData
    ) *
    WnSimpleGce__computeGasMass( self, d_time );

}

/*##############################################################################
// WnSimpleGce__computeGasMass().
//############################################################################*/

double
WnSimpleGce__computeGasMass(
  WnSimpleGce *self,
  double d_time
)
{

  return
    self->dInitialGasMass *
    exp(
      WnSimpleGce__computeInfallCycleNumber( self, d_time )
      - WnSimpleGce__getOmega( self ) * d_time
    );

}

/*##############################################################################
// WnSimpleGce__computeStarMass().
//############################################################################*/

double
WnSimpleGce__computeStarMass(
  WnSimpleGce *self,
  double d_time
)
{

  return
    WnSimpleGce__integrator(
      (WnSimpleGce__integrand) WnSimpleGce__starMassIntegrand,
      0.,
      d_time,
      self
    );

}

/*##############################################################################
// WnSimpleGce__starMassIntegrand().
//############################################################################*/

double
WnSimpleGce__starMassIntegrand(
  double d_time, void *p_data
)
{

  WnSimpleGce * p_gce = ( WnSimpleGce * ) p_data;

  return
    p_gce->dOmega * WnSimpleGce__computeGasMass( p_gce, d_time );

}

/*##############################################################################
// WnSimpleGce__computeOmegaFromGasFraction().
//############################################################################*/

int
WnSimpleGce__computeOmegaFromGasFraction(
  WnSimpleGce *self,
  double d_time,
  double d_gas_fraction
)
{

  typedef struct {
    WnSimpleGce *pGce;
    double dTime;
    double dFraction;
  } extra_data;

  int i_status;
  int iter = 0, max_iter = 100;
  double d_lo = 0., d_hi = 1.;
  const gsl_root_fsolver_type *T;
  gsl_root_fsolver *s;

  gsl_function F;

  extra_data *p_extra_data = ( extra_data * ) malloc( sizeof( extra_data ) );

  if( !p_extra_data )
    WN_SIMPLE_GCE__ERROR( "Couldn't allocate memory" );

  p_extra_data->pGce = self;
  p_extra_data->dTime = d_time;
  p_extra_data->dFraction = d_gas_fraction;
     
  F.function = &WnSimpleGce__computeOmegaFromGasFractionHelper;
  F.params = p_extra_data;
     
  T = gsl_root_fsolver_brent;
  s = gsl_root_fsolver_alloc (T);

  if(
      !WnSimpleGce__bracket_root_of_function (
         F, &d_lo, &d_hi, F.params
      )
  )
    WN_SIMPLE_GCE__ERROR( "Couldn't bracket root" );

  gsl_root_fsolver_set( s, &F, d_lo, d_hi );

  do
     {
       iter++;
       i_status = gsl_root_fsolver_iterate (s);
       self->dOmega = gsl_root_fsolver_root (s);
       d_lo = gsl_root_fsolver_x_lower (s);
       d_hi = gsl_root_fsolver_x_upper (s);
       i_status =
         gsl_root_test_interval (d_lo, d_hi, 0., 0.001);
     }
  while( i_status == GSL_CONTINUE && iter < max_iter );

  if( i_status != GSL_SUCCESS || iter >= max_iter )
    return 0;
     
  gsl_root_fsolver_free (s);

  free( p_extra_data );

  return 1;
   
}

/*##############################################################################
// WnSimpleGce__computeOmegaFromGasFractionHelper().
//############################################################################*/

double
WnSimpleGce__computeOmegaFromGasFractionHelper(
  double d_omega, void *p_data
)
{

  typedef struct {
    WnSimpleGce *pGce;
    double dTime;
    double dFraction;
  } extra_data;

  extra_data *p_extra_data = ( extra_data * ) p_data;

  p_extra_data->pGce->dOmega = d_omega;

  return
    p_extra_data->dFraction -
    WnSimpleGce__computeGasMass( p_extra_data->pGce, p_extra_data->dTime ) /
    WnSimpleGce__computeTotalMass( p_extra_data->pGce, p_extra_data->dTime );
  
}

/*##############################################################################
// WnSimpleGce__bracket_of_root_function()
//############################################################################*/

int
WnSimpleGce__bracket_root_of_function(
  gsl_function F, double *p_x1, double *p_x2, void *params
)
{

  double d_f1, d_f2, d_factor = 1.6, d_xT, d_fT;
  int i_iter = 0, i_max_iter = 1000;

  d_f1 = F.function( *p_x1, params );
  d_f2 = F.function( *p_x2, params );

  while( i_iter < i_max_iter ) {
    i_iter++;
    if( d_f1 * d_f2 < 0 ) return 1;
    if( fabs( d_f1 ) < fabs( d_f2 ) ) {
      d_xT = *p_x1;
      d_fT = d_f1;
      *p_x1 = *p_x1 + d_factor * ( *p_x1 - *p_x2 );
      *p_x2 = d_xT;
      d_f1 = F.function( *p_x1, params );
      d_f2 = d_fT;
    } else {
      d_xT = *p_x2;
      d_fT = d_f2;
      *p_x2 = *p_x2 + d_factor * ( *p_x2 - *p_x1 );
      *p_x1 = d_xT;
      d_f2 = F.function( *p_x2, params );
      d_f1 = d_fT;
    }
  }

  return 0;

}

/*##############################################################################
// WnSimpleGce__updateInfallCycleFunctionsAndData().
//############################################################################*/

void
WnSimpleGce__updateInfallCycleFunctionsAndData(
  WnSimpleGce *self,
  WnSimpleGce__infall_cycle_integrand pf_integrand,
  void *p_integrand_data,
  WnSimpleGce__infall_cycle_function pf_function,
  void *p_function_data
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model" );

  if( !pf_integrand )
    WN_SIMPLE_GCE__ERROR( "User must supply integrand" );

  self->pfInfallCycleIntegrand = pf_integrand;
  self->pInfallCycleIntegrandData = p_integrand_data;
  self->pfInfallCycleFunction = pf_function;
  self->pInfallCycleFunctionData = p_function_data;

}

/*##############################################################################
// WnSimpleGce__clearInfallCycleFunctionAndIntegrand().
//############################################################################*/

void
WnSimpleGce__clearInfallCycleFunctionsAndData(
  WnSimpleGce *self
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid model" );

  self->pfInfallCycleIntegrand =
    (WnSimpleGce__infall_cycle_integrand)
    WnSimpleGce__defaultInfallCycleIntegrand;
  self->pInfallCycleIntegrandData = self;

  self->pfInfallCycleFunction =
    (WnSimpleGce__infall_cycle_function)
    WnSimpleGce__defaultInfallCycleFunction;
  self->pInfallCycleFunctionData = self;

}

/*##############################################################################
// WnSimpleGce__getOmega().
//############################################################################*/

double
WnSimpleGce__getOmega( WnSimpleGce *self )
{

  return self->dOmega;

}

/*##############################################################################
// WnSimpleGce__updateInitialPrimaryMetallicity().
//############################################################################*/

void
WnSimpleGce__updateInitialPrimaryMetallicity(
  WnSimpleGce *self, double d_metallicity
)
{

  self->dInitialPrimaryMetallicity = d_metallicity;

}

/*##############################################################################
// WnSimpleGce__setInfallPrimaryMetallicityFunction().
//############################################################################*/

void
WnSimpleGce__setInfallPrimaryMetallicityFunction(
  WnSimpleGce *self,
  WnSimpleGce__infall_primary_metallicity_function pf_func,
  void *p_user_data
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid input" );

  self->pfInfallPrimaryMetallicityFunction = pf_func;
  self->pInfallPrimaryMetallicityUserData = p_user_data;

}

/*##############################################################################
// WnSimpleGce__clearInfallPrimaryMetallicityFunction().
//############################################################################*/

void
WnSimpleGce__clearInfallPrimaryMetallicityFunction(
  WnSimpleGce *self
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid input" );

  self->pfInfallPrimaryMetallicityFunction = NULL;
  self->pInfallPrimaryMetallicityUserData = NULL;

}

/*##############################################################################
// WnSimpleGce__Species__setInfallMassFractionFunction().
//############################################################################*/

void
WnSimpleGce__Species__setInfallMassFractionFunction(
  WnSimpleGce__Species *self,
  WnSimpleGce__Species__infall_mass_fraction_function pf_func,
  void *p_user_data
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid input" );

  self->pfInfallMassFractionFunction = pf_func;
  self->pInfallMassFractionUserData = p_user_data;

}

/*##############################################################################
// WnSimpleGce__Species__clearInfallMassFractionFunction().
//############################################################################*/

void
WnSimpleGce__Species__clearInfallMassFractionFunction(
  WnSimpleGce__Species *self
)
{

  if( !self )
    WN_SIMPLE_GCE__ERROR( "Invalid input" );

  self->pfInfallMassFractionFunction = NULL;
  self->pInfallMassFractionUserData = NULL;

}

/*##############################################################################
// WnSimpleGce__getInitialMass().
//############################################################################*/

double
WnSimpleGce__getInitialGasMass( WnSimpleGce *self ) {

  if( !self ) WN_SIMPLE_GCE__ERROR( "Invalid input" );

  return self->dInitialGasMass;

}

/*##############################################################################
// WnSimpleGce__getInfallKValue().
//############################################################################*/

unsigned int
WnSimpleGce__getInfallKValue( WnSimpleGce *self ) {

  return self->iK;

}

/*##############################################################################
// WnSimpleGce__getInfallDelta().
//############################################################################*/

double
WnSimpleGce__getInfallDelta( WnSimpleGce *self ) {

  return self->dDelta;

}

/*##############################################################################
// WnSimpleGce__computePrimaryMetallicity().
//############################################################################*/

double
WnSimpleGce__computePrimaryMetallicity(
  WnSimpleGce *self,
  double d_time
)
{

  typedef struct {
    WnSimpleGce *pGce;
    double dTime;
  } extra_data;

  double d_result;

  extra_data *p_extra_data = ( extra_data * ) malloc( sizeof( extra_data ) );

  if( !p_extra_data )
    WN_SIMPLE_GCE__ERROR( "Couldn' allocate memory" );

  p_extra_data->pGce = self;
  p_extra_data->dTime = d_time;

  d_result =
    (
      self->dAlpha * self->dOmega *
      WnSimpleGce__integrator(
        (WnSimpleGce__integrand)
           WnSimpleGce__primaryMetallicityIntegrand,
        0.,
        d_time,
        p_extra_data
      ) +
      exp( -WnSimpleGce__computeInfallCycleNumber( self, d_time ) ) *
      self->dInitialPrimaryMetallicity
    );

  if( self->pfInfallPrimaryMetallicityFunction )
    d_result +=
      WnSimpleGce__integrator(
        (WnSimpleGce__integrand)
           WnSimpleGce__infallPrimaryMetallicityIntegrand,
        0.,
        d_time,
        p_extra_data
      );

  free( p_extra_data );

  return d_result;

}

/*##############################################################################
// WnSimpleGce__primaryMetallicityIntegrand().
//############################################################################*/

double
WnSimpleGce__primaryMetallicityIntegrand(
  double d_time,
  void *p_data
)
{

  typedef struct {
    WnSimpleGce *pGce;
    double dTime;
  } extra_data;

  extra_data *p_extra_data = ( extra_data * ) p_data;

  return
    exp(
      -WnSimpleGce__computeInfallCycleNumber(
         p_extra_data->pGce, p_extra_data->dTime
      ) +
      WnSimpleGce__computeInfallCycleNumber( p_extra_data->pGce, d_time )
    );

}

/*##############################################################################
// WnSimpleGce__infallPrimaryMetallicityIntegrand().
//############################################################################*/

double
WnSimpleGce__infallPrimaryMetallicityIntegrand(
  double d_time,
  void *p_data
)
{

  typedef struct {
    WnSimpleGce *pGce;
    double dTime;
  } extra_data;

  extra_data *p_extra_data = ( extra_data * ) p_data;

  return
    exp(
      -WnSimpleGce__computeInfallCycleNumber(
         p_extra_data->pGce, p_extra_data->dTime
      ) +
      WnSimpleGce__computeInfallCycleNumber( p_extra_data->pGce, d_time )
    ) *
    p_extra_data->pGce->pfInfallPrimaryMetallicityFunction(
      d_time,
      p_extra_data->pGce->pInfallPrimaryMetallicityUserData
    ) *
    p_extra_data->pGce->pfInfallCycleIntegrand(
      d_time,
      p_extra_data->pGce->pInfallCycleIntegrandData
    );

}

/*##############################################################################
// WnSimpleGce__computeInfallCycleNumber().
//############################################################################*/

double
WnSimpleGce__computeInfallCycleNumber(
  WnSimpleGce *self, double d_time
)
{

  if( self->pfInfallCycleFunction )
    return
      self->pfInfallCycleFunction( d_time, self->pInfallCycleFunctionData );
  else
    return  
      WnSimpleGce__integrator(
        (WnSimpleGce__integrand) self->pfInfallCycleIntegrand,
        0.,
        d_time,
        self->pInfallCycleIntegrandData
      );

}

/*##############################################################################
// WnSimpleGce__integrator().
//############################################################################*/

double
WnSimpleGce__integrator(
  WnSimpleGce__integrand pf_func,
  double d_start_time,
  double d_end_time,
  void *p_data
)
{

  static size_t i_allocs;
  double d_result, d_error;
  gsl_integration_workspace * p_w;
  gsl_function F;

  /*============================================================================
  // Check start and end times.
  //==========================================================================*/

  if( !gsl_fcmp( d_start_time, d_end_time, D_EPS_COMP ) )
    return 0.;

  /*============================================================================
  // Allocate workspace.
  //==========================================================================*/

  p_w = gsl_integration_workspace_alloc( (size_t) I_WORKSPACE );

  /*============================================================================
  // Check number of allocations to prevent runaway.
  //==========================================================================*/

  if( ++i_allocs > 10 ) WN_SIMPLE_GCE__ERROR( "Too many nested integrals" );

  /*============================================================================
  // Set function and data.
  //==========================================================================*/

  F.function = pf_func;
  F.params = p_data;

  gsl_integration_qags(
    &F,
    d_start_time,
    d_end_time,
    0.,
    D_TOLERANCE,
    (size_t) I_MAX_STEPS,
    p_w,
    &d_result,
    &d_error
  );

  gsl_integration_workspace_free( p_w );
  
  i_allocs--;

  return d_result;

} 

/*##############################################################################
// WnSimpleGce__compute_Clayton_integral().
//############################################################################*/

double
WnSimpleGce__compute_Clayton_integral(
  unsigned int i_k,
  double d_Delta,
  double d_time,
  double d_lambda
)
{

  if( d_lambda == 0. )
  {
    return
      ( d_Delta / ( i_k + 1. ) ) *
      ( pow( ( d_time + d_Delta ) / d_Delta, i_k + 1. ) - 1. );
  }

  if( i_k == 0 )
    return ( exp( d_lambda * d_time ) - 1. ) / d_lambda;
  else
    return
      (
        pow(
          ( d_time + d_Delta ) / d_Delta,
          (double) i_k
        ) *
        exp( d_lambda * d_time ) -
        1. -
        i_k *
          WnSimpleGce__compute_Clayton_integral( 
            i_k - 1, d_Delta, d_time, d_lambda
          ) /
          d_Delta
       ) /
       d_lambda;

}
       

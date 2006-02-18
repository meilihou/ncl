#include <stdio.h>
#include "wrapper.h"

extern void NGCALLF(preshybrid,PRESHYBRID)(double*,double*,double*,double*,
                                           int*,double*);

extern void NGCALLF(dpreshybrid,DRESPHYBRID)(double*,double*,double*,double*,
                                             int*,double*);

extern void NGCALLF(dphybrid,DPHYBRID)(double*,double*,double*,double*,
                                       int*,int*,int*,double*);

extern void NGCALLF(ddphybrid,DDPHYBRID)(double*,double*,double*,double*,
                                         int*,int*,int*,double*);

extern void NGCALLF(dh2sdrv,DH2SDRV)(double*,double*,double*,double*,
                                     double*,double*,double*,int*,double*,
                                     int*,int*);

extern void NGCALLF(p2hyo,P2HYO)(double*,int*,int*,int*,double*,double*,
                                 double*,double*,double*,int*,double*);

NhlErrorTypes pres_hybrid_W( void )
{
/*
 * Input variables
 */
  void *psfc, *p0, *hya, *hyb;
  double *tmp_psfc, *tmp_p0, *tmp_hya, *tmp_hyb;
  int ndims_psfc, dsizes_psfc[NCL_MAX_DIMENSIONS];
  int dsizes_hya[NCL_MAX_DIMENSIONS], dsizes_hyb[NCL_MAX_DIMENSIONS];
  NclBasicDataTypes type_p0, type_psfc, type_hya, type_hyb;
/*
 * Output variables
 */
  void *phy;
  double *tmp_phy;
  int ndims_phy, *dsizes_phy;
  NclBasicDataTypes type_phy;
/*
 * Various.
 */
  int i, j, index_phy, klvl, size_leftmost, size_phy;
/*
 * Retrieve parameters
 *
 * Note that any of the pointer parameters can be set to NULL,
 * which implies you don't care about its value.
 */
  psfc = (void*)NclGetArgValue(
          0,
          4,
          &ndims_psfc, 
          dsizes_psfc,
          NULL,
          NULL,
          &type_psfc,
          2);
  p0 = (void*)NclGetArgValue(
          1,
          4,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);
  hya = (void*)NclGetArgValue(
          2,
          4,
          NULL,
          dsizes_hya,
          NULL,
          NULL,
          &type_hya,
          2);
  hyb = (void*)NclGetArgValue(
          3,
          4,
          NULL,
          dsizes_hyb,
          NULL,
          NULL,
          &type_hyb,
          2);
/*
 * Check dimensions.
 */
  klvl = dsizes_hya[0];
  if( dsizes_hyb[0] != klvl) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid: The 'hyb' array must be the same length as 'hya'");
    return(NhlFATAL);
  }
/*
 * Determine type of output.
 */
  if(type_psfc == NCL_double) {
    type_phy = NCL_double;
  }
  else {
    type_phy = NCL_float;
  }
/*
 * Calculate total size of output array.
 */
  if(ndims_psfc == 1 && dsizes_psfc[0] == 1) {
    ndims_phy = 1;
  }
  else {
    ndims_phy = ndims_psfc + 1;
  }

  dsizes_phy = (int*)calloc(ndims_phy,sizeof(int));  
  if( dsizes_phy == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid: Unable to allocate memory for holding dimension sizes");
    return(NhlFATAL);
  }

  size_leftmost = 1;
/*
 * psfc is not a scalar, so phy will be an (ndims_psfc+1)-D array of length 
 * N x klvl, where N represents the dimensions of psfc.
 */
  for( i = 0; i < ndims_psfc; i++ ) {
    size_leftmost *= dsizes_psfc[i];
    dsizes_phy[i] = dsizes_psfc[i];
  }
  size_phy = size_leftmost * klvl;
  dsizes_phy[ndims_phy-1] = klvl;
/*
 * Coerce data to double if necessary.
 */
  tmp_p0  = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_hya = coerce_input_double(hya,type_hya,klvl,0,NULL,NULL);
  tmp_hyb = coerce_input_double(hyb,type_hyb,klvl,0,NULL,NULL);
  if( tmp_p0 == NULL || tmp_hya == NULL || tmp_hyb == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid: Unable to coerce input to double precision");
    return(NhlFATAL);
  }

/*
 * Coerce psfc.
 */
  if(type_psfc != NCL_double) {
    tmp_psfc = (double*)calloc(1,sizeof(double));
    if( tmp_psfc == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid: Unable to allocate memory for coercing psfc array to double precision");
      return(NhlFATAL);
    }
  }
/*
 * Allocate space for output array.
 */
  if(type_phy == NCL_float) {
    phy     = (void*)calloc(size_phy,sizeof(float));
    tmp_phy = (double*)calloc(klvl,sizeof(double));
    if(tmp_phy == NULL || phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    phy = (void*)calloc(size_phy,sizeof(double));
    if(phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }

/*
 * Call function.
 */
  index_phy = 0;
  for( i = 0; i < size_leftmost; i++ ) {
    if(type_psfc != NCL_double) {
/*
 * Coerce subsection of psfc (tmp_psfc) to double.
 */
      coerce_subset_input_double(psfc,tmp_psfc,i,type_psfc,1,0,NULL,NULL);
    }
    else {
/*
 * Point tmp_psfc to appropriate location in psfc.
 */
      tmp_psfc = &((double*)psfc)[i];
    }

    if(type_phy == NCL_double) tmp_phy = &((double*)phy)[index_phy];

    NGCALLF(preshybrid,PRESHYBRID)(tmp_p0,tmp_psfc,tmp_hya,tmp_hyb,&klvl,
                                   tmp_phy);
/*
 * Copy output values from temporary tmp_phy to phy.
 */
    if(type_phy != NCL_double) {
      coerce_output_float_only(phy,tmp_phy,klvl,index_phy);
    }
    index_phy += klvl;
  }
/*
 * Free memory.
 */
  if(type_psfc != NCL_double) NclFree(tmp_psfc);
  if(type_p0   != NCL_double) NclFree(tmp_p0);
  if(type_hya  != NCL_double) NclFree(tmp_hya);
  if(type_hyb  != NCL_double) NclFree(tmp_hyb);
  if(type_phy  != NCL_double) NclFree(tmp_phy);
/*
 * Return.
 */
  return(NclReturnValue(phy,ndims_phy,dsizes_phy,NULL,type_phy,0));
}



NhlErrorTypes dpres_hybrid_W( void )
{
/*
 * Input variables
 */
  void *psfc, *p0, *hya, *hyb;
  double *tmp_psfc, *tmp_p0, *tmp_hya, *tmp_hyb;
  int ndims_psfc, dsizes_psfc[NCL_MAX_DIMENSIONS];
  int dsizes_hya[NCL_MAX_DIMENSIONS], dsizes_hyb[NCL_MAX_DIMENSIONS];
  NclBasicDataTypes type_p0, type_psfc, type_hya, type_hyb;
/*
 * Output variables
 */
  void *phy;
  double *tmp_phy;
  int ndims_phy, *dsizes_phy;
  NclBasicDataTypes type_phy;
/*
 * Various.
 */
  int i, j, index_phy, klvl, klvl1, size_leftmost, size_phy;
/*
 * Retrieve parameters
 *
 * Note that any of the pointer parameters can be set to NULL,
 * which implies you don't care about its value.
 */
  psfc = (void*)NclGetArgValue(
          0,
          4,
          &ndims_psfc, 
          dsizes_psfc,
          NULL,
          NULL,
          &type_psfc,
          2);
  p0 = (void*)NclGetArgValue(
          1,
          4,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);
  hya = (void*)NclGetArgValue(
          2,
          4,
          NULL,
          dsizes_hya,
          NULL,
          NULL,
          &type_hya,
          2);
  hyb = (void*)NclGetArgValue(
          3,
          4,
          NULL,
          dsizes_hyb,
          NULL,
          NULL,
          &type_hyb,
          2);
/*
 * Check dimensions.
 */
  klvl  = dsizes_hya[0];
  klvl1 = klvl-1;
  if( dsizes_hyb[0] != klvl) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid: The 'hyb' array must be the same length as 'hya'");
    return(NhlFATAL);
  }
/*
 * Determine type of output.
 */
  if(type_psfc == NCL_double) {
    type_phy = NCL_double;
  }
  else {
    type_phy = NCL_float;
  }
/*
 * Calculate total size of output array.
 */
  if(ndims_psfc == 1 && dsizes_psfc[0] == 1) {
    ndims_phy = 1;
  }
  else {
    ndims_phy = ndims_psfc + 1;
  }

  dsizes_phy = (int*)calloc(ndims_phy,sizeof(int));  
  if( dsizes_phy == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid: Unable to allocate memory for holding dimension sizes");
    return(NhlFATAL);
  }

  size_leftmost = 1;
/*
 * psfc is not a scalar, so phy will be an (ndims_psfc+1)-D array of length 
 * N x klvl1, where N represents the dimensions of psfc.
 */
  for( i = 0; i < ndims_psfc; i++ ) {
    size_leftmost *= dsizes_psfc[i];
    dsizes_phy[i] = dsizes_psfc[i];
  }
  size_phy = size_leftmost * klvl1;
  dsizes_phy[ndims_phy-1] = klvl1;
/*
 * Coerce data to double if necessary.
 */
  tmp_p0  = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_hya = coerce_input_double(hya,type_hya,klvl,0,NULL,NULL);
  tmp_hyb = coerce_input_double(hyb,type_hyb,klvl,0,NULL,NULL);
  if( tmp_hya == NULL || tmp_hyb == NULL || tmp_p0 == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid: Unable to coerce input to double precision");
    return(NhlFATAL);
  }
/*
 * Coerce psfc.
 */
  if(type_psfc != NCL_double) {
    tmp_psfc = (double*)calloc(1,sizeof(double));
    if( tmp_psfc == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid: Unable to allocate memory for coercing psfc array to double precision");
      return(NhlFATAL);
    }
  }
/*
 * Allocate space for output array.
 */
  if(type_phy == NCL_float) {
    phy     = (void*)calloc(size_phy,sizeof(float));
    tmp_phy = (double*)calloc(klvl1,sizeof(double));
    if(tmp_phy == NULL || phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    phy = (void*)calloc(size_phy,sizeof(double));
    if(phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }

/*
 * Call function.
 */
  index_phy = 0;
  for( i = 0; i < size_leftmost; i++ ) {
    if(type_psfc != NCL_double) {
/*
 * Coerce subsection of psfc (tmp_psfc) to double.
 */
      coerce_subset_input_double(psfc,tmp_psfc,i,type_psfc,1,0,NULL,NULL);
    }
    else {
/*
 * Point tmp_psfc to appropriate location in psfc.
 */
      tmp_psfc = &((double*)psfc)[i];
    }

    if(type_phy == NCL_double) tmp_phy = &((double*)phy)[index_phy];

    NGCALLF(dpreshybrid,DPRESHYBRID)(tmp_p0,tmp_psfc,tmp_hya,tmp_hyb,&klvl,
                                     tmp_phy);
/*
 * Copy output values from temporary tmp_phy to phy.
 */
    if(type_phy != NCL_double) {
      coerce_output_float_only(phy,tmp_phy,klvl1,index_phy);
    }
    index_phy += klvl1;
  }
/*
 * Free memory.
 */
  if(type_psfc != NCL_double) NclFree(tmp_psfc);
  if(type_p0   != NCL_double) NclFree(tmp_p0);
  if(type_hya  != NCL_double) NclFree(tmp_hya);
  if(type_hyb  != NCL_double) NclFree(tmp_hyb);
  if(type_phy  != NCL_double) NclFree(tmp_phy);
/*
 * Return.
 */
  return(NclReturnValue(phy,ndims_phy,dsizes_phy,NULL,type_phy,0));
}

NhlErrorTypes pres_hybrid_ccm_W( void )
{
/*
 * Input variables
 */
  void *psfc, *p0, *hya, *hyb;
  double *tmp_psfc, *tmp_p0, *tmp_hya, *tmp_hyb;
  int ndims_psfc, dsizes_psfc[NCL_MAX_DIMENSIONS];
  int dsizes_hya[NCL_MAX_DIMENSIONS], dsizes_hyb[NCL_MAX_DIMENSIONS];
  NclBasicDataTypes type_p0, type_psfc, type_hya, type_hyb;
/*
 * Output variables
 */
  void *phy;
  double *tmp_phy;
  int ndims_phy, *dsizes_phy;
  NclBasicDataTypes type_phy;
/*
 * Various.
 */
  int i, j, index_psfc, index_phy, nlat, nlon, klvl, nlatnlon, klvlnlatnlon;
  int size_leftmost, size_phy;
/*
 * Retrieve parameters
 *
 * Note that any of the pointer parameters can be set to NULL,
 * which implies you don't care about its value.
 */
  psfc = (void*)NclGetArgValue(
          0,
          4,
          &ndims_psfc, 
          dsizes_psfc,
          NULL,
          NULL,
          &type_psfc,
          2);
  p0 = (void*)NclGetArgValue(
          1,
          4,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);
  hya = (void*)NclGetArgValue(
          2,
          4,
          NULL,
          dsizes_hya,
          NULL,
          NULL,
          &type_hya,
          2);
  hyb = (void*)NclGetArgValue(
          3,
          4,
          NULL,
          dsizes_hyb,
          NULL,
          NULL,
          &type_hyb,
          2);
/*
 * Check dimensions.
 */
  klvl = dsizes_hya[0];
  if( dsizes_hyb[0] != klvl) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: The 'hyb' array must be the same length as 'hya'");
    return(NhlFATAL);
  }
  if(ndims_psfc < 2) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: The 'psfc' array must be at least two-dimensional");
    return(NhlFATAL);
  }
  nlat = dsizes_psfc[ndims_psfc-2];
  nlon = dsizes_psfc[ndims_psfc-1];
  nlatnlon     = nlat * nlon;
  klvlnlatnlon = klvl * nlatnlon;
/*
 * Determine type of output.
 */
  if(type_psfc == NCL_double) {
    type_phy = NCL_double;
  }
  else {
    type_phy = NCL_float;
  }
/*
 * Calculate total size of output array.
 */
  ndims_phy = ndims_psfc + 1;

  dsizes_phy = (int*)calloc(ndims_phy,sizeof(int));  
  if( dsizes_phy == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: Unable to allocate memory for holding dimension sizes");
    return(NhlFATAL);
  }

/*
 * Calculate dimension sizes of phy.
 */
  size_leftmost = 1;
  for( i = 0; i < ndims_psfc-2; i++ ) {
    size_leftmost *= dsizes_psfc[i];
    dsizes_phy[i] = dsizes_psfc[i];
  }
  size_phy = size_leftmost * klvlnlatnlon;
  dsizes_phy[ndims_psfc-2] = klvl;
  dsizes_phy[ndims_psfc-1] = nlat;
  dsizes_phy[ndims_psfc]   = nlon;
/*
 * Coerce data to double if necessary.
 */
  tmp_p0  = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_hya = coerce_input_double(hya,type_hya,klvl,0,NULL,NULL);
  tmp_hyb = coerce_input_double(hyb,type_hyb,klvl,0,NULL,NULL);
  if( tmp_hya == NULL || tmp_hyb == NULL || tmp_p0 == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: Unable to coerce input to double precision");
    return(NhlFATAL);
  }

/*
 * Coerce psfc.
 */
  if(type_psfc != NCL_double) {
    tmp_psfc = (double*)calloc(nlatnlon,sizeof(double));
    if( tmp_psfc == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: Unable to allocate memory for coercing psfc array to double precision");
      return(NhlFATAL);
    }
  }

/*
 * Allocate space for output array.
 */
  if(type_phy == NCL_float) {
    phy     = (void*)calloc(size_phy,sizeof(float));
    tmp_phy = (double*)calloc(klvlnlatnlon,sizeof(double));
    if(tmp_phy == NULL || phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    phy = (void*)calloc(size_phy,sizeof(double));
    if(phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"pres_hybrid_ccm: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }

/*
 * Call function.
 */
  index_psfc = index_phy = 0;
  for( i = 0; i < size_leftmost; i++ ) {
    if(type_psfc != NCL_double) {
/*
 * Coerce subsection of psfc (tmp_psfc) to double.
 */
      coerce_subset_input_double(psfc,tmp_psfc,index_psfc,type_psfc,
                                 nlatnlon,0,NULL,NULL);
    }
    else {
/*
 * Point tmp_psfc to appropriate location in psfc.
 */
      tmp_psfc = &((double*)psfc)[index_psfc];
    }

    if(type_phy == NCL_double) tmp_phy = &((double*)phy)[index_phy];

    NGCALLF(dphybrid,DPHYBRID)(tmp_p0,tmp_hya,tmp_hyb,tmp_psfc,&nlon,&nlat,
                               &klvl,tmp_phy);
/*
 * Copy output values from temporary tmp_phy to phy.
 */
    if(type_phy != NCL_double) {
      coerce_output_float_only(phy,tmp_phy,klvlnlatnlon,index_phy);
    }
    index_psfc += nlatnlon;
    index_phy  += klvlnlatnlon;
  }
/*
 * Free memory.
 */
  if(type_psfc != NCL_double) NclFree(tmp_psfc);
  if(type_p0   != NCL_double) NclFree(tmp_p0);
  if(type_hya  != NCL_double) NclFree(tmp_hya);
  if(type_hyb  != NCL_double) NclFree(tmp_hyb);
  if(type_phy  != NCL_double) NclFree(tmp_phy);
/*
 * Return.
 */
  return(NclReturnValue(phy,ndims_phy,dsizes_phy,NULL,type_phy,0));
}



NhlErrorTypes dpres_hybrid_ccm_W( void )
{
/*
 * Input variables
 */
  void *psfc, *p0, *hya, *hyb;
  double *tmp_psfc, *tmp_p0, *tmp_hya, *tmp_hyb;
  int ndims_psfc, dsizes_psfc[NCL_MAX_DIMENSIONS];
  int dsizes_hya[NCL_MAX_DIMENSIONS], dsizes_hyb[NCL_MAX_DIMENSIONS];
  NclBasicDataTypes type_p0, type_psfc, type_hya, type_hyb;
/*
 * Output variables
 */
  void *phy;
  double *tmp_phy;
  int ndims_phy, *dsizes_phy;
  NclBasicDataTypes type_phy;
/*
 * Various.
 */
  int i, j, nlat, nlon, klvl, klvl1, nlatnlon, klvl1nlatnlon;
  int index_psfc, index_phy, size_leftmost, size_phy;
/*
 * Retrieve parameters
 *
 * Note that any of the pointer parameters can be set to NULL,
 * which implies you don't care about its value.
 */
  psfc = (void*)NclGetArgValue(
          0,
          4,
          &ndims_psfc, 
          dsizes_psfc,
          NULL,
          NULL,
          &type_psfc,
          2);
  p0 = (void*)NclGetArgValue(
          1,
          4,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);
  hya = (void*)NclGetArgValue(
          2,
          4,
          NULL,
          dsizes_hya,
          NULL,
          NULL,
          &type_hya,
          2);
  hyb = (void*)NclGetArgValue(
          3,
          4,
          NULL,
          dsizes_hyb,
          NULL,
          NULL,
          &type_hyb,
          2);
/*
 * Check dimensions.
 */
  klvl = dsizes_hya[0];
  klvl1 = klvl - 1;
  if( dsizes_hyb[0] != klvl) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: The 'hyb' array must be the same length as 'hya'");
    return(NhlFATAL);
  }
  if(ndims_psfc < 2) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: The 'psfc' array must be at least two-dimensional");
    return(NhlFATAL);
  }
  nlat = dsizes_psfc[ndims_psfc-2];
  nlon = dsizes_psfc[ndims_psfc-1];
  nlatnlon      = nlat * nlon;
  klvl1nlatnlon = klvl1 * nlatnlon;
/*
 * Determine type of output.
 */
  if(type_psfc == NCL_double) {
    type_phy = NCL_double;
  }
  else {
    type_phy = NCL_float;
  }
/*
 * Calculate total size of output array.
 */
  ndims_phy = ndims_psfc + 1;

  dsizes_phy = (int*)calloc(ndims_phy,sizeof(int));  
  if( dsizes_phy == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: Unable to allocate memory for holding dimension sizes");
    return(NhlFATAL);
  }

/*
 * Calculate dimension sizes of phy.
 */
  size_leftmost = 1;
  for( i = 0; i < ndims_psfc-2; i++ ) {
    size_leftmost *= dsizes_psfc[i];
    dsizes_phy[i] = dsizes_psfc[i];
  }
  size_phy = size_leftmost * klvl1nlatnlon;
  dsizes_phy[ndims_psfc-2] = klvl1;
  dsizes_phy[ndims_psfc-1] = nlat;
  dsizes_phy[ndims_psfc]   = nlon;
/*
 * Coerce data to double if necessary.
 */
  tmp_p0  = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_hya = coerce_input_double(hya,type_hya,klvl,0,NULL,NULL);
  tmp_hyb = coerce_input_double(hyb,type_hyb,klvl,0,NULL,NULL);
  if( tmp_hya == NULL || tmp_hyb == NULL || tmp_p0 == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: Unable to coerce input to double precision");
    return(NhlFATAL);
  }

/*
 * Coerce psfc.
 */
  if(type_psfc != NCL_double) {
    tmp_psfc = (double*)calloc(nlatnlon,sizeof(double));
    if( tmp_psfc == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: Unable to allocate memory for coercing psfc array to double precision");
      return(NhlFATAL);
    }
  }
/*
 * Allocate space for output array.
 */
  if(type_phy == NCL_float) {
    phy     = (void*)calloc(size_phy,sizeof(float));
    tmp_phy = (double*)calloc(klvl1nlatnlon,sizeof(double));
    if(tmp_phy == NULL || phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    phy = (void*)calloc(size_phy,sizeof(double));
    if(phy == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"dpres_hybrid_ccm: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }

/*
 * Call function.
 */
  index_psfc = index_phy = 0;
  for( i = 0; i < size_leftmost; i++ ) {
    if(type_psfc != NCL_double) {
/*
 * Coerce subsection of psfc (tmp_psfc) to double.
 */
      coerce_subset_input_double(psfc,tmp_psfc,index_psfc,type_psfc,
                                 nlatnlon,0,NULL,NULL);
    }
    else {
/*
 * Point tmp_psfc to appropriate location in psfc.
 */
      tmp_psfc = &((double*)psfc)[index_psfc];
    }

    if(type_phy == NCL_double) tmp_phy = &((double*)phy)[index_phy];

    NGCALLF(ddphybrid,DDPHYBRID)(tmp_p0,tmp_hya,tmp_hyb,tmp_psfc,&nlon,&nlat,
                                 &klvl,tmp_phy);
/*
 * Copy output values from temporary tmp_phy to phy.
 */
    if(type_phy != NCL_double) {
      coerce_output_float_only(phy,tmp_phy,klvl1nlatnlon,index_phy);
    }
    index_phy  += klvl1nlatnlon;
    index_psfc += nlatnlon;
  }
/*
 * Free memory.
 */
  if(type_psfc != NCL_double) NclFree(tmp_psfc);
  if(type_p0   != NCL_double) NclFree(tmp_p0);
  if(type_hya  != NCL_double) NclFree(tmp_hya);
  if(type_hyb  != NCL_double) NclFree(tmp_hyb);
  if(type_phy  != NCL_double) NclFree(tmp_phy);
/*
 * Return.
 */
  return(NclReturnValue(phy,ndims_phy,dsizes_phy,NULL,type_phy,0));
}

NhlErrorTypes sigma2hybrid_W( void )
{
/*
 * Input variables
 */
  void *x, *sigma, *hya, *hyb, *p0, *psfc;
  int *intyp;
  double *tmp_x, *tmp_sigma, *tmp_hya, *tmp_hyb, *tmp_p0, *tmp_psfc;
  int ndims_x, dsizes_x[NCL_MAX_DIMENSIONS];
  int dsizes_sigma[1], dsizes_hya[1], dsizes_hyb[1];
  int ndims_psfc, dsizes_psfc[NCL_MAX_DIMENSIONS];
  NclBasicDataTypes type_x, type_sigma, type_hya, type_hyb;
  NclBasicDataTypes type_p0, type_psfc;
/*
 * Output variables
 */
  void *xhybrid;
  double *tmp_xhybrid;
  int *dsizes_xhybrid;
  NclBasicDataTypes type_xhybrid;
/*
 * Various.
 */
  double *tmp_sigo;
  int i, scalar_psfc, nlvi, nlvo;
  int index_x, index_xhybrid, size_leftmost, size_xhybrid;
/*
 * Retrieve parameters
 *
 * Note that any of the pointer parameters can be set to NULL,
 * which implies you don't care about its value.
 */
  x = (void*)NclGetArgValue(
          0,
          7,
          &ndims_x, 
          dsizes_x,
          NULL,
          NULL,
          &type_x,
          2);
  sigma = (void*)NclGetArgValue(
          1,
          7,
          NULL, 
          dsizes_sigma,
          NULL,
          NULL,
          &type_sigma,
          2);
  hya = (void*)NclGetArgValue(
          2,
          7,
          NULL,
          dsizes_hya,
          NULL,
          NULL,
          &type_hya,
          2);
  hyb = (void*)NclGetArgValue(
          3,
          7,
          NULL,
          dsizes_hyb,
          NULL,
          NULL,
          &type_hyb,
          2);
  p0 = (void*)NclGetArgValue(
          4,
          7,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);
  psfc = (void*)NclGetArgValue(
          5,
          7,
          &ndims_psfc, 
          dsizes_psfc,
          NULL,
          NULL,
          &type_psfc,
          2);
  intyp = (int*)NclGetArgValue(
          6,
          7,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          2);
/*
 * Check dimensions. hya and hyb must be the same size, and the
 * rightmost dimension of x must be the same as the length of sigma.
 */
  nlvi = dsizes_x[ndims_x-1];
  nlvo = dsizes_hya[0];

  if( dsizes_sigma[0] != nlvi ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: The rightmost dimension of 'x' must be the same length as 'sigma'");
    return(NhlFATAL);
  }

  if( dsizes_hyb[0] != nlvo ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: 'hya' and 'hyb' must be the same length");
    return(NhlFATAL);
  }
/*
 * psfc must be the same as the leftmost N-1 dimensions of X. If x is
 * a 1D array, then psfc must be a scalar.
 */
  scalar_psfc = is_scalar(ndims_psfc,dsizes_psfc);
  if( (ndims_x == 1 && !scalar_psfc) ||
      (ndims_x  > 1 && ndims_psfc != (ndims_x-1)) ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: If 'x' is one-dimensional, then 'psfc' must be a scalar; otherwise, the dimensions of 'psfc' must be equal to all but the rightmost dimension of 'x'");
    return(NhlFATAL);
  }
  if(!scalar_psfc) {
    for(i = 0; i <= ndims_psfc-1; i++) {
      if(dsizes_psfc[i] != dsizes_x[i]) {
        NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: If 'x' has more than one dimension, then the dimensions of 'psfc' must be equal to all but the rightmost dimension of 'x'");
        return(NhlFATAL);
      }
    }
  }
/*
 * Determine type of output.
 */
  if(type_x == NCL_double) {
    type_xhybrid = NCL_double;
  }
  else {
    type_xhybrid = NCL_float;
  }
/*
 * Calculate size of output array.
 *
 * xhybrid will be dimensioned N x nlvo, where N represents all but the
 * last dimension of x. 
 */
  dsizes_xhybrid = (int*)calloc(ndims_x,sizeof(int));  
  if( dsizes_xhybrid == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: Unable to allocate memory for holding dimension sizes");
    return(NhlFATAL);
  }

  size_leftmost = 1;
  for( i = 0; i < ndims_x-1; i++ ) {
    size_leftmost *= dsizes_x[i];
    dsizes_xhybrid[i] = dsizes_x[i];
  }
  size_xhybrid = size_leftmost * nlvo;
  dsizes_xhybrid[ndims_x-1] = nlvo;
/*
 * Coerce data to double if necessary.
 */
  tmp_sigma= coerce_input_double(sigma,type_sigma,nlvi,0,NULL,NULL);
  tmp_hya  = coerce_input_double(hya,type_hya,nlvo,0,NULL,NULL);
  tmp_hyb  = coerce_input_double(hyb,type_hyb,nlvo,0,NULL,NULL);
  tmp_p0   = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_psfc = coerce_input_double(psfc,type_psfc,1,0,NULL,NULL);
  if( tmp_hya == NULL || tmp_hyb == NULL || tmp_p0 == NULL ||
      tmp_psfc == NULL) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: Unable to coerce input to double precision");
    return(NhlFATAL);
  }

/*
 * Allocate space for output array.
 */
  if(type_xhybrid == NCL_float) {
    xhybrid     = (void*)calloc(size_xhybrid,sizeof(float));
    tmp_x       = (double*)calloc(nlvi,sizeof(double));
    tmp_sigo    = (double*)calloc(nlvo,sizeof(double));
    tmp_xhybrid = (double*)calloc(nlvo,sizeof(double));
    if(tmp_x == NULL || tmp_xhybrid == NULL || xhybrid == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    tmp_sigo = (double*)calloc(nlvo,sizeof(double));
    xhybrid  = (void*)calloc(size_xhybrid,sizeof(double));
    if(tmp_sigo == NULL || xhybrid == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"sigma2hybrid: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }

/*
 * Loop through leftmost dimensions and call Fortran function.
 */
  index_x = index_xhybrid = 0;
  for( i = 0; i < size_leftmost; i++ ) {
    if(!scalar_psfc) {
      if(type_psfc != NCL_double) {
/*
 * Coerce subsection of psfc (tmp_psfc) to double.
 */
        coerce_subset_input_double(psfc,tmp_psfc,i,type_psfc,
                                   1,0,NULL,NULL);
      }
      else {
/*
 * Point tmp_psfc to appropriate location in psfc.
 */
        tmp_psfc = &((double*)psfc)[i];
      }
    }
    if(type_x != NCL_double) {
/*
 * Coerce subsection of x (tmp_x) to double.
 */
      coerce_subset_input_double(x,tmp_x,index_x,type_x,nlvi,0,NULL,NULL);
    }
    else {
/*
 * Point tmp_x to appropriate location in x.
 */
      tmp_x = &((double*)x)[index_x];
    }
/*
 * Point temporary output array to appropriate location in xhybrid.
 */
    if(type_xhybrid == NCL_double) {
      tmp_xhybrid = &((double*)xhybrid)[index_xhybrid];
    }

    NGCALLF(dh2sdrv,DH2SDRV)(tmp_x,tmp_xhybrid,tmp_hya,tmp_hyb,tmp_p0,
                             tmp_sigma,tmp_sigo,intyp,tmp_psfc,&nlvi,&nlvo);
/*
 * Copy output values from temporary tmp_xhybrid to xhybrid.
 */
    if(type_xhybrid != NCL_double) {
      coerce_output_float_only(xhybrid,tmp_xhybrid,nlvo,index_xhybrid);
    }
    index_xhybrid += nlvo;
    index_x       += nlvi;
  }
/*
 * Free memory.
 */
  if(type_x       != NCL_double) NclFree(tmp_x);
  if(type_sigma   != NCL_double) NclFree(tmp_sigma);
  if(type_hya     != NCL_double) NclFree(tmp_hya);
  if(type_hyb     != NCL_double) NclFree(tmp_hyb);
  if(type_p0      != NCL_double) NclFree(tmp_p0);
  if(type_psfc    != NCL_double) NclFree(tmp_psfc);
  if(type_xhybrid != NCL_double) NclFree(tmp_xhybrid);
/*
 * Return.
 */
  return(NclReturnValue(xhybrid,ndims_x,dsizes_xhybrid,NULL,
                        type_xhybrid,0));
}


NhlErrorTypes p2hy_W( void )
{
/*
 * Input variables
 */
  void *p, *ps, *p0, *xi, *hyao, *hybo;
  int *iflag;
  double *tmp_p, *tmp_ps, *tmp_p0, *tmp_xi, *tmp_hyao, *tmp_hybo;
  int ndims_ps, dsizes_ps[NCL_MAX_DIMENSIONS];
  int ndims_xi, dsizes_xi[NCL_MAX_DIMENSIONS];
  int  dsizes_p[1], dsizes_hyao[1], dsizes_hybo[1];
  NclBasicDataTypes type_p, type_p0, type_ps, type_xi, type_hyao, type_hybo;
/*
 * Output variables
 */
  void *xo;
  double *tmp_xo;
  int *dsizes_xo;
  NclBasicDataTypes type_xo;
/*
 * Various.
 */
  int i, j, index_xi, index_xo, index_ps, size_leftmost, size_xo;
  int nlat, nlon, nlevi, nlevo, nlat_nlon, nlat_nlon_nlevi, nlat_nlon_nlevo;
/*
 * Retrieve parameters
 *
 * Note that any of the pointer parameters can be set to NULL,
 * which implies you don't care about its value.
 */
  p = (void*)NclGetArgValue(
          0,
          7,
          NULL,
          dsizes_p,
          NULL,
          NULL,
          &type_p,
          2);
  ps = (void*)NclGetArgValue(
          1,
          7,
          &ndims_ps, 
          dsizes_ps,
          NULL,
          NULL,
          &type_ps,
          2);
  p0 = (void*)NclGetArgValue(
          2,
          7,
          NULL,
          NULL,
          NULL,
          NULL,
          &type_p0,
          2);
  xi = (void*)NclGetArgValue(
          3,
          7,
          &ndims_xi, 
          dsizes_xi,
          NULL,
          NULL,
          &type_xi,
          2);
  hyao = (void*)NclGetArgValue(
          4,
          7,
          NULL,
          dsizes_hyao,
          NULL,
          NULL,
          &type_hyao,
          2);
  hybo = (void*)NclGetArgValue(
          5,
          7,
          NULL,
          dsizes_hybo,
          NULL,
          NULL,
          &type_hybo,
          2);

  iflag = (int*)NclGetArgValue(
          6,
          7,
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          2);
/*
 * Check # of dimensions.
 */
  if(ndims_ps < 2) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: 'ps' must be at least two dimensions");
    return(NhlFATAL);
  }
  if(ndims_xi < 3) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: 'xi' must be at least three dimensions");
    return(NhlFATAL);
  }
  if(ndims_xi != ndims_ps+1) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: 'xi' must have one more dimension than 'ps'");
    return(NhlFATAL);
  }
/*
 * Check dimension sizes.
 */
  nlevi  = dsizes_p[0];
  nlevo = dsizes_hyao[0];
  nlat  = dsizes_ps[ndims_ps-2];
  nlon  = dsizes_ps[ndims_ps-1];
  nlat_nlon       = nlat*nlon;
  nlat_nlon_nlevi = nlat_nlon*nlevi;
  nlat_nlon_nlevo = nlat_nlon*nlevo;

  if( dsizes_xi[ndims_xi-3] != nlevi || dsizes_xi[ndims_xi-2] != nlat || 
      dsizes_xi[ndims_xi-1] != nlon) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: The three rightmost dimensions of 'xi' must be nlevi x nlat x nlon");
    return(NhlFATAL);
  }
  if( dsizes_hybo[0] != nlevo ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: 'hyao' and 'hybo' must be the same length");
    return(NhlFATAL);
  }
  for(i=0; i <= ndims_ps-3; i++) {
    if(dsizes_ps[i] != dsizes_xi[i]) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: The leftmost n-2 dimensions of 'ps' and n-3 dimensions of 'xi' must be the same");
      return(NhlFATAL);
    }
  }

/*
 * Determine type of output.
 */
  if(type_ps == NCL_double || type_xi == NCL_double) {
    type_xo = NCL_double;
  }
  else {
    type_xo = NCL_float;
  }
/*
 * Calculate total size of output array.
 */
  dsizes_xo = (int*)calloc(ndims_xi,sizeof(int));  
  if( dsizes_xo == NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: Unable to allocate memory for holding dimension sizes");
    return(NhlFATAL);
  }

/*
 * Xo will have the same dimensions as xi, except the level dimension
 * is replaced by the dimension of hyao/hybo
 */
  size_leftmost = 1;
  for( i = 0; i < ndims_xi-3; i++ ) {
    size_leftmost *= dsizes_xi[i];
    dsizes_xo[i] = dsizes_xi[i];
  }
  dsizes_xo[ndims_xi-3] = nlevo;
  dsizes_xo[ndims_xi-2] = nlat;
  dsizes_xo[ndims_xi-1] = nlon;
  size_xo = size_leftmost * nlat_nlon_nlevo;

/*
 * Coerce data to double if necessary.
 */
  tmp_p0   = coerce_input_double(p0,type_p0,1,0,NULL,NULL);
  tmp_p    = coerce_input_double(p,type_p,nlevi,0,NULL,NULL);
  tmp_hyao = coerce_input_double(hyao,type_hyao,nlevo,0,NULL,NULL);
  tmp_hybo = coerce_input_double(hybo,type_hybo,nlevo,0,NULL,NULL);
  if( tmp_p0==NULL || tmp_p==NULL || tmp_hyao==NULL || tmp_hybo==NULL ) {
    NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: Unable to coerce input to double precision");
    return(NhlFATAL);
  }

/*
 * Create temp array for coercing subselections of ps in do loop later.
 */
  if(type_ps != NCL_double) {
    tmp_ps = (double*)calloc(nlat_nlon,sizeof(double));
    if( tmp_ps == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: Unable to allocate memory for coercing ps array to double precision");
      return(NhlFATAL);
    }
  }
/*
 * Create temp array for coercing subselections of xi in do loop later.
 */
  if(type_xi != NCL_double) {
    tmp_xi = (double*)calloc(nlat_nlon_nlevi,sizeof(double));
    if( tmp_xi == NULL ) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: Unable to allocate memory for coercing xi array to double precision");
      return(NhlFATAL);
    }
  }
/*
 * Allocate space for output array.
 */
  if(type_xo == NCL_float) {
    xo     = (void*)calloc(size_xo,sizeof(float));
    tmp_xo = (double*)calloc(nlat_nlon_nlevo,sizeof(double));
    if(tmp_xo == NULL || xo == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }
  else {
    xo = (void*)calloc(size_xo,sizeof(double));
    if(xo == NULL) {
      NhlPError(NhlFATAL,NhlEUNKNOWN,"p2hy: Unable to allocate memory for output array");
      return(NhlFATAL);
    }
  }

/*
 * Call function.
 */
  index_xi = 0;
  index_xo = 0;
  index_ps = 0;
  for( i = 0; i < size_leftmost; i++ ) {
    if(type_ps != NCL_double) {
/*
 * Coerce subsection of ps (tmp_ps) to double.
 */
      coerce_subset_input_double(ps,tmp_ps,index_ps,type_ps,nlat_nlon,
                                 0,NULL,NULL);
    }
    else {
/*
 * Point tmp_ps to appropriate location in ps.
 */
      tmp_ps = &((double*)ps)[index_ps];
    }

    if(type_xi != NCL_double) {
/*
 * Coerce subsection of xi (tmp_xi) to double.
 */
      coerce_subset_input_double(xi,tmp_xi,index_xi,type_xi,nlat_nlon_nlevi,
                                 0,NULL,NULL);
    }
    else {
/*
 * Point tmp_xi to appropriate location in xi.
 */
      tmp_xi = &((double*)xi)[index_xi];
    }

    if(type_xo == NCL_double) tmp_xo = &((double*)xo)[index_xo];

    NGCALLF(p2hyo,P2HYO)(tmp_p,&nlon,&nlat,&nlevi,tmp_xi,tmp_ps,tmp_p0,
                         tmp_hyao,tmp_hybo,&nlevo,tmp_xo);
/*
 * Copy output values from temporary tmp_xo to xo.
 */
    if(type_xo != NCL_double) {
      coerce_output_float_only(xo,tmp_xo,nlat_nlon_nlevo,index_xo);
    }
    index_xi +=nlat_nlon_nlevi;
    index_xo +=nlat_nlon_nlevo;
    index_ps += nlat_nlon;
  }
/*
 * Free memory.
 */
  if(type_p    != NCL_double) NclFree(tmp_p);
  if(type_ps   != NCL_double) NclFree(tmp_ps);
  if(type_p0   != NCL_double) NclFree(tmp_p0);
  if(type_xi   != NCL_double) NclFree(tmp_xi);
  if(type_hyao != NCL_double) NclFree(tmp_hyao);
  if(type_hybo != NCL_double) NclFree(tmp_hybo);
  if(type_xo   != NCL_double) NclFree(tmp_xo);
/*
 * Return.
 */
  return(NclReturnValue(xo,ndims_xi,dsizes_xo,NULL,type_xo,0));
}

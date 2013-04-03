/*! \file npspec.h
 *  \brief The C-interface to the NPSpec library
 */

#ifndef NPSPEC_H
#define NPSPEC_H

/* Pull in the constants */
#include "npspec/constants.h"

/* Protect as C if C++ so that library can be called from C or Fortran */
#ifdef __cplusplus
extern "C" {
#else
/*! \cond */
/* If using library in C, use stdbool.h if available */
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef int bool;
const bool false = 0;
const bool true  = 1;
#endif
/*! \endcond */
#endif

/*! \brief Function to return the index of a material.
 *
 *  \param material This is the material/element name to find the integer
 *                  index of.
 *  \return The integer corresponding to the material.  -1 is returned if
 *          the material is not found.
 */
int material_index(const char *material);

/*! \brief This function is used to calculate the spectra of a nanoparticle.
 *
 *  \param [in]  nlayers The number of layers in the nanoparticle.
 *                       This cannot be greater than 10.
 *  \param [in]  rad An array of length 2 representing the nanoparticle radius
 *                   on the Z axis and the radius on the XY axis. If the XY
 *                   axis component is <0, it is assumed that the particle is
 *                   a sphere and mie theory will be used.  Otherwise the
 *                   quasistatic approzimation is used.
 *  \param [in]  rel_rad An array of length nlayers x 2 which indicates the
 *                       relative radius of each layer in the.  Each component
 *                       (Z axis and XY axis) must sum to 1.0.
 *  \param [in]  indx The integer index of the material/element to calculate.
 *                    It is an array of length nlayers.
 *  \param [in]  mrefrac The refractive index of the surrounding medium.
 *  \param [in]  size_correct Should we size correct the dielectric function?
 *  \param [in]  increment This is the increment to use when looping over the
 *                         wavelengths.  There are 800 wavelengths in increments
 *                         of 1 nm.  This value must be a factor 800.
 *  \param [in]  path_length When calculating absorption, this is the
 *                           Beer's law path length in cm to use.
 *  \param [in]  concentration When calculating absorption, this is the
 *                             Beer's law path concentration in molarity
 *                             to use.
 *  \param [in]  spectra_type The spectra type to calculate.  It is an enum of
 *                            [SpectraType](\ref SpectraType).
 *  \param [out] extinct This is the extinction spectra calculated by the given parameters.
 *  \param [out] scat This is the scattering spectra calculated by the given parameters.
 *  \param [out] absorb This is the absorbance spectra calculated by the given parameters.
 *  \return The error code indicating what went wrong if the
 *          calculation failed.
 */
#ifdef __cplusplus
NPSpec::ErrorCode npspec (const int nlayers,
#else
enum ErrorCode npspec (const int nlayers,
#endif
                  const double rad[2],
                  const double rel_rad[][2],
                  const int indx[],
                  const double mrefrac,
                  const bool size_correct,
                  const int increment,
                  const double path_length,
                  const double concentration,
#ifdef __cplusplus
                  const NPSpec::SpectraType spectra_type,
#else
                  const enum SpectraType spectra_type,
#endif
                  double extinct[],
                  double scat[],
                  double absorb[]
                );

/*! \brief Given a spectra as calculated by npspec,
 *         return the color in RGB color space
 *
 *  \param [in]  spec_in The spectrum that is calculated by [npspec](\ref npspec).
 *  \param [in]  inc The increment used when calculating spec_in.
 *  \param [in]  trans Calculated assuming transmission instead of
 *               absorption (currently not used).
 *  \param [out] r The RED component of RGB color space, between 0 and 1 (inclusive).
 *  \param [out] g The GREEN component of RGB color space, between 0 and 1 (inclusive).
 *  \param [out] b The BLUE component of RGB color space, between 0 and 1 (inclusive).
 */
void RGB(const double spec_in[], 
         const int inc, 
         const bool trans, 
         double *r, 
         double *g,
         double *b);

/*! \brief Convert a color in RGB color space to HSV color space
 *
 *  \param [in]  r The RED component of RGB color space, between 0 and 1 (inclusive).
 *  \param [in]  g The GREEN component of RGB color space, between 0 and 1 (inclusive).
 *  \param [in]  b The BLUE component of RGB color space, between 0 and 1 (inclusive).
 *  \param [out] h The HUE component of HSV color space, between 0 and 360 (inclusive).
 *  \param [out] s The SATURATION component of HSV color space, between 0 and 1 (inclusive).
 *  \param [out] v The VALUE component of HSV color space, between 0 and 1 (inclusive).
 */
void RGB_to_HSV(const double r,
                const double g,
                const double b,
                double *h,
                double *s,
                double *v);

#ifdef __cplusplus
} // extern 
#endif

#endif /* NPSPEC_H */

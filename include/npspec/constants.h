/*! \file constants.h
 *  \brief This file contains the global constants for the NPSpec library.
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "npspec/version.h"

/* Protect as C if C++ so that library can be called from C or Fortran */
#ifdef __cplusplus
namespace NPSpec {
extern "C" {
#endif

/*! Number of wavelengths that will be calculated. */
const int NLAMBDA = 800;

/*! Maximum number of layers the nanoparticle can contain. */
const int MAXLAYERS = 10;

/*! Enum for spectra type. */
enum SpectraType { Efficiency,   /*!< Calculate the efficiency spectra (unitless). */
                   CrossSection, /*!< Calculate the CrossSection spectra (in nm^2). */
                   Molar,        /*!< Calculate the molar absorbtivitty (in L mol^−1 cm^−1). */
                   Absorption    /*!< Calculate the absorption spectra (unitless). */
                 };

/*! Enum for npspec errors. */
enum ErrorCode { NoError = 0, /*!< No error has occured. */
                 SizeWarning = -3, /*!< The nanoparticle size is too large. */
                 InvalidRadius = -4, /*!< The radius given is invalid. */
                 InvalidRelativeRadius = -5, /*!< The relative radius given is invalid. */
                 InvalidIncrement = -6, /*!< The increment is not a factor of 800 or is negative. */
                 InvalidPathLength = -7, /*!< The path length chosen is invalid. */
                 InvalidConcentration = -8, /*!< The concentration given is invalid. */
                 InvalidRefractiveIndex = -9, /*!< The refractive index given is invalid. */
                 InvalidNumberOfLayers = -10, /*!< The number of layers is either negative or greater than 10. */
                 UnknownMaterial = -11 /*!< The material requested is unknown. */
               };

/*! Enum for shape. This is only used in conjunction with the
 * [Nanoparticle](\ref Nanoparticle) class.
 */
enum NanoparticleShape { Sphere,   /*!< The nanoparticle is a sphere. */
                         Ellipsoid /*!< The nanoparticle is an ellipsoid. */
                       };

/*! Enum for spectra property. This is only used in conjunction with the
 * [Nanoparticle](\ref Nanoparticle) class.
 */
enum SpectraProperty { Extinction, /*!< Return the extinction spectra. */
                       Absorbance, /*!< Return the absorbance spectra. */
                       Scattering  /*!< Return the scattering spectra. */
                     };

/*! The wavelengths that will be calcuated over.  This is used internally but
 *  is made public to make plotting the data easy.
 */
extern const double wavelengths[];

#ifdef __cplusplus
} // extern
} // namespace NPSpec
#endif

#endif /* CONSTANTS_H */

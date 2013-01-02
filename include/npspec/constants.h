#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "version.h"

/* Protect as C if C++ so that library can be called from C or Fortran */
#ifdef __cplusplus
namespace NPSpec {
extern "C" {
#endif

/* Number of wavelengths */
const int NLAMBDA = 800;

/* Maximum number of layers */
const int MAXLAYERS = 10;

/* Enum for spectra type */
enum SpectraType { Efficiency, CrossSection, Molar, Absorption };

/* Enum for shape */
enum NanoparticleShape { Sphere, Ellipsoid };

/* Enum for spectra property */
enum SpectraProperty { Extinction, Absorbance, Scattering };

/* Enum for npspec errors */
enum ErrorCode { NoError = 0,
                 NanoparticleTooLarge = -1,
                 SizeParameterWarning = -3,
                 InvalidRadius = -4,
                 InvalidRelativeRadius = -5,
                 InvalidIncrement = -6,
                 InvalidPathLength = -7,
                 InvalidConcentration = -8,
                 InvalidRefractiveIndex = -9,
                 InvalidNumberOfLayers = -10,
                 UnknownMaterial = -11,
                 LayerError = -50,
                 ValueError = -51 };

/* The wavelengths to calculate at as defined in wavelengths.c */
extern const double wavelengths[];

#ifdef __cplusplus
} // extern
} // namespace NPSpec
#endif

#endif /* CONSTANTS_H */

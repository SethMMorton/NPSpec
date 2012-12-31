#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Protect as C if C++ so that library can be called from C or Fortran */
#ifdef __cplusplus
extern "C" {
namespace NPSpec {
#endif

/* Number of wavelengths */
const int NLAMBDA = 800;

/* Maximum number of layers */
const int MAXLAYERS = 10;

/* Enum for spectra type */
enum SpectraType { Efficiency, CrossSection, Molar, Absorbtion };

/* Enum for shape */
enum NanoparticleShape { Sphere, Ellipsoid };

/* Enum for spectra property */
enum SpectraProperty { Extinction, Absorbance, Scattering };

/* The wavelengths to calculate at as defined in wavelengths.c */
extern const double wavelengths[];

#ifdef __cplusplus
} // namespace NPSpec
} // extern
#endif

#endif /* CONSTANTS_H */

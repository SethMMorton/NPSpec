#ifndef CONSTANTS_H
#define CONSTANTS_H

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

#endif /* CONSTANTS_H */

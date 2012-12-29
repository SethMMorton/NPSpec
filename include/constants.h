#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Number of wavelengths */
const int NLAMBDA = 800;

/* Enum for spectra type */
enum SpectraType { Efficiency, CrossSection, Molar, Absorbance };

/* The wavelengths to calculate at as defined in wavelengths.c */
extern const double wavelengths[];

#endif /* CONSTANTS_H */

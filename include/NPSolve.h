#ifndef NPSOLVE_H
#define NPSOLVE_H

#define NLAMBDA 800

/* Protect as C if C++ so that library can be called from C or Fortran */
#ifdef __cplusplus
extern "C" {
#else
/* If using library in C, use stdbool.h if available */
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef int bool;
#endif
#endif

/* Enum for spectra type */
enum { Efficiency, CrossSection, Molar, Absorbance };

/* The wavelengths to calculate at as defined in wavelengths.c */
extern const double wavelengths[];

/* Color matching data found in standard_color_matching.c */
extern const double CIE_Mat[3][3];
extern const double CIE_X[];
extern const double CIE_Y[];
extern const double CIE_Z[];
extern const double CIE_D65[];

/* Function to return the index of a material, from material_index.c */
int material_index(char *material);
void initiallize_material_index(void);

/* The actual npsolve function declaration */
int npsolve (int nlayers,
             double rad[],
             double rel_rad[][2],
             int indx[],
             double mrefrac,
             bool size_correct,
             bool coarse,
             double path_length,
             double concentration,
             int spectra_type,
             double extinct[],
             double scat[],
             double absorb[]
           );

#ifdef __cplusplus
}
#endif

#endif /* NPSOLVE_H */

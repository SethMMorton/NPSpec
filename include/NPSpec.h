#ifndef NPSPEC_H
#define NPSPEC_H

/* Protect as C if C++ so that library can be called from C or Fortran */
#ifdef __cplusplus
namespace NPSpec {
extern "C" {
#else
/* If using library in C, use stdbool.h if available */
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef int bool;
const bool false = 0;
const bool true  = 1;
#endif
#endif

/* Pull in some constants */
#include "constants.h"

/* Function to return the index of a material, from material_index.c */
int material_index(char *material);

/* The actual npsolve function declaration */
int npspec (const int nlayers,
            const double rad[2],
            const double rel_rad[][2],
            const int indx[],
            const double mrefrac,
            const bool size_correct,
            const int increment,
            const double path_length,
            const double concentration,
            const SpectraType spectra_type,
            double extinct[],
            double scat[],
            double absorb[]
          );

/* Determine color based on spectra */
void RGB(const double spec_in[], 
         const int inc, 
         const bool trans, 
         double *r, 
         double *g,
         double *b);

/* Color conversion */
void RGB_to_HSV(const double r,
                const double g,
                const double b,
                double *h,
                double *s,
                double *v);

#ifdef __cplusplus
} // extern 
} // NPSpec namespace
#endif

#endif /* NPSPEC_H */

#ifndef SOLVERS_H
#define SOLVERS_H

#include <complex>

/* Quasistatic approx */
int quasi (const int nlayers,
           const std::complex<double> dielec[],
           const double mdie,
           const double rel_rad[][2],
           const double rad[],
           const double size_param,
           double *extinct,
           double *scat,
           double *absorb
         );

/* Mie theory */
int mie (const int nlayers,
         const std::complex<double> refrac_indx[],
         const double rel_rad[],
         const double size_param,
         double *extinct,
         double *scat,
         double *absorb,
         double *backscat,
         double *rad_pressure,
         double *albedo,
         double *asymmetry
        );

#endif // SOLVERS_H

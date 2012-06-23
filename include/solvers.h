#ifndef SOLVERS_H
#define SOLVERS_H

#include <complex>
using namespace std;

/* Quasistatic approx */
int quasi (int nlayers,
           complex<double> dielec[],
           double mdie,
           double rel_rad[][2],
           double rad[],
           double size_param,
           double *extinct,
           double *scat,
           double *absorb
         );

/* Mie theory */
int mie (int nlayers,
         complex<double> refrac_indx[],
         double rel_rad[],
         double size_param,
         double *extinct,
         double *scat,
         double *absorb,
         double *backscat,
         double *rad_pressure,
         double *albedo,
         double *asymmetry
        );

#endif // SOLVERS_H

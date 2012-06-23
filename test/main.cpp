#include <cstdio>
#include <cmath>
#include <complex>
#include "solvers.h"
#include "NPSolve.h"

using namespace::std;

#define NLAYERS 2
#define WAVELENGTH 500
#define MEDIUMDIE 1.0
#define PI 3.14159265358979323846

// Declare Fortran routines
extern "C" {
void quasif(int *n, complex<double> [], double*, double[], double [], double*, double*, double*, double*);
void mief(int *n, complex<double> [], double [], double*, double*, double*, double*, double*, double*, double*, double*);
}

int main(int argc, char *argv[])
{
    
    // Define the system
    double rel_rad_spheroid[NLAYERS][2] = { { 0.8, 0.8 },
                                            { 0.2, 0.2 }
                                            };
    double rel_rad_spheroid_f[3*NLAYERS]= { 0.8, 0.2, 0.8, 0.2, 0.8, 0.2 };
    double rel_rad_sphere[NLAYERS] = { 0.8, 0.2 };
    double rad[2] = { 10.0, 10.0 };
    double rad_f[3] = { 10.0, 10.0, 10.0 };
    double size_param = 2.0 * PI * rad[0] / WAVELENGTH;

    // Define the dielectric constant, and the refractive index from that
    complex<double> dielec[NLAYERS] = { complex<double>(1.2, 0.5),
                                        complex<double>(0.8, 0.9)
                                      };
    complex<double> refrac_indx[NLAYERS];
    for (int i = 0; i < NLAYERS; i++) {
        double tmp0 = abs(dielec[i]);
        double tmp1 = sqrt(( tmp0 + real(dielec[i]) ) / 2.0);
        double tmp2 = sqrt(( tmp0 - real(dielec[i]) ) / 2.0);
        refrac_indx[i] = complex<double>(tmp1, tmp2);
    }

    // Variables for passing to fortran
    int nlay = NLAYERS;
    double mdie = MEDIUMDIE;

    // The return variables
    double extinct, scat, abs, back, radpress, alb, asym;

    // Test the quasistatic approx
    int res = quasi(NLAYERS, dielec, MEDIUMDIE, rel_rad_spheroid, rad, size_param, &extinct, &scat, &abs);
    if (res == 1) {
        fprintf(stderr, "Too many layers for quasistatic approx\n");
        return 1;
    }
    printf("Quasi C: Ext %.16f, Sca %.16f, Abs %.16f\n", extinct, scat, abs);
    extinct = 0.0; scat = 0.0; abs = 0.0;
    // Fortran version

    quasif(&nlay, dielec, &mdie, rel_rad_spheroid_f, rad_f, &size_param, &extinct, &scat, &abs);
    printf("Quasi F: Ext %.16f, Sca %.16f, Abs %.16f\n", extinct, scat, abs);
    extinct = 0.0; scat = 0.0; abs = 0.0;

    // Test Mie theory
    res = mie(NLAYERS, refrac_indx, rel_rad_sphere, size_param, &extinct, &scat, &abs, &back, &radpress, &alb, &asym);
    if (res == 1) {
        fprintf(stderr, "Product of size parameter and refractive index to large for at least one layer\n");
        return 1;
    }
    printf("Mie C: Ext %.16f, Sca %.16f, Abs %.16f\n", extinct, scat, abs);
    extinct = 0.0; scat = 0.0; abs = 0.0;
    // Fortran version
    mief(&nlay, refrac_indx, rel_rad_sphere, &size_param, &extinct, &scat, &abs, &back, &radpress, &alb, &asym);
    printf("Mie F: Ext %.16f, Sca %.16f, Abs %.16f\n", extinct, scat, abs);

    // Prep the material index
    initiallize_material_index();

    // Main solver test
    int indx[NLAYERS] = { material_index((char*) "Ag"), material_index((char*) "Au") };
    double qext[NLAMBDA], qscat[NLAMBDA], qabs[NLAMBDA];
    res = npsolve(NLAYERS, rad, rel_rad_spheroid, indx, MEDIUMDIE, false, qext, qscat, qabs);
    printf("\n\nTHIS IS VERIFY\n\n");
    for (int i = 0; i < 20; i++) {
        printf("NPSolve: Ext %.16f, Sca %.16f, Abs %.16f\n", qext[i], qscat[i], qabs[i]);
    }

    return 0;
}

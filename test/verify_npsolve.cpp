#include <cstdio>
#include <cmath>
#include <complex>
#include "gtest/gtest.h"
#include "solvers.h"
#include "NPSolve.h"

using namespace::std;

const double wavelength = 500.0;
const double pi = 4.0 * atan(1.0);

/* Verify the C routines against Fortran standards */

// Declare Fortran routines
extern "C" {
void quasifort(int *n, complex<double> [], double*, double[], double [], 
               double*, double*, double*, double*);
void miefort(int *n, complex<double> [], double [], 
             double*, double*, double*, double*, 
             double*, double*, double*, double*);
}

#define NLAYERS 2
TEST(VerifyAgainstFortran, Quasi2Layer) {

    // Variables for passing to fortran
    int nlayers = NLAYERS;
    double mdie = 1.0;
    double rel_rad_spheroid[NLAYERS][2] = { { 0.8, 0.8 },
                                            { 0.2, 0.2 }
                                          };
    double rel_rad_spheroid_f[3*NLAYERS]= { 0.8, 0.2, 0.8, 0.2, 0.8, 0.2 };
    double rel_rad_sphere[NLAYERS] = { 0.8, 0.2 };
    double rad[2] = { 10.0, 10.0 };
    double rad_f[3] = { 10.0, 10.0, 10.0 };
    double size_param = 2.0 * pi * rad[0] / wavelength;

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

    // The return variables
    double cextinct, cscat, cabs;
    double fextinct, fscat, fabs;

    // C++ version
    quasi(nlayers,   dielec, mdie, rel_rad_spheroid, rad, size_param, 
          &cextinct, &cscat, &cabs);
    // Fortran version
    quasifort(&nlayers,  dielec, &mdie, rel_rad_spheroid_f, rad_f, &size_param, 
              &fextinct, &fscat, &fabs);

    // Make sure results are correct
    EXPECT_DOUBLE_EQ(cextinct, fextinct);
    EXPECT_DOUBLE_EQ(cscat,    fscat);
    EXPECT_DOUBLE_EQ(cabs,     fabs);

}

// Run the tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

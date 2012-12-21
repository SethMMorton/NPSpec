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

class VerifyAgainstFortran : public ::testing::Test {
 protected:

    complex<double> make_refrac_index(complex<double> dielec) {
        double tmp0 = abs(dielec);
        double tmp1 = sqrt(( tmp0 + real(dielec) ) / 2.0);
        double tmp2 = sqrt(( tmp0 - real(dielec) ) / 2.0);
        return complex<double>(tmp1, tmp2);
    }

    virtual void SetUp() { 
        rel_rad_spheroid1[0][0] = 1.0; rel_rad_spheroid1[0][1] = 1.0;
        rel_rad_spheroid2[0][0] = 0.8; rel_rad_spheroid2[0][1] = 0.8;
        rel_rad_spheroid2[1][0] = 0.2; rel_rad_spheroid2[1][1] = 0.2;
        rel_rad_spheroid3[0][0] = 0.3; rel_rad_spheroid3[0][1] = 0.3;
        rel_rad_spheroid3[1][0] = 0.2; rel_rad_spheroid3[1][1] = 0.2;
        rel_rad_spheroid3[2][0] = 0.3; rel_rad_spheroid3[2][1] = 0.3;
        rel_rad_spheroid_f1[0]= 1.0;
        rel_rad_spheroid_f1[1]= 1.0;
        rel_rad_spheroid_f1[2]= 1.0;
        rel_rad_spheroid_f2[0]= 0.8; rel_rad_spheroid_f2[1]= 0.2;
        rel_rad_spheroid_f2[2]= 0.8; rel_rad_spheroid_f2[3]= 0.2;
        rel_rad_spheroid_f2[4]= 0.8; rel_rad_spheroid_f2[5]= 0.2;
        rel_rad_spheroid_f3[0]= 0.3; rel_rad_spheroid_f3[1]= 0.2; rel_rad_spheroid_f3[2]= 0.3; 
        rel_rad_spheroid_f3[3]= 0.3; rel_rad_spheroid_f3[4]= 0.2; rel_rad_spheroid_f3[5]= 0.3;
        rel_rad_spheroid_f3[6]= 0.3; rel_rad_spheroid_f3[7]= 0.2; rel_rad_spheroid_f3[8]= 0.3; 
        rel_rad_sphere1[0] = 1.0;
        rel_rad_sphere2[0] = 0.8; rel_rad_sphere2[1] = 0.2;
        rel_rad_sphere3[0] = 0.3; rel_rad_sphere3[1] = 0.2; rel_rad_sphere3[2] = 0.3;
        rad[0] = 10.0; rad[1] = 10.0;
        rad_f[0] = 10.0; rad_f[1] = 10.0; rad_f[2] = 10.0;
        // These dielectric constants may not be physical, but we are comparing
        // numerics, not comparing to experiment
        dielec1[0] = complex<double>(1.2, 0.5);
        dielec2[0] = complex<double>(1.2, 0.5);
        dielec2[1] = complex<double>(0.8, 0.9);
        dielec3[0] = complex<double>(1.2, 0.5);
        dielec3[1] = complex<double>(0.8, 0.9);
        dielec3[2] = complex<double>(1.5, 0.2);
        refrac1[0] = make_refrac_index(dielec1[0]);
        for (int i = 0; i < 2; i++)
            refrac2[i] = make_refrac_index(dielec2[i]);
        for (int i = 0; i < 3; i++)
            refrac3[i] = make_refrac_index(dielec3[i]);
    }

    double rel_rad_spheroid1[1][2],  rel_rad_spheroid2[2][2],  rel_rad_spheroid3[3][2];
    double rel_rad_spheroid_f1[3], rel_rad_spheroid_f2[6], rel_rad_spheroid_f3[9];
    double rel_rad_sphere1[1], rel_rad_sphere2[2], rel_rad_sphere3[3];
    double rad[2];
    double rad_f[3];
    complex<double> dielec1[1], dielec2[2], dielec3[3];
    complex<double> refrac1[1], refrac2[2], refrac3[3];
    double cextinct, cscat, cabs;
    double fextinct, fscat, fabs;
    double back, radpres, alb, asym;
};

TEST_F(VerifyAgainstFortran, Mie1Layer) {

    // Variables
    int nlayers = 1;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    mie(nlayers,   refrac1, rel_rad_sphere1, size_param, 
        &cextinct, &cscat,  &cabs, &back, &radpres, &alb, &asym);
    // Fortran version
    miefort(&nlayers,  refrac1, rel_rad_sphere1, &size_param, 
            &fextinct, &fscat,  &fabs, &back, &radpres, &alb, &asym);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, Mie2Layer) {

    // Variables
    int nlayers = 2;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    mie(nlayers,   refrac2, rel_rad_sphere2, size_param, 
        &cextinct, &cscat,  &cabs, &back, &radpres, &alb, &asym);
    // Fortran version
    miefort(&nlayers,  refrac2, rel_rad_sphere2, &size_param, 
            &fextinct, &fscat,  &fabs, &back, &radpres, &alb, &asym);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, Mie3Layer) {

    // Variables
    int nlayers = 3;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    mie(nlayers,   refrac3, rel_rad_sphere3, size_param, 
        &cextinct, &cscat,  &cabs, &back, &radpres, &alb, &asym);
    // Fortran version
    miefort(&nlayers,  refrac3, rel_rad_sphere3, &size_param, 
            &fextinct, &fscat,  &fabs, &back, &radpres, &alb, &asym);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, MieMediumDielectric) {

    // Variables
    int nlayers = 1;
    double mdie = 2.5;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    mie(nlayers,   refrac1, rel_rad_sphere1, size_param, 
        &cextinct, &cscat,  &cabs, &back, &radpres, &alb, &asym);
    // Fortran version
    miefort(&nlayers,  refrac1, rel_rad_sphere1, &size_param, 
            &fextinct, &fscat,  &fabs, &back, &radpres, &alb, &asym);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, Quasi1LayerSphere) {

    // Variables
    int nlayers = 1;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    quasi(nlayers,   dielec1, mdie, rel_rad_spheroid1, rad, size_param, 
          &cextinct, &cscat,  &cabs);
    // Fortran version
    quasifort(&nlayers,  dielec1, &mdie, rel_rad_spheroid_f1, rad_f, &size_param, 
              &fextinct, &fscat,  &fabs);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, Quasi2LayerSphere) {

    // Variables
    int nlayers = 2;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    quasi(nlayers,   dielec2, mdie, rel_rad_spheroid2, rad, size_param, 
          &cextinct, &cscat,  &cabs);
    // Fortran version
    quasifort(&nlayers,  dielec2, &mdie, rel_rad_spheroid_f2, rad_f, &size_param, 
              &fextinct, &fscat,  &fabs);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, QuasiProlate) {

    // Variables
    int nlayers = 1;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;
    rad[1] = 5.0;
    rad_f[1] = 5.0; rad_f[2] = 5.0;

    // C++ version
    quasi(nlayers,   dielec1, mdie, rel_rad_spheroid1, rad, size_param, 
          &cextinct, &cscat,  &cabs);
    // Fortran version
    quasifort(&nlayers,  dielec1, &mdie, rel_rad_spheroid_f1, rad_f, &size_param, 
              &fextinct, &fscat,  &fabs);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, QuasiOblate) {

    // Variables
    int nlayers = 1;
    double mdie = 1.0;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;
    rad[1] = 15.0;
    rad_f[1] = 15.0; rad_f[2] = 15.0;

    // C++ version
    quasi(nlayers,   dielec1, mdie, rel_rad_spheroid1, rad, size_param, 
          &cextinct, &cscat,  &cabs);
    // Fortran version
    quasifort(&nlayers,  dielec1, &mdie, rel_rad_spheroid_f1, rad_f, &size_param, 
              &fextinct, &fscat,  &fabs);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

TEST_F(VerifyAgainstFortran, QuasiMediumDielectric) {

    // Variables
    int nlayers = 1;
    double mdie = 2.5;
    double size_param = 2.0 * pi * rad[0] * mdie / wavelength;

    // C++ version
    quasi(nlayers,   dielec1, mdie, rel_rad_spheroid1, rad, size_param, 
          &cextinct, &cscat,  &cabs);
    // Fortran version
    quasifort(&nlayers,  dielec1, &mdie, rel_rad_spheroid_f1, rad_f, &size_param, 
              &fextinct, &fscat,  &fabs);

    // Make sure results are correct
    EXPECT_NEAR(cextinct, fextinct, 1e-14);
    EXPECT_NEAR(cscat,    fscat,    1e-14);
    EXPECT_NEAR(cabs,     fabs,     1e-14);

}

// Run the tests
int main(int argc, char **argv) {
    initiallize_material_index();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

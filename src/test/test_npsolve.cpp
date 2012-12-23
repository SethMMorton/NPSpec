#include "gtest/gtest.h"
#include "NPSolve.h"

class TestSolver : public ::testing::Test {
 protected:

    virtual void SetUp() { 
        // Initiallize the variables
        index1[0] = material_index((char*) "Ag");
        index2[0] = material_index((char*) "Ag");
        index2[1] = material_index((char*) "Quartz");
        index3[0] = material_index((char*) "Ag");
        index3[1] = material_index((char*) "Quartz");
        index3[2] = material_index((char*) "TiO2");
        // Relative radii
        relative_radius_spheroid1[0][0] = 1.0;
        relative_radius_spheroid1[0][1] = 1.0;
        relative_radius_spheroid2[0][0] = 0.6;
        relative_radius_spheroid2[0][1] = 0.6;
        relative_radius_spheroid2[1][0] = 0.4;
        relative_radius_spheroid2[1][1] = 0.4;
        relative_radius_spheroid3[0][0] = 0.2;
        relative_radius_spheroid3[0][1] = 0.2;
        relative_radius_spheroid3[1][0] = 0.3;
        relative_radius_spheroid3[1][1] = 0.3;
        relative_radius_spheroid3[2][0] = 0.5;
        relative_radius_spheroid3[2][1] = 0.5;
    }

    int index1[1], index2[2], index3[3];
    double relative_radius_spheroid1[1][2];
    double relative_radius_spheroid2[2][2];
    double relative_radius_spheroid3[3][2];
    double qext[NLAMBDA], qabs[NLAMBDA], qscat[NLAMBDA];

};

class TestSpectraTypes : public ::testing::Test {
 protected:

    virtual void SetUp() { 
        // Initiallize the variables
        pi        = 4.0 * atan(1.0);
        avogadro  = 6.0221412927e23;
        index[0] = material_index((char*) "Ag");
        relative_radius[0][0] = 1.0;
        relative_radius[0][1] = 1.0;
        nlayers = 1;
        inc = 80;
        medium_refrac = 1.0;
        radius[0] = 10.0;
        radius[1] = -1.0;
        npsolve(nlayers, radius, relative_radius, index,
                medium_refrac, false, inc, 1.0, 1.0, Efficiency,
                qext1, qscat1, qabs1);
    }

    inline double sqr(double x) { return x*x; };

    double pi;
    double avogadro;
    int nlayers;
    int inc;
    int index[1];
    double medium_refrac;
    double radius[2];
    double relative_radius[1][2];
    double qext1[NLAMBDA], qabs1[NLAMBDA], qscat1[NLAMBDA];
    double qext2[NLAMBDA], qabs2[NLAMBDA], qscat2[NLAMBDA];

};

// Check that the number of wavelengths is 800
TEST(SanityTest, NLAMBDA_Check) {
    EXPECT_EQ(NLAMBDA, 800);
}

// Check that the range of the wavelenths array is correct
TEST(SanityTest, Wavelengths) {
    EXPECT_EQ(wavelengths[0], 200.0);
    EXPECT_EQ(wavelengths[1], 201.0);
    EXPECT_EQ(wavelengths[10], 210.0);
    EXPECT_EQ(wavelengths[500], 700.0);
    EXPECT_EQ(wavelengths[799], 999.0);
}

// Check that the CIE matrix is correct
TEST(SanityTest, CIE_MAT) {
    EXPECT_EQ(CIE_Mat[0][0],  3.2410);
    EXPECT_EQ(CIE_Mat[0][1], -1.5374);
    EXPECT_EQ(CIE_Mat[0][2], -0.4986);
    EXPECT_EQ(CIE_Mat[1][0], -0.9692);
    EXPECT_EQ(CIE_Mat[1][1],  1.8760);
    EXPECT_EQ(CIE_Mat[1][2],  0.0416);
    EXPECT_EQ(CIE_Mat[2][0],  0.0556);
    EXPECT_EQ(CIE_Mat[2][1], -0.2040);
    EXPECT_EQ(CIE_Mat[2][2],  1.0570);
};

// Test that the material index works
TEST(SanityTest, MatIndx) {
    // Get the correct material indices
    // Remember that initiallize_material_index has already been called
    EXPECT_EQ(material_index((char*) "Au"), 4);
    EXPECT_EQ(material_index((char*) "Diamond"), 13);
    EXPECT_EQ(material_index((char*) "TiO2"), 43);
    // Make sure that unknowns return -1
    EXPECT_EQ(material_index((char*) "Kryptonite"), -1);
};

TEST_F(TestSolver, Mie1Layer) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   2.2285778886683643, 1e-14);
    EXPECT_NEAR(qext[250], 0.2386953127709600, 1e-14);
    EXPECT_NEAR(qext[500], 0.0133494242710007, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.3062750316703077, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0572390734717247, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0039184205430821, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   1.9223028569980565, 1e-14);
    EXPECT_NEAR(qabs[250], 0.1814562392992353, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0094310037279186, 1e-14);
};

TEST_F(TestSolver, Mie2Layer) {
    const int nlayers = 2;
    const double medium_refrac = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid2, index2,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   0.5373534948346590, 1e-14);
    EXPECT_NEAR(qext[250], 0.3143612207865766, 1e-14);
    EXPECT_NEAR(qext[500], 0.0058357432042841, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.0482912065377480, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0250656892007021, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0010568992342033, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   0.4890622882969110, 1e-14);
    EXPECT_NEAR(qabs[250], 0.2892955315858746, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0047788439700809, 1e-14);

};

TEST_F(TestSolver, Mie3Layer) {
    const int nlayers = 3;
    const double medium_refrac = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid3, index3,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   2.0261050243604539, 1e-14);
    EXPECT_NEAR(qext[250], 0.0248674748630347, 1e-14);
    EXPECT_NEAR(qext[500], 0.0012280674844302, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.3388504569262775, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0081054247786561, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0011010132200970, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   1.6872545674341763, 1e-14);
    EXPECT_NEAR(qabs[250], 0.0167620500843786, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0001270542643331, 1e-14);

};

TEST_F(TestSolver, Quasi1Layer) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   1.0611537055667197, 1e-14);
    EXPECT_NEAR(qext[250], 0.0764461773630003, 1e-14);
    EXPECT_NEAR(qext[500], 0.0042801575796004, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.0241764183390374, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0029871788218089, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0002336823836763, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   1.0369772872276823, 1e-14);
    EXPECT_NEAR(qabs[250], 0.0734589985411914, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0040464751959240, 1e-14);

};

TEST_F(TestSolver, Quasi2Layer) {
    const int nlayers = 2;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid2, index2,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   0.2170155342365016, 1e-14);
    EXPECT_NEAR(qext[250], 0.1117949449870051, 1e-14);
    EXPECT_NEAR(qext[500], 0.0022927389984260, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.0033678648313046, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0012481734552626, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0000636722865861, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   0.2136476694051971, 1e-14);
    EXPECT_NEAR(qabs[250], 0.1105467715317425, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0022290667118399, 1e-14);

};
    
TEST_F(TestSolver, Quasi3Layer) {
    const int nlayers = 3;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid3, index3,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // This should fail - 2 means too many layers for quasi
    EXPECT_EQ(result, 2);

};

TEST_F(TestSolver, QuasiProlate) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 5.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   0.6549347175126272, 1e-14);
    EXPECT_NEAR(qext[250], 0.0713320530670000, 1e-14);
    EXPECT_NEAR(qext[500], 0.0030711419580141, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.0036651236274123, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0006166697027189, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0000411265232696, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   0.6512695938852150, 1e-14);
    EXPECT_NEAR(qabs[250], 0.0707153833642811, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0030300154347445, 1e-14);

};

TEST_F(TestSolver, QuasiOblate) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 5.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   0.8371294510751321, 1e-14);
    EXPECT_NEAR(qext[250], 0.0785298784021552, 1e-14);
    EXPECT_NEAR(qext[500], 0.0037899923685295, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.0093404960245316, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0014531069310511, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0001022886141183, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   0.8277889550506005, 1e-14);
    EXPECT_NEAR(qabs[250], 0.0770767714711041, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0036877037544112, 1e-14);

};

TEST_F(TestSolver, TestIncrement5) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 5, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0], 1.0981095009299680, 1e-14);
    EXPECT_EQ(qext[1],   0.0);
    EXPECT_EQ(qext[2],   0.0);
    EXPECT_EQ(qext[3],   0.0);
    EXPECT_EQ(qext[4],   0.0);
    EXPECT_NEAR(qext[5], 1.0405331345505673, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0], 0.0241147084928534, 1e-14);
    EXPECT_EQ(qscat[1],   0.0);
    EXPECT_EQ(qscat[2],   0.0);
    EXPECT_EQ(qscat[3],   0.0);
    EXPECT_EQ(qscat[4],   0.0);
    EXPECT_NEAR(qscat[5], 0.0218343064818317, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0], 1.0739947924371145, 1e-14);
    EXPECT_EQ(qabs[1],   0.0);
    EXPECT_EQ(qabs[2],   0.0);
    EXPECT_EQ(qabs[3],   0.0);
    EXPECT_EQ(qabs[4],   0.0);
    EXPECT_NEAR(qabs[5], 1.0186988280687357, 1e-14);

};

TEST_F(TestSolver, TestIncrement7) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 7, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Bad increment should return 3
    EXPECT_EQ(result, 3);

};

TEST_F(TestSolver, TestIncrementNegative) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, -1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Bad increment should return 3
    EXPECT_EQ(result, 3);

};

TEST_F(TestSolver, TestSizeCorrect) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 5.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, true, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   0.5334378505066342, 1e-14);
    EXPECT_NEAR(qext[250], 0.0025491434133546, 1e-14);
    EXPECT_NEAR(qext[500], 0.0020526111029140, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.0015165842885118, 1e-14);
    EXPECT_NEAR(qscat[250], 0.0001902864485487, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0000145576138027, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   0.5319212662181224, 1e-14);
    EXPECT_NEAR(qabs[250], 0.0023588569648059, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0020380534891113, 1e-14);

};

TEST_F(TestSolver, TestMediumRefractiveIndex) {
    const int nlayers = 1;
    const double medium_refrac = 2.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   2.8328562530696506, 1e-14);
    EXPECT_NEAR(qext[250], 2.0565291634339800, 1e-14);
    EXPECT_NEAR(qext[500], 0.0970864882507346, 1e-14);
    // Scattering
    EXPECT_NEAR(qscat[0],   1.0698792522825473, 1e-14);
    EXPECT_NEAR(qscat[250], 1.4426586799505641, 1e-14);
    EXPECT_NEAR(qscat[500], 0.0718406948561757, 1e-14);
    // Absorption
    EXPECT_NEAR(qabs[0],   1.7629770007871033, 1e-14);
    EXPECT_NEAR(qabs[250], 0.6138704834834159, 1e-14);
    EXPECT_NEAR(qabs[500], 0.0252457933945589, 1e-14);

};

TEST_F(TestSolver, TestMieQuasiSmall) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 3.0, -1.0 };
    /* Mie */
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);
    EXPECT_EQ(result, 0);
    const double radius2[2] = { 3.0, 3.0 };
    double qext2[NLAMBDA], qscat2[NLAMBDA], qabs2[NLAMBDA];
    /* Quasistatic */
    result = npsolve(nlayers, radius2, relative_radius_spheroid1, index1,
                     medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                     qext2, qscat2, qabs2);
    EXPECT_EQ(result, 0);

    // Extinction
    EXPECT_NEAR(qext[0],   qext2[0],   2e-3);
    EXPECT_NEAR(qext[250], qext2[250], 2e-3);
    EXPECT_NEAR(qext[500], qext2[500], 2e-3);
    // Scattering
    EXPECT_NEAR(qscat[0],   qscat2[0],   2e-3);
    EXPECT_NEAR(qscat[250], qscat2[250], 2e-3);
    EXPECT_NEAR(qscat[500], qscat2[500], 2e-3);
    // Absorption
    EXPECT_NEAR(qabs[0],   qabs2[0],   2e-3);
    EXPECT_NEAR(qabs[250], qabs2[250], 2e-3);
    EXPECT_NEAR(qabs[500], qabs2[500], 2e-3);

};

TEST_F(TestSpectraTypes, TestCrossSection) {
    int result = npsolve(nlayers, radius, relative_radius, index, 
                         medium_refrac, false, inc, 1.0, 1.0, CrossSection,
                         qext2, qscat2, qabs2);
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_FLOAT_EQ(qext1[0]   * pi * sqr(radius[0]), qext2[0]);
    EXPECT_FLOAT_EQ(qext1[160] * pi * sqr(radius[0]), qext2[160]);
    EXPECT_FLOAT_EQ(qext1[640] * pi * sqr(radius[0]), qext2[640]);
    // Scattering
    EXPECT_FLOAT_EQ(qscat1[0]   * pi * sqr(radius[0]), qscat2[0]);
    EXPECT_FLOAT_EQ(qscat1[160] * pi * sqr(radius[0]), qscat2[160]);
    EXPECT_FLOAT_EQ(qscat1[640] * pi * sqr(radius[0]), qscat2[640]);
    // Absorbance
    EXPECT_FLOAT_EQ(qabs1[0]   * pi * sqr(radius[0]), qabs2[0]);
    EXPECT_FLOAT_EQ(qabs1[160] * pi * sqr(radius[0]), qabs2[160]);
    EXPECT_FLOAT_EQ(qabs1[640] * pi * sqr(radius[0]), qabs2[640]);
};

TEST_F(TestSpectraTypes, TestMolar) {
    int result = npsolve(nlayers, radius, relative_radius, index, 
                         medium_refrac, false, inc, 1.0, 1.0, Molar,
                         qext2, qscat2, qabs2);
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_FLOAT_EQ(qext1[0]   * pi * sqr(radius[0]) 
                               * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qext2[0]);
    EXPECT_FLOAT_EQ(qext1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qext2[160]);
    EXPECT_FLOAT_EQ(qext1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qext2[640]);
    // Scattering
    EXPECT_FLOAT_EQ(qscat1[0]   * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qscat2[0]);
    EXPECT_FLOAT_EQ(qscat1[160] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qscat2[160]);
    EXPECT_FLOAT_EQ(qscat1[640] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qscat2[640]);
    // Absorbance
    EXPECT_FLOAT_EQ(qabs1[0]   * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qabs2[0]);
    EXPECT_FLOAT_EQ(qabs1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qabs2[160]);
    EXPECT_FLOAT_EQ(qabs1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ),
                    qabs2[640]);
};

TEST_F(TestSpectraTypes, TestAbsorbance) {
    const double path_length = 1.5, molarity = 0.004;
    int result = npsolve(nlayers, radius, relative_radius, index, 
                         medium_refrac, false, inc, path_length, molarity,
                         Absorbance, qext2, qscat2, qabs2);
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_FLOAT_EQ(qext1[0]   * pi * sqr(radius[0]) 
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qext2[0]);
    EXPECT_FLOAT_EQ(qext1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qext2[160]);
    EXPECT_FLOAT_EQ(qext1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qext2[640]);
    // Scattering
    EXPECT_FLOAT_EQ(qscat1[0]   * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qscat2[0]);
    EXPECT_FLOAT_EQ(qscat1[160] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qscat2[160]);
    EXPECT_FLOAT_EQ(qscat1[640] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qscat2[640]);
    // Absorbance
    EXPECT_FLOAT_EQ(qabs1[0]   * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qabs2[0]);
    EXPECT_FLOAT_EQ(qabs1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qabs2[160]);
    EXPECT_FLOAT_EQ(qabs1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity,
                    qabs2[640]);
};

// Run the tests
int main(int argc, char **argv) {
    initiallize_material_index();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

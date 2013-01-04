#include "gtest/gtest.h"
#include "npspec/npspec.h"

using namespace NPSpec;

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
        npspec(nlayers, radius, relative_radius, index,
                medium_refrac, false, inc, 1.0, 1.0, Efficiency,
                qext1, qscat1, qabs1);
    }

    inline double sqr(double x) { return x*x; }

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

class TestColors : public ::testing::Test {
protected:

    virtual void SetUp() {
        for (int i = 0; i < NLAMBDA; i++) 
            spec[i] = 0.0;
        radius[0] = 20.0;
        radius[1] = -1.0;
        relative_radius[0][0] = 1.0;
        relative_radius[0][1] = 1.0;
        index[0] = material_index((char*) "Ag");
        npspec(1, radius, relative_radius, index,
                1.0, false, 1, 1.0, 1.0, Efficiency,
                qext, qscat, qabs);
    }

    int index[1];
    double radius[2];
    double relative_radius[1][2];
    double qext[NLAMBDA], qabs[NLAMBDA], qscat[NLAMBDA];
    double spec[NLAMBDA];
    double r, g, b, o, h, s, v;

};

// Check that the number of wavelengths is 800
TEST(SanityTest, NLAMBDA_Check) {
    EXPECT_EQ(800, NLAMBDA);
}

// Check that the range of the wavelenths array is correct
TEST(SanityTest, Wavelengths) {
    EXPECT_EQ(200.0, wavelengths[0]);
    EXPECT_EQ(201.0, wavelengths[1]);
    EXPECT_EQ(210.0, wavelengths[10]);
    EXPECT_EQ(700.0, wavelengths[500]);
    EXPECT_EQ(999.0, wavelengths[799]);
}

// Test that the material index works
TEST(SanityTest, MatIndx) {
    // Get the correct material indices
    EXPECT_EQ(4,  material_index((char*) "Au"));
    EXPECT_EQ(13, material_index((char*) "Diamond"));
    EXPECT_EQ(43, material_index((char*) "TiO2"));
    // Make sure that unknowns return -1
    EXPECT_EQ(UnknownMaterial, material_index((char*) "Kryptonite"));
}

// Make sure that the error checking is active
TEST(SanityTest, ErrorCodes) {
    double qext[NLAMBDA], qabs[NLAMBDA], qscat[NLAMBDA];
    double radius[2];
    double relative_radius[MAXLAYERS][2];
    int matIndx[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    ErrorCode result;
    radius[0] = 10.0;
    radius[1] = -1.0;
    relative_radius[0][0] = 1.0;
    relative_radius[0][1] = 1.0;
    for (int i = 1; i < MAXLAYERS; i++) {
        relative_radius[i][0] = 0.0;
        relative_radius[i][1] = 0.0;
    }
    result = npspec(1, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 0.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidConcentration, result);
    result = npspec(1, radius, relative_radius, matIndx,
                    1.0, false, 1, 0.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidPathLength, result);
    result = npspec(1, radius, relative_radius, matIndx,
                    0.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidRefractiveIndex, result);
    result = npspec(0, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidNumberOfLayers, result);
    result = npspec(11, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidNumberOfLayers, result);
    radius[0] = 0.0;
    result = npspec(1, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidRadius, result);
    radius[0] = 500.0;
    result = npspec(1, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(SizeWarning, result);
    radius[0] = 20.0;
    radius[1] = 20.0;
    result = npspec(1, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(SizeWarning, result);
    radius[0] = 10.0;
    radius[1] = 10.0;
    relative_radius[2][0] = -0.4;
    result = npspec(3, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidRelativeRadius, result);
    relative_radius[2][0] = 0.5; // Sum greater than 1
    result = npspec(3, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidRelativeRadius, result);
    relative_radius[0][0] = 0.4; // Sum less than 1
    result = npspec(3, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidRelativeRadius, result);
    relative_radius[0][0] = 0.5; // Check that sum to 1 is OK
    result = npspec(3, radius, relative_radius, matIndx,
                    1.0, false, 1, 1.0, 1.0, Efficiency,
                    qext, qscat, qabs);
    EXPECT_EQ(InvalidNumberOfLayers, result); // 3 layers too
                                              // many for quasistatic
}

TEST_F(TestSolver, Mie1Layer) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(2.2285778886683643, qext[0],   1e-14);
    EXPECT_NEAR(0.2386953127709600, qext[250], 1e-14);
    EXPECT_NEAR(0.0133494242710007, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.3062750316703077, qscat[0],   1e-14);
    EXPECT_NEAR(0.0572390734717247, qscat[250], 1e-14);
    EXPECT_NEAR(0.0039184205430821, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(1.9223028569980565, qabs[0],   1e-14);
    EXPECT_NEAR(0.1814562392992353, qabs[250], 1e-14);
    EXPECT_NEAR(0.0094310037279186, qabs[500], 1e-14);
}

TEST_F(TestSolver, Mie2Layer) {
    const int nlayers = 2;
    const double medium_refrac = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid2, index2,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(0.5373534948346590, qext[0],   1e-14);
    EXPECT_NEAR(0.3143612207865766, qext[250], 1e-14);
    EXPECT_NEAR(0.0058357432042841, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0482912065377480, qscat[0],   1e-14);
    EXPECT_NEAR(0.0250656892007021, qscat[250], 1e-14);
    EXPECT_NEAR(0.0010568992342033, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(0.4890622882969110, qabs[0],   1e-14);
    EXPECT_NEAR(0.2892955315858746, qabs[250], 1e-14);
    EXPECT_NEAR(0.0047788439700809, qabs[500], 1e-14);

}

TEST_F(TestSolver, Mie3Layer) {
    const int nlayers = 3;
    const double medium_refrac = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid3, index3,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(2.0261050243604539, qext[0],   1e-14);
    EXPECT_NEAR(0.0248674748630347, qext[250], 1e-14);
    EXPECT_NEAR(0.0012280674844302, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.3388504569262775, qscat[0],   1e-14);
    EXPECT_NEAR(0.0081054247786561, qscat[250], 1e-14);
    EXPECT_NEAR(0.0011010132200970, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(1.6872545674341763, qabs[0],   1e-14);
    EXPECT_NEAR(0.0167620500843786, qabs[250], 1e-14);
    EXPECT_NEAR(0.0001270542643331, qabs[500], 1e-14);

}

TEST_F(TestSolver, Quasi1Layer) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(1.0611537055667197, qext[0],   1e-14);
    EXPECT_NEAR(0.0764461773630003, qext[250], 1e-14);
    EXPECT_NEAR(0.0042801575796004, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0241764183390374, qscat[0],   1e-14);
    EXPECT_NEAR(0.0029871788218089, qscat[250], 1e-14);
    EXPECT_NEAR(0.0002336823836763, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(1.0369772872276823, qabs[0],   1e-14);
    EXPECT_NEAR(0.0734589985411914, qabs[250], 1e-14);
    EXPECT_NEAR(0.0040464751959240, qabs[500], 1e-14);

}

TEST_F(TestSolver, Quasi2Layer) {
    const int nlayers = 2;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid2, index2,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(0.2170155342365016, qext[0],   1e-14);
    EXPECT_NEAR(0.1117949449870051, qext[250], 1e-14);
    EXPECT_NEAR(0.0022927389984260, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0033678648313046, qscat[0],   1e-14);
    EXPECT_NEAR(0.0012481734552626, qscat[250], 1e-14);
    EXPECT_NEAR(0.0000636722865861, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(0.2136476694051971, qabs[0],   1e-14);
    EXPECT_NEAR(0.1105467715317425, qabs[250], 1e-14);
    EXPECT_NEAR(0.0022290667118399, qabs[500], 1e-14);

}
    
TEST_F(TestSolver, Quasi3Layer) {
    const int nlayers = 3;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid3, index3,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    EXPECT_EQ(InvalidNumberOfLayers, result);

}

TEST_F(TestSolver, QuasiProlate) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, 5.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(0.6549347175126272, qext[0],   1e-14);
    EXPECT_NEAR(0.0713320530670000, qext[250], 1e-14);
    EXPECT_NEAR(0.0030711419580141, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0036651236274123, qscat[0],   1e-14);
    EXPECT_NEAR(0.0006166697027189, qscat[250], 1e-14);
    EXPECT_NEAR(0.0000411265232696, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(0.6512695938852150, qabs[0],   1e-14);
    EXPECT_NEAR(0.0707153833642811, qabs[250], 1e-14);
    EXPECT_NEAR(0.0030300154347445, qabs[500], 1e-14);

}

TEST_F(TestSolver, QuasiOblate) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 5.0, 10.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(0.8371294510751321, qext[0],   1e-14);
    EXPECT_NEAR(0.0785298784021552, qext[250], 1e-14);
    EXPECT_NEAR(0.0037899923685295, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0093404960245316, qscat[0],   1e-14);
    EXPECT_NEAR(0.0014531069310511, qscat[250], 1e-14);
    EXPECT_NEAR(0.0001022886141183, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(0.8277889550506005, qabs[0],   1e-14);
    EXPECT_NEAR(0.0770767714711041, qabs[250], 1e-14);
    EXPECT_NEAR(0.0036877037544112, qabs[500], 1e-14);

}

TEST_F(TestSolver, TestIncrement5) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 5, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(1.0981095009299680, qext[0], 1e-14);
    EXPECT_EQ(0.0, qext[1]);
    EXPECT_EQ(0.0, qext[2]);
    EXPECT_EQ(0.0, qext[3]);
    EXPECT_EQ(0.0, qext[4]);
    EXPECT_NEAR(1.0405331345505673, qext[5], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0241147084928534, qscat[0], 1e-14);
    EXPECT_EQ(0.0, qscat[1]);
    EXPECT_EQ(0.0, qscat[2]);
    EXPECT_EQ(0.0, qscat[3]);
    EXPECT_EQ(0.0, qscat[4]);
    EXPECT_NEAR(0.0218343064818317, qscat[5], 1e-14);
    // Absorption
    EXPECT_NEAR(1.0739947924371145, qabs[0], 1e-14);
    EXPECT_EQ(0.0, qabs[1]);
    EXPECT_EQ(0.0, qabs[2]);
    EXPECT_EQ(0.0, qabs[3]);
    EXPECT_EQ(0.0, qabs[4]);
    EXPECT_NEAR(1.0186988280687357, qabs[5], 1e-14);

}

TEST_F(TestSolver, TestIncrement7) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 7, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    EXPECT_EQ(InvalidIncrement, result);

}

TEST_F(TestSolver, TestIncrementNegative) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, -1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    EXPECT_EQ(InvalidIncrement, result);

}

TEST_F(TestSolver, TestSizeCorrect) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 5.0, -1.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, true, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(0.5334378505066342, qext[0],   1e-14);
    EXPECT_NEAR(0.0025491434133546, qext[250], 1e-14);
    EXPECT_NEAR(0.0020526111029140, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(0.0015165842885118, qscat[0],   1e-14);
    EXPECT_NEAR(0.0001902864485487, qscat[250], 1e-14);
    EXPECT_NEAR(0.0000145576138027, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(0.5319212662181224, qabs[0],   1e-14);
    EXPECT_NEAR(0.0023588569648059, qabs[250], 1e-14);
    EXPECT_NEAR(0.0020380534891113, qabs[500], 1e-14);

}

TEST_F(TestSolver, TestMediumRefractiveIndex) {
    const int nlayers = 1;
    const double medium_refrac = 2.0;
    const double radius[2] = { 20.0, -1.0 };
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Checks
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_NEAR(2.8328562530696506, qext[0],   1e-14);
    EXPECT_NEAR(2.0565291634339800, qext[250], 1e-14);
    EXPECT_NEAR(0.0970864882507346, qext[500], 1e-14);
    // Scattering
    EXPECT_NEAR(1.0698792522825473, qscat[0],   1e-14);
    EXPECT_NEAR(1.4426586799505641, qscat[250], 1e-14);
    EXPECT_NEAR(0.0718406948561757, qscat[500], 1e-14);
    // Absorption
    EXPECT_NEAR(1.7629770007871033, qabs[0],   1e-14);
    EXPECT_NEAR(0.6138704834834159, qabs[250], 1e-14);
    EXPECT_NEAR(0.0252457933945589, qabs[500], 1e-14);

}

TEST_F(TestSolver, TestMieQuasiSmall) {
    const int nlayers = 1;
    const double medium_refrac = 1.0;
    const double radius[2] = { 3.0, -1.0 };
    /* Mie */
    ErrorCode result = npspec(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);
    EXPECT_EQ(NoError, result);
    const double radius2[2] = { 3.0, 3.0 };
    double qext2[NLAMBDA], qscat2[NLAMBDA], qabs2[NLAMBDA];
    /* Quasistatic */
    result = npspec(nlayers, radius2, relative_radius_spheroid1, index1,
                     medium_refrac, false, 1, 1.0, 1.0, Efficiency,
                     qext2, qscat2, qabs2);
    EXPECT_EQ(NoError, result);
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

}

TEST_F(TestSpectraTypes, TestCrossSection) {
    ErrorCode result = npspec(nlayers, radius, relative_radius, index,
                         medium_refrac, false, inc, 1.0, 1.0, CrossSection,
                         qext2, qscat2, qabs2);
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_FLOAT_EQ(qext2[0],   qext1[0]   * pi * sqr(radius[0]));
    EXPECT_FLOAT_EQ(qext2[160], qext1[160] * pi * sqr(radius[0]));
    EXPECT_FLOAT_EQ(qext2[640], qext1[640] * pi * sqr(radius[0]));
    // Scattering
    EXPECT_FLOAT_EQ(qscat2[0],   qscat1[0]   * pi * sqr(radius[0]));
    EXPECT_FLOAT_EQ(qscat2[160], qscat1[160] * pi * sqr(radius[0]));
    EXPECT_FLOAT_EQ(qscat2[640], qscat1[640] * pi * sqr(radius[0]));
    // Absorbance
    EXPECT_FLOAT_EQ(qabs2[0],   qabs1[0]   * pi * sqr(radius[0]));
    EXPECT_FLOAT_EQ(qabs2[160], qabs1[160] * pi * sqr(radius[0]));
    EXPECT_FLOAT_EQ(qabs2[640], qabs1[640] * pi * sqr(radius[0]));
}

TEST_F(TestSpectraTypes, TestMolar) {
    ErrorCode result = npspec(nlayers, radius, relative_radius, index,
                         medium_refrac, false, inc, 1.0, 1.0, Molar,
                         qext2, qscat2, qabs2);
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_FLOAT_EQ(qext2[0],
                    qext1[0]   * pi * sqr(radius[0]) 
                               * 1e-14 * avogadro / ( 1000 * log(10) ));
    EXPECT_FLOAT_EQ(qext2[160],
                    qext1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ));
    EXPECT_FLOAT_EQ(qext2[640],
                    qext1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ));
    // Scattering
    EXPECT_FLOAT_EQ(qscat2[0],
                    qscat1[0]   * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) ));
    EXPECT_FLOAT_EQ(qscat2[160],
                    qscat1[160] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) ));
    EXPECT_FLOAT_EQ(qscat2[640],
                    qscat1[640] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) ));
    // Absorbance
    EXPECT_FLOAT_EQ(qabs2[0],
                    qabs1[0]   * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ));
    EXPECT_FLOAT_EQ(qabs2[160],
                    qabs1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ));
    EXPECT_FLOAT_EQ(qabs2[640],
                    qabs1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) ));
}

TEST_F(TestSpectraTypes, TestAbsorption) {
    const double path_length = 1.5, molarity = 0.004;
    ErrorCode result = npspec(nlayers, radius, relative_radius, index,
                         medium_refrac, false, inc, path_length, molarity,
                         Absorption, qext2, qscat2, qabs2);
    EXPECT_EQ(NoError, result);
    // Extinction
    EXPECT_FLOAT_EQ(qext2[0],
                    qext1[0]   * pi * sqr(radius[0]) 
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity);
    EXPECT_FLOAT_EQ(qext2[160],
                    qext1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity);
                    
    EXPECT_FLOAT_EQ(qext2[640],
                    qext1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity);
                    
    // Scattering
    EXPECT_FLOAT_EQ(qscat2[0],
                    qscat1[0]   * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) )
                                * path_length * molarity);
    EXPECT_FLOAT_EQ(qscat2[160],
                    qscat1[160] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) )
                                * path_length * molarity);
    EXPECT_FLOAT_EQ(qscat2[640],
                    qscat1[640] * pi * sqr(radius[0])
                                * 1e-14 * avogadro / ( 1000 * log(10) )
                                * path_length * molarity);
    // Absorbance
    EXPECT_FLOAT_EQ(qabs2[0],
                    qabs1[0]   * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity);
    EXPECT_FLOAT_EQ(qabs2[160],
                    qabs1[160] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity);
    EXPECT_FLOAT_EQ(qabs2[640],
                    qabs1[640] * pi * sqr(radius[0])
                               * 1e-14 * avogadro / ( 1000 * log(10) )
                               * path_length * molarity);
                    
}

TEST_F(TestColors, TestBlack) {
    // Black will from all zero's
    RGB(spec, 1, false, &r, &g, &b);
    EXPECT_FLOAT_EQ(0.0, r);
    EXPECT_FLOAT_EQ(0.0, g);
    EXPECT_FLOAT_EQ(0.0, b);
    RGB_to_HSV(r, g, b, &h, &s, &v);
    EXPECT_FLOAT_EQ(0.0, h);
    EXPECT_FLOAT_EQ(0.0, s);
    EXPECT_FLOAT_EQ(0.0, v);
}

TEST_F(TestColors, TestWhite) {
    // White will from all one's
    for (int i = 0; i < NLAMBDA; i++) 
        spec[i] = 1.0;
    RGB(spec, 1, false, &r, &g, &b);
    EXPECT_FLOAT_EQ(1.0,        r);
    EXPECT_FLOAT_EQ(0.97691756, g);
    EXPECT_FLOAT_EQ(0.95884293, b);
    RGB_to_HSV(r, g, b, &h, &s, &v);
    EXPECT_FLOAT_EQ(26.349688,   h);
    EXPECT_FLOAT_EQ(0.041157044, s);
    EXPECT_FLOAT_EQ(1.0,         v);
}

/*
TEST_F(TestColors, TestRed) {
    // Black will from all one's
    for (int i = 0; i < NLAMBDA; i++) {
        if (wavelengths[i] == 700.0)
            spec[i] = 1.0;
    }
    RGB(spec, 1, false, &r, &g, &b);
    EXPECT_FLOAT_EQ(1.0,        r);
    EXPECT_FLOAT_EQ(0.97691756, g);
    EXPECT_FLOAT_EQ(0.95884293, b);
    RGB_to_HSV(r, g, b, &h, &s, &v);
    EXPECT_FLOAT_EQ(26.349688,   h);
    EXPECT_FLOAT_EQ(0.041157044, s);
    EXPECT_FLOAT_EQ(1.0,         v);
}
*/

TEST_F(TestColors, Test20nmAg) {
    RGB(qabs, 1, false, &r, &g, &b);
    EXPECT_FLOAT_EQ(0.12381521, r);
    EXPECT_FLOAT_EQ(0.10589667, g);
    EXPECT_FLOAT_EQ(0.48082513, b);
    RGB_to_HSV(r, g, b, &h, &s, &v);
    EXPECT_FLOAT_EQ(242.86751,  h);
    EXPECT_FLOAT_EQ(0.77976054, s);
    EXPECT_FLOAT_EQ(0.48082513, v);
}

// Run the tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

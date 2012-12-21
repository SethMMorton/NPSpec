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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid2, index2,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid3, index3,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid2, index2,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid3, index3,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // This should fail - 2 means too many layers for quasi
    EXPECT_EQ(result, 2);

};

TEST_F(TestSolver, QuasiProlate) {
    const int nlayers = 1;
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, 5.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 5.0, 10.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, 1, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, 5, 1.0, 1.0, Efficiency,
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
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, 7, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Bad increment should return 3
    EXPECT_EQ(result, 3);

};

TEST_F(TestSolver, TestIncrementNegative) {
    const int nlayers = 1;
    const double medium_dielectric = 1.0;
    const double radius[2] = { 10.0, -1.0 };
    for (int i = 0; i < NLAMBDA; i++) {
        qext[i] = 0.0;
        qscat[i] = 0.0;
        qabs[i] = 0.0;
    }
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, -1, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);

    // Bad increment should return 3
    EXPECT_EQ(result, 3);

};

TEST_F(TestSolver, TestSizeCorrect) {
    const int nlayers = 1;
    const double medium_dielectric = 1.0;
    const double radius[2] = { 5.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, true, 1, 1.0, 1.0, Efficiency,
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

// TODO test Efficiency alternatives, and conc. / pathlength

    /*
    int i;
    printf("    // Checks\n");
    printf("    EXPECT_EQ(result, 0);\n");
    printf("    // Extinction\n");
    i = 0;
    printf("    EXPECT_NEAR(qext[%d],   %.16f, 1e-14);\n", i, qext[i]);
    i = 250;
    printf("    EXPECT_NEAR(qext[%d], %.16f, 1e-14);\n", i, qext[i]);
    i = 500;
    printf("    EXPECT_NEAR(qext[%d], %.16f, 1e-14);\n", i, qext[i]);
    printf("    // Scattering\n");
    i = 0;
    printf("    EXPECT_NEAR(qscat[%d],   %.16f, 1e-14);\n", i, qscat[i]);
    i = 250;
    printf("    EXPECT_NEAR(qscat[%d], %.16f, 1e-14);\n", i, qscat[i]);
    i = 500;
    printf("    EXPECT_NEAR(qscat[%d], %.16f, 1e-14);\n", i, qscat[i]);
    printf("    // Absorption\n");
    i = 0;
    printf("    EXPECT_NEAR(qabs[%d],   %.16f, 1e-14);\n", i, qabs[i]);
    i = 250;
    printf("    EXPECT_NEAR(qabs[%d], %.16f, 1e-14);\n", i, qabs[i]);
    i = 500;
    printf("    EXPECT_NEAR(qabs[%d], %.16f, 1e-14);\n", i, qabs[i]);
    */

// Run the tests
int main(int argc, char **argv) {
    initiallize_material_index();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

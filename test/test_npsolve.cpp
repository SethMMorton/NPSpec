#include "gtest/gtest.h"
#include "NPSolve.h"

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

TEST_F(TestSolver, Mie1Layer) {
    const int nlayers = 1;
    const double medium_dielectric = 1.0;
    const double radius[2] = { 20.0, -1.0 };
    int result = npsolve(nlayers, radius, relative_radius_spheroid1, index1,
                         medium_dielectric, false, false, 1.0, 1.0, Efficiency,
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
                         medium_dielectric, false, false, 1.0, 1.0, Efficiency,
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
                         medium_dielectric, false, false, 1.0, 1.0, Efficiency,
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

};

// Run the tests
int main(int argc, char **argv) {
    initiallize_material_index();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

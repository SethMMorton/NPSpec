#include "gtest/gtest.h"
#include "NPSolve.h"

// Check that the number of wavelengths is 800
TEST(SanityTest, NLAMBDA_CHECK) {
    EXPECT_EQ(NLAMBDA, 800);
}

// Check that the range of the wavelenths array is correct
TEST(SanityTest, Wavelenths) {
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


TEST(TestSolver, Mie1Layer) {
    const int nlayers = 1;
    // Let's look at Ag
    const int index[nlayers] = { material_index((char*) "Ag") };
    // The dielectric of the medium is 1.0;
    const double medium_dielectric = 1.0;
    // Let's say 20 nm radius
    const double radius[2] = { 20.0, -1.0 };
    // Only one layer, so relative radius is 1.0
    const double relative_radius_spheroid[nlayers][2] = { { 1.0, 1.0 } };
    // Now run the calculation
    double qext[NLAMBDA];
    double qabs[NLAMBDA];
    double qscat[NLAMBDA];
    int result = npsolve(nlayers, radius, relative_radius_spheroid, index,
                         medium_dielectric, false, false, 1.0, 1.0, Efficiency,
                         qext, qscat, qabs);
    //int i;
    //i = 0;
    //printf("NPSolve: Ext %.16f, Sca %.16f, Abs %.16f\n", qext[i], qscat[i], qabs[i]);
    //i = 250;
    //printf("NPSolve: Ext %.16f, Sca %.16f, Abs %.16f\n", qext[i], qscat[i], qabs[i]);
    //i = 500;
    //printf("NPSolve: Ext %.16f, Sca %.16f, Abs %.16f\n", qext[i], qscat[i], qabs[i]);

    // Checks
    EXPECT_EQ(result, 0);
    // Extinction
    EXPECT_NEAR(qext[0],   2.2285778886683643, 1e-10);
    EXPECT_NEAR(qext[250], 0.2386953127709600, 1e-10);
    EXPECT_NEAR(qext[500], 0.0133494242710007, 1e-10);
    // Scattering
    EXPECT_NEAR(qscat[0],   0.3062750316703077, 1e-10);
    EXPECT_NEAR(qscat[250], 0.0572390734717247, 1e-10);
    EXPECT_NEAR(qscat[500], 0.0039184205430821, 1e-10);
    // Absorption
    EXPECT_NEAR(qabs[0],   1.9223028569980565, 1e-10);
    EXPECT_NEAR(qabs[250], 0.1814562392992353, 1e-10);
    EXPECT_NEAR(qabs[500], 0.0094310037279186, 1e-10);
};

// Run the tests
int main(int argc, char **argv) {
    initiallize_material_index();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

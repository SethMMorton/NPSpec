#include <stdexcept>
#include "gtest/gtest.h"
#include "npspec/nanoparticle.h"

using namespace NPSpec;

TEST(SanityTest, TestDefaults) {
    Nanoparticle np;
    EXPECT_EQ(1, np.getNLayers());
    EXPECT_EQ(Efficiency, np.getSpectraType());
    EXPECT_EQ(Absorbance, np.getSpectraProperty());
    EXPECT_EQ(false, np.getSizeCorrect());
    EXPECT_FLOAT_EQ(1.0, np.getPathLength());
    EXPECT_FLOAT_EQ(1.0e-6, np.getConcentration());
    EXPECT_FLOAT_EQ(1.0, np.getMediumRefractiveIndex());
    EXPECT_FLOAT_EQ(1.0, np.getIncrement());
    EXPECT_EQ(Sphere, np.getShape());
    EXPECT_FLOAT_EQ(10.0, np.getSphereRadius());
    EXPECT_FLOAT_EQ(10.0, np.getEllipsoidZRadius());
    EXPECT_FLOAT_EQ(10.0, np.getEllipsoidXYRadius());
    for (int i = 1; i <= MAXLAYERS; i++) {
        EXPECT_EQ(std::string("Ag"), np.getLayerMaterial(i));
        EXPECT_EQ(0, np.getLayerIndex(i));
        if (i == 1) {
            EXPECT_FLOAT_EQ(1.0, np.getSphereLayerRelativeRadius(i));
            EXPECT_FLOAT_EQ(1.0, np.getEllipsoidLayerZRelativeRadius(i));
            EXPECT_FLOAT_EQ(1.0, np.getEllipsoidLayerXYRelativeRadius(i));
        } else {
            EXPECT_FLOAT_EQ(0.0, np.getSphereLayerRelativeRadius(i));
            EXPECT_FLOAT_EQ(0.0, np.getEllipsoidLayerZRelativeRadius(i));
            EXPECT_FLOAT_EQ(0.0, np.getEllipsoidLayerXYRelativeRadius(i));
        }
    }
}

TEST(SetterGetterTest, TestNLayers) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setNLayers(4));
    EXPECT_EQ(4, np.getNLayers());
    EXPECT_THROW(np.setNLayers(11), std::out_of_range);
    EXPECT_EQ(4, np.getNLayers());
    EXPECT_THROW(np.setNLayers(0), std::out_of_range);
    EXPECT_EQ(4, np.getNLayers());
}

TEST(SetterGetterTest, TestShape) {
    Nanoparticle np;
    np.setShape(Ellipsoid);
    EXPECT_EQ(Ellipsoid, np.getShape());
    np.setShape(Sphere);
    EXPECT_EQ(Sphere, np.getShape());
}

TEST(SetterGetterTest, TestSpectraType) {
    Nanoparticle np;
    np.setSpectraType(CrossSection);
    EXPECT_EQ(CrossSection, np.getSpectraType());
    np.setSpectraType(Molar);
    EXPECT_EQ(Molar, np.getSpectraType());
    np.setSpectraType(Absorption);
    EXPECT_EQ(Absorption, np.getSpectraType());
    np.setSpectraType(Efficiency);
    EXPECT_EQ(Efficiency, np.getSpectraType());
}

TEST(SetterGetterTest, TestSpectraProperty) {
    Nanoparticle np;
    np.setSpectraProperty(Scattering);
    EXPECT_EQ(Scattering, np.getSpectraProperty());
    np.setSpectraProperty(Absorbance);
    EXPECT_EQ(Absorbance, np.getSpectraProperty());
    np.setSpectraProperty(Extinction);
    EXPECT_EQ(Extinction, np.getSpectraProperty());
}

TEST(SetterGetterTest, TestSphereRadius) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setSphereRadius(30.0));
    EXPECT_FLOAT_EQ(30.0, np.getSphereRadius());
    EXPECT_NO_THROW(np.setSphereRadius(1.0));
    EXPECT_FLOAT_EQ(1.0, np.getSphereRadius());
    EXPECT_THROW(np.setSphereRadius(0.0), std::domain_error);
    EXPECT_FLOAT_EQ(1.0, np.getSphereRadius());
}

TEST(SetterGetterTest, TestRelativeRadius) {
    Nanoparticle np;
    np.setNLayers(4);
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(1, 0.3));
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(2, 0.15));
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(3, 0.2));
    // The 4th layer should now be set to 0.35
    EXPECT_FLOAT_EQ(0.3,  np.getSphereLayerRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.15, np.getSphereLayerRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.2,  np.getSphereLayerRelativeRadius(3));
    EXPECT_FLOAT_EQ(0.35, np.getSphereLayerRelativeRadius(4));
    // Adjust a layer and check result
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(2, 0.5));
    EXPECT_FLOAT_EQ(0.3,  np.getSphereLayerRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.5,  np.getSphereLayerRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.0,  np.getSphereLayerRelativeRadius(3));
    EXPECT_FLOAT_EQ(0.2, np.getSphereLayerRelativeRadius(4));
    // Adjust a layer and check result
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(4, 0.1));
    EXPECT_FLOAT_EQ(0.3,  np.getSphereLayerRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.5,  np.getSphereLayerRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.1,  np.getSphereLayerRelativeRadius(3));
    EXPECT_FLOAT_EQ(0.1, np.getSphereLayerRelativeRadius(4));
    // Adjust a layer and check result
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(3, 0.9));
    EXPECT_FLOAT_EQ(0.1,  np.getSphereLayerRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.0,  np.getSphereLayerRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.9,  np.getSphereLayerRelativeRadius(3));
    EXPECT_FLOAT_EQ(0.0, np.getSphereLayerRelativeRadius(4));
    // Make sure the errors work
    EXPECT_THROW(np.setSphereLayerRelativeRadius(0, 0.5), std::out_of_range);
    EXPECT_THROW(np.setSphereLayerRelativeRadius(11, 0.5), std::out_of_range);
    EXPECT_THROW(np.setSphereLayerRelativeRadius(2, 1.1), std::domain_error);
    EXPECT_THROW(np.setSphereLayerRelativeRadius(2, -0.1), std::domain_error);
}

TEST(SetterGetterTest, TestEllipsoidRadius) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setEllipsoidRadius(30.0, 30.0));
    EXPECT_FLOAT_EQ(30.0, np.getEllipsoidZRadius());
    EXPECT_FLOAT_EQ(30.0, np.getEllipsoidXYRadius());
    EXPECT_NO_THROW(np.setEllipsoidRadius(1.0, 15.0));
    EXPECT_FLOAT_EQ(1.0, np.getEllipsoidZRadius());
    EXPECT_FLOAT_EQ(15.0, np.getEllipsoidXYRadius());
    EXPECT_THROW(np.setEllipsoidRadius(0.0, 15.0), std::domain_error);
    EXPECT_THROW(np.setEllipsoidRadius(15.0, 0.0), std::domain_error);
    EXPECT_THROW(np.setEllipsoidRadius(0.0, 0.0), std::domain_error);
    EXPECT_FLOAT_EQ(1.0, np.getEllipsoidZRadius());
    EXPECT_FLOAT_EQ(15.0, np.getEllipsoidXYRadius());
}

TEST(SetterGetterTest, TestEllipsoidRelativeRadius) {
    Nanoparticle np;
    np.setNLayers(2);
    EXPECT_NO_THROW(np.setEllipsoidLayerRelativeRadius(1, 0.6, 0.4));
    // The 4th layer should now be set to 0.35
    EXPECT_FLOAT_EQ(0.6, np.getEllipsoidLayerZRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.4, np.getEllipsoidLayerXYRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.4, np.getEllipsoidLayerZRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.6, np.getEllipsoidLayerXYRelativeRadius(2));
    // Adjust a layer and check result
    EXPECT_NO_THROW(np.setEllipsoidLayerRelativeRadius(2, 0.7, 0.6));
    EXPECT_FLOAT_EQ(0.3, np.getEllipsoidLayerZRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.4, np.getEllipsoidLayerXYRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.7, np.getEllipsoidLayerZRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.6, np.getEllipsoidLayerXYRelativeRadius(2));
    // Adjust a layer and check result
    EXPECT_NO_THROW(np.setEllipsoidLayerRelativeRadius(1, 1.0, 1.0));
    EXPECT_FLOAT_EQ(1.0, np.getEllipsoidLayerZRelativeRadius(1));
    EXPECT_FLOAT_EQ(1.0, np.getEllipsoidLayerXYRelativeRadius(1));
    EXPECT_FLOAT_EQ(0.0, np.getEllipsoidLayerZRelativeRadius(2));
    EXPECT_FLOAT_EQ(0.0, np.getEllipsoidLayerXYRelativeRadius(2));
    // Make sure the errors work
    EXPECT_THROW(np.setEllipsoidLayerRelativeRadius(0, 0.5, 0.5), std::out_of_range);
    EXPECT_THROW(np.setEllipsoidLayerRelativeRadius(11, 0.5, 0.5), std::out_of_range);
    EXPECT_THROW(np.setEllipsoidLayerRelativeRadius(2, 1.1, 0.5), std::domain_error);
    EXPECT_THROW(np.setEllipsoidLayerRelativeRadius(2, -0.1, 0.5), std::domain_error);
    EXPECT_THROW(np.setEllipsoidLayerRelativeRadius(2, -0.1, 1.1), std::domain_error);
}

TEST(SetterGetterTest, TestLayerMaterial) {
    Nanoparticle np;
    std::string Cu("Cu");
    char Al[] = "Al";
    EXPECT_NO_THROW(np.setLayerMaterial(2, "Au"));
    EXPECT_EQ(std::string("Au"), np.getLayerMaterial(2));
    EXPECT_EQ(4, np.getLayerIndex(2));
    EXPECT_NO_THROW(np.setLayerMaterial(3, Cu));
    EXPECT_EQ(std::string("Cu"), np.getLayerMaterial(3));
    EXPECT_EQ(10, np.getLayerIndex(3));
    EXPECT_NO_THROW(np.setLayerMaterial(4, Al));
    EXPECT_EQ(std::string("Al"), np.getLayerMaterial(4));
    EXPECT_EQ(1, np.getLayerIndex(4));
    EXPECT_THROW(np.setLayerMaterial(0, Al), std::out_of_range);
    EXPECT_THROW(np.setLayerMaterial(11, Al), std::out_of_range);
    EXPECT_THROW(np.setLayerMaterial(1, "Kryptonite"), std::invalid_argument);
    EXPECT_EQ(std::string("Al"), np.getLayerMaterial(4));
    EXPECT_THROW(np.getLayerMaterial(11), std::out_of_range);
    EXPECT_THROW(np.getLayerIndex(11), std::out_of_range);
}

TEST(SetterGetterTest, TestIncrement) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setIncrement(5));
    EXPECT_EQ(5, np.getIncrement());
    EXPECT_THROW(np.setIncrement(-1), std::domain_error);
    EXPECT_THROW(np.setIncrement(7), std::invalid_argument);
    EXPECT_EQ(5, np.getIncrement());
}

TEST(SetterGetterTest, TestPathLength) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setPathLength(5.0));
    EXPECT_FLOAT_EQ(5.0, np.getPathLength());
    EXPECT_THROW(np.setPathLength(0.0), std::domain_error);
    EXPECT_FLOAT_EQ(5.0, np.getPathLength());
}

TEST(SetterGetterTest, TestConcentration) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setConcentration(1e-3));
    EXPECT_FLOAT_EQ(1e-3, np.getConcentration());
    EXPECT_THROW(np.setConcentration(0.0), std::domain_error);
    EXPECT_FLOAT_EQ(1e-3, np.getConcentration());
}

TEST(SetterGetterTest, TestMediumRefractiveIndex) {
    Nanoparticle np;
    EXPECT_NO_THROW(np.setMediumRefractiveIndex(2.0));
    EXPECT_FLOAT_EQ(2.0, np.getMediumRefractiveIndex());
    EXPECT_THROW(np.setMediumRefractiveIndex(0.0), std::domain_error);
    EXPECT_FLOAT_EQ(2.0, np.getMediumRefractiveIndex());
}

TEST(SetterGetterTest, TestSizeCorrect) {
    Nanoparticle np;
    np.setSizeCorrect(true);
    EXPECT_TRUE(np.getSizeCorrect());
    np.setSizeCorrect(false);
    EXPECT_FALSE(np.getSizeCorrect());
}

TEST(CalculatorTest, TestDefaultCalculations) {
    Nanoparticle np;
    double spec[NLAMBDA];
    EXPECT_NO_THROW(np.calculateSpectrum());
    np.getSpectrum(spec);
    EXPECT_NEAR(1.0739947924371145,    spec[0],   1e-14);
    EXPECT_NEAR(0.077512335915926767,  spec[250], 1e-14);
    EXPECT_NEAR(0.0042230869218847257, spec[500], 1e-14);
}

TEST(CalculatorTest, TestMieLayers) {
    Nanoparticle np;
    double spec[NLAMBDA];
    EXPECT_NO_THROW(np.setNLayers(3));
    EXPECT_NO_THROW(np.setSphereRadius(20.0));
    EXPECT_NO_THROW(np.setLayerMaterial(1, "Ag"));
    EXPECT_NO_THROW(np.setLayerMaterial(2, "Quartz"));
    EXPECT_NO_THROW(np.setLayerMaterial(3, "TiO2"));
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(1, 0.2));
    EXPECT_NO_THROW(np.setSphereLayerRelativeRadius(2, 0.3));
    EXPECT_NO_THROW(np.calculateSpectrum());
    np.getSpectrum(spec);
    EXPECT_NEAR(1.6872545674341763, spec[0],   1e-14);
    EXPECT_NEAR(0.0167620500843786, spec[250], 1e-14);
    EXPECT_NEAR(0.0001270542643331, spec[500], 1e-14);
}

TEST(CalculatorTest, TestQuasiLayers) {
    Nanoparticle np;
    double spec[NLAMBDA];
    EXPECT_NO_THROW(np.setNLayers(2));
    EXPECT_NO_THROW(np.setEllipsoidRadius(10.0, 10.0));
    EXPECT_NO_THROW(np.setLayerMaterial(1, "Ag"));
    EXPECT_NO_THROW(np.setLayerMaterial(2, "Quartz"));
    EXPECT_NO_THROW(np.setEllipsoidLayerRelativeRadius(1, 0.6, 0.6));
    np.setShape(Ellipsoid);
    EXPECT_NO_THROW(np.calculateSpectrum());
    np.getSpectrum(spec);
    EXPECT_NEAR(0.2136476694051971, spec[0],   1e-14);
    EXPECT_NEAR(0.1105467715317425, spec[250], 1e-14);
    EXPECT_NEAR(0.0022290667118399, spec[500], 1e-14);
}

TEST(CalculatorTest, TestMediumRefractiveIndex) {
    Nanoparticle np;
    double spec[NLAMBDA];
    EXPECT_NO_THROW(np.setSphereRadius(20.0));
    np.setSpectraProperty(Scattering);
    EXPECT_NO_THROW(np.setMediumRefractiveIndex(2.0));
    EXPECT_NO_THROW(np.calculateSpectrum());
    np.getSpectrum(spec);
    EXPECT_NEAR(1.0698792522825473, spec[0],   1e-14);
    EXPECT_NEAR(1.4426586799505641, spec[250], 1e-14);
    EXPECT_NEAR(0.0718406948561757, spec[500], 1e-14);
}

TEST(CalculatorTest, TestColor) {
    Nanoparticle np;
    double r, g, b, h, s, v;
    EXPECT_NO_THROW(np.setSphereRadius(20.0));
    EXPECT_NO_THROW(np.calculateSpectrum());
    np.getRGB(&r, &g, &b);
    EXPECT_FLOAT_EQ(0.12381521, r);
    EXPECT_FLOAT_EQ(0.10589667, g);
    EXPECT_FLOAT_EQ(0.48082513, b);
    np.getHSV(&h, &s, &v);
    EXPECT_FLOAT_EQ(242.86751,  h);
    EXPECT_FLOAT_EQ(0.77976054, s);
    EXPECT_FLOAT_EQ(0.48082513, v);
    EXPECT_FLOAT_EQ(0.48082513, np.getOpacity());
}

// Run tests
int main (int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

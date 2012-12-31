#ifndef NANOPARTICLE_H
#define NANOPARTICLE_H

#include <string>

// Add the constants as part of the NPSpec namespace
namespace NPSpec {
#include "constants.h"
}

class Nanoparticle
{
public:
    Nanoparticle();

    // Calculate the spectra!
    int calculateSpectrum();
    // Get the spectra!
    double* getSpectrum() const;

    // Get the colors
    void getRGB(double *r, double *g, double *b) const;
    void getHSV(double *h, double *s, double *v) const;
    double getOpacity() const;

    // Setters
    int setNLayers(int nlay);
    void setShape(NPSpec::NanoparticleShape npshape);
    void setSpectraType(NPSpec::SpectraType stype);
    void setSpectraProperty(NPSpec::SpectraProperty spec);
    int setSphereRadius(double rad);
    int setEllipsoidRadius(double zrad, double xyrad);
    int setSphereLayerRelativeRadius(int nlay, double rrad);
    int setEllipsoidLayerRelativeRadius(int nlay, double zrrad, double xyrrad);
    int setLayerMaterial(int nlay, char *mat);
    int setLayerMaterial(int nlay, std::string mat);
    int setIncrement(int i);
    int setPathLength(double len);
    int setConcentration(double conc);
    void setSizeCorrect(bool corr);
    int setMediumRefractiveIndex(double mref);

    // Getters
    int getNLayers() const;
    NPSpec::NanoparticleShape getShape() const;
    NPSpec::SpectraType getSpectraType() const;
    NPSpec::SpectraProperty getSpectraProperty() const;
    double getSphereRadius() const;
    double getEllipsoidZRadius() const;
    double getEllipsoidXYRadius() const;
    double getSphereLayerRelativeRadius(int nlay) const;
    double getEllipsoidLayerZRelativeRadius(int nlay) const;
    double getEllipsoidLayerXYRelativeRadius(int nlay) const;
    std::string getLayerMaterial(int nlay) const;
    int getLayerIndex(int nlay) const;
    int getIncrement() const;
    double getPathLength() const;
    double getConcentration() const;
    bool getSizeCorrect() const;
    double getMediumRefractiveIndex() const;

private:
    // Private functions
    void updateRadius(NPSpec::NanoparticleShape npshape);
    void updateRelativeRadius(NPSpec::NanoparticleShape npshape);

    // Input parameters to solvers
    int nLayers;
    NPSpec::SpectraType sType;
    NPSpec::SpectraProperty sProp;
    NPSpec::NanoparticleShape shape;
    int increment;
    bool sizeCorrect;
    double pathLength;
    double concentration;
    double mediumRefractiveIndex;
    std::string materials[NPSpec::MAXLAYERS];
    int  materialIndex[NPSpec::MAXLAYERS];
    double radius[2];
    double relativeRadius[NPSpec::MAXLAYERS][2];

    // Auxillary
    double sphereRadius;
    double sphereRelativeRadius[NPSpec::MAXLAYERS];
    double ellipsoidRadius[2];
    double ellipsoidRelativeRadius[NPSpec::MAXLAYERS][2];
    bool gotColor;

    // Results
    double extinction[NPSpec::NLAMBDA];
    double scattering[NPSpec::NLAMBDA];
    double absorbance[NPSpec::NLAMBDA];
    double red;
    double blue;
    double green;
    double hue;
    double saturation;
    double value;

};


#endif // NANOPARTICLE_H

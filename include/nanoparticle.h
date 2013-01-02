#ifndef NANOPARTICLE_H
#define NANOPARTICLE_H

#include <string>
#include "constants.h"

class Nanoparticle
{
public:
    Nanoparticle();

    // Calculate the spectra!
    NPSpec::ErrorCode calculateSpectrum();
    // Get the spectra!
    void getSpectrum(double spec[NPSpec::NLAMBDA]) const;

    // Get the colors
    void getRGB(double *r, double *g, double *b) const;
    void getHSV(double *h, double *s, double *v) const;
    double getOpacity() const;

    // Setters
    NPSpec::ErrorCode setNLayers(int nlay);
    void setShape(NPSpec::NanoparticleShape npshape);
    void setSpectraType(NPSpec::SpectraType stype);
    void setSpectraProperty(NPSpec::SpectraProperty spec);
    NPSpec::ErrorCode setSphereRadius(double rad);
    NPSpec::ErrorCode setEllipsoidRadius(double zrad, double xyrad);
    NPSpec::ErrorCode setSphereLayerRelativeRadius(int layer_num, double rrad);
    NPSpec::ErrorCode setEllipsoidLayerRelativeRadius(int layer_num, double zrrad, double xyrrad);
    NPSpec::ErrorCode setLayerMaterial(int layer_num, std::string mat);
    NPSpec::ErrorCode setIncrement(int i);
    NPSpec::ErrorCode setPathLength(double len);
    NPSpec::ErrorCode setConcentration(double conc);
    void setSizeCorrect(bool corr);
    NPSpec::ErrorCode setMediumRefractiveIndex(double mref);

    // Getters
    int getNLayers() const;
    NPSpec::NanoparticleShape getShape() const;
    NPSpec::SpectraType getSpectraType() const;
    NPSpec::SpectraProperty getSpectraProperty() const;
    double getSphereRadius() const;
    double getEllipsoidZRadius() const;
    double getEllipsoidXYRadius() const;
    double getSphereLayerRelativeRadius(int layer_num) const;
    double getEllipsoidLayerZRelativeRadius(int layer_num) const;
    double getEllipsoidLayerXYRelativeRadius(int layer_num) const;
    std::string getLayerMaterial(int layer_num) const;
    int getLayerIndex(int layer_num) const;
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

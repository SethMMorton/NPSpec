#include "nanoparticle.h"
#include <cstring>
#include <cmath>
/* Wrap this header in a namespace to avoid clobbering things */
#include "NPSolve.h"

using namespace NPSpec;

Nanoparticle::Nanoparticle() :
    nLayers(1),
    sType(Efficiency),
    sProp(Absorbance),
    sizeCorrect(false),
    pathLength(1.0),
    concentration(1.0e-6),
    mediumRefractiveIndex(1.0),
    gotColor(false)
{
    /* Initiallize the Nanoparticle class's lists */

    // All materials are defaulted to Ag
    for (int i = 0; i < 10; i++) {
        materials[i] = "Ag";
        materialIndex[i] = material_index(materials[i].c_str());
    }

    // Radius is defaulted to 10 nm (sphere)
    sphereRadius = 10.0;
    ellipsoidRadius[0] = 10.0;
    ellipsoidRadius[1] = 10.0;

    // Relative radius is 1.0
    sphereRelativeRadius[0] = 1.0;
    ellipsoidRelativeRadius[0][0] = 1.0;
    ellipsoidRelativeRadius[0][1] = 1.0;
    for (int i = 1; i < 10; i++) {
        sphereRelativeRadius[i] = 0.0;
        ellipsoidRelativeRadius[i][0] = 0.0;
        ellipsoidRelativeRadius[i][1] = 0.0;
    }

    // Start as a sphere
    setShape(Sphere);
}

int Nanoparticle::calculateSpectrum()
{
    /* Calculate the spectrum based on the Nanoparticle parameters */

    // Call the solver
    int result = npspec(nLayers,
                        radius,
                        relativeRadius,
                        materialIndex,
                        mediumRefractiveIndex,
                        sizeCorrect,
                        increment,
                        pathLength,
                        concentration,
                        sType,
                        extinction,
                        scattering,
                        absorbance);

    // Recalculate the colors
    RGB(getSpectrum(), increment, false, &red, &blue, &green);
    RGB_to_HSV(red, blue, green, &hue, &saturation, &value);

    /* 0:  OK! */
    /* 1:  Mie calculation failed */
    /* 2:  Too many layers for quasistatic */
    /* 3:  Invalid increment value */
    /* 4:  Size parameter too large for quazistatic */
    /* -1: Size_parameter may be too large for quazistatic */
    return result;
}

/*********
 * Getters
 *********/

double* Nanoparticle::getSpectrum() const {
    /* Return the spectrum that has been calculated */
    switch(sProp) {
    case Extinction:
        return extinction;
    case Absorbance:
        return absorbance;
    case Scattering:
        return scattering;
    }
}

void Nanoparticle::getRGB(double *r, double *g, double *b) const {
    /* Get the RGB colors for this spectrum */
    *r = red;
    *g = green;
    *b = blue;
}

void Nanoparticle::getHSV(double *h, double *s, double *v) const {
    /* Get the HSV colors for this spectrum */
    *h = hue;
    *s = saturation;
    *v = value;
}

double Nanoparticle::getOpacity() const {
    /* Return the opacity of the spectrum */
    return value;
}

int Nanoparticle::getNLayers() const {
    /* Return the number of layers */
    return nLayers;
}

NanoparticleShape Nanoparticle::getShape() const {
    /* Return the current shape of the nanoparticle */
    return shape;
}

SpectraType Nanoparticle::getSpectraType() const {
    /* Return the current spectra type */
    return sType;
}

SpectraProperty Nanoparticle::getSpectraProperty() const {
    /* Return the current spectra property */
    return sProp;
}

double Nanoparticle::getSphereRadius() const {
    /* Get the current stored radius for the sphere shape */
    return sphereRadius;
}

double Nanoparticle::getEllipsoidZRadius() const {
    /* Get the current stored z-axis radius for the ellipsoid shape */
    return ellipsoidRadius[0];
}

double Nanoparticle::getEllipsoidXYRadius() const {
    /* Get the current stored xy-axis radius for the ellipsoid shape */
    return ellipsoidRadius[1];
}

double Nanoparticle::getSphereLayerRelativeRadius(int nlay) const {
    /* Get the current relative radius for the sphere shape for the given layer */
    if (nlay > MAXLAYERS || nlay < 1)
        return -1.0;
    return sphereRelativeRadius[nlay];
}

double Nanoparticle::getEllipsoidLayerZRelativeRadius(int nlay) const {
    /* Get the current relative radius for the z-axis of the ellipsoid shape for the given layer */
    if (nlay > MAXLAYERS || nlay < 1)
        return -1.0;
    return ellipsoidRelativeRadius[nlay][0];
}

double Nanoparticle::getEllipsoidLayerXYRelativeRadius(int nlay) const {
    /* Get the current relative radius for the xy-axis of the ellipsoid shape for the given layer */
    if (nlay > MAXLAYERS || nlay < 1)
        return -1.0;
    return ellipsoidRelativeRadius[nlay][1];
}

std::string Nanoparticle::getLayerMaterial(int nlay) const {
    /* Get the current layer type for the given layer */
    if (nlay > MAXLAYERS || nlay < 1)
        return std::string("ERROR");
    return materials[nlay];
}

int Nanoparticle::getLayerIndex(int nlay) const {
    /* Get the current layer index for the given layer */
    if (nlay > MAXLAYERS || nlay < 1)
        return -1;
    return materialIndex[nlay];
}

int Nanoparticle::getIncrement() const {
    /* Get the current calculation increment */
    return increment;
}

double Nanoparticle::getPathLength() const {
    /* Get the current light path length */
    return pathLength;
}

double Nanoparticle::getConcentration() const {
    /* Get the current solution concentration */
    return concentration;
}

bool Nanoparticle::getSizeCorrect() const {
    /* Return whether size correction is currently turned on */
    return sizeCorrect;
}

double Nanoparticle::getMediumRefractiveIndex() const {
    /* Return the current refractive index of the surrounding medium */
    return mediumRefractiveIndex;
}

/*********
 * Setters
 *********/

int Nanoparticle::setNLayers(int nlay) {
    /* Set the number of layers */
    // NLayers must be between 1 and maxlayers
    if (nlay > MAXLAYERS || nlay < 1)
        return 1;
    nLayers = nlay;
    updateRelativeRadius(shape);
    return 0;
}

void Nanoparticle::setShape(NanoparticleShape npshape) {
    /* Change the nanopaticle's shape */
    shape = npshape;
    updateRadius(shape);
    updateRelativeRadius(shape);
}

void Nanoparticle::setSpectraType(SpectraType stype) {
    /* Change the spectra type */
    sType = stype;
}

void Nanoparticle::setSpectraProperty(SpectraProperty spec) {
    /* Change the spectra type */
    sProp = spec;
}

int Nanoparticle::setSphereRadius(double rad) {
    /* Set the radius for a sphere */
    if (rad <= 0.0)
        return 2;
    sphereRadius = rad;
    updateRadius(shape);
    return 0;
}

int Nanoparticle::setEllipsoidRadius(double zrad, double xyrad) {
    /* Set the radius for an ellipsoid */
    if (zrad <= 0.0 || xyrad <= 0.0)
        return 2;
    ellipsoidRadius[0] = zrad;
    ellipsoidRadius[1] = xyrad;
    updateRadius(shape);
    return 0;
}

int Nanoparticle::setSphereLayerRelativeRadius(int nlay, double rrad) {
    /* Set the relative radius for the given layer of a sphere */
    if (nlay > MAXLAYERS || nlay < 1)
        return 1;
    if (rrad < 0.0 || rrad > 1.0)
        return 2;
    sphereRelativeRadius[nlay] = rrad;
    return 0;
}

int Nanoparticle::setEllipsoidLayerRelativeRadius(int nlay, double zrrad, double xyrrad) {
    /* Set the relative radius for the given layer of an ellipsoid */
    if (nlay > MAXLAYERS || nlay < 1)
        return 1;
    if (zrrad < 0.0 || zrrad > 1.0)
        return 2;
    if (xyrrad < 0.0 || xyrrad > 1.0)
        return 2;
    ellipsoidRelativeRadius[nlay][0] = zrrad;
    ellipsoidRelativeRadius[nlay][1] = xyrrad;
    return 0;
}

int Nanoparticle::setLayerMaterial(int nlay, char *mat) {
    /* Set the material for the given layer */
    return setLayerMaterial(nlay, std::string(mat));
}

int Nanoparticle::setLayerMaterial(int nlay, std::string mat){
    /* Set the material for the given layer */
    if (nlay > MAXLAYERS || nlay < 1)
        return 1;
    int tmp = material_index(mat.c_str());
    if (tmp < 0)
        return 2;
    materialIndex[nlay] = tmp;
    materials[nlay] = mat;
    return 0;
}

int Nanoparticle::setIncrement(int i) {
    /* Change the wavelength increment in the solver */
    if (i < 0)
        return 2;
    else if (std::fmod((double) NLAMBDA, (double) i) > 0.000001)
        return 2;
    increment = i;
    return 0;
}

int Nanoparticle::setPathLength(double len) {
    /* Change the light path length */
    if (len <= 0.0)
        return 2;
    pathLength = len;
    return 0;
}

int Nanoparticle::setConcentration(double conc) {
    /* Change the concentration of the solution */
    if (conc <= 0.0)
        return 2;
    concentration = conc;
    return 0;
}

void Nanoparticle::setSizeCorrect(bool corr) {
    /* Change size correction */
    sizeCorrect = corr;
}

int Nanoparticle::setMediumRefractiveIndex(double mref) {
    /* Change the refractive index of the surrounding medium */
    if (mref < 0.0)
        return 2;
    mediumRefractiveIndex = mref;
    return 0;
}

/*******************
 * Private Functions
 *******************/

void Nanoparticle::updateRadius(NanoparticleShape npshape) {
    /* Update the nanoparticle radius based on shape */
    switch (npshape) {
    case Sphere:
        radius[0] = sphereRadius;
        radius[1] = -1;
        break;
    case Ellipsoid:
        radius[0] = ellipsoidRadius[0];
        radius[1] = ellipsoidRadius[1];
        break;
    }
}

void Nanoparticle::updateRelativeRadius(NanaparticleShape npshape) {
    /* Update the nanoparticle relative radii */
    switch (npshape) {
    case Sphere:
        for (int i = 0; i < nLayers; i++) {
            relativeRadius[i][0] = sphereRelativeRadius[i];
            relativeRadius[i][1] = -1;
        }
        break;
    case Ellipsoid:
        for (int i = 0; i < nLayers; i++) {
            relativeRadius[i][0] = ellipsoidRelativeRadius[i][0];
            relativeRadius[i][1] = ellipsoidRelativeRadius[i][0];
        }
        break;
    }
}

#include <cmath>
#include <iostream>
#include "nanoparticle.h"
#include "NPSpec.h"

using namespace NPSpec;

Nanoparticle::Nanoparticle() :
    nLayers(1),
    sType(Efficiency),
    sProp(Absorbance),
    sizeCorrect(false),
    pathLength(1.0),
    concentration(1.0e-6),
    mediumRefractiveIndex(1.0),
    increment(1.0)
{
    /* Initiallize the Nanoparticle class's lists */

    // All materials are defaulted to Ag
    for (int i = 0; i < MAXLAYERS; i++) {
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
    for (int i = 1; i < MAXLAYERS; i++) {
        sphereRelativeRadius[i] = 0.0;
        ellipsoidRelativeRadius[i][0] = 0.0;
        ellipsoidRelativeRadius[i][1] = 0.0;
    }

    // Start as a sphere
    setShape(Sphere);
}

ErrorCode Nanoparticle::calculateSpectrum()
{
    /* Calculate the spectrum based on the Nanoparticle parameters */

    // Call the solver
    ErrorCode result = npspec(nLayers,
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
    //for (int i = 0; i < NLAMBDA; i++) { std::cout << absorbance[i] << std::endl; }

    // Recalculate the colors
    double spec[NLAMBDA];
    getSpectrum(spec);
    // TODO: Think about transmission vs. not for RGB
    RGB(spec, increment, false, &red, &green, &blue);
    RGB_to_HSV(red, green, blue, &hue, &saturation, &value);

    return result;
}

/*********
 * Getters
 *********/

void Nanoparticle::getSpectrum(double spec[NLAMBDA]) const {
    /* Return the spectrum that has been calculated */
    switch(sProp) {
    case Extinction:
        for (int i = 0; i < NLAMBDA; i++)
            spec[i] = extinction[i];
        break;
    case Absorbance:
        for (int i = 0; i < NLAMBDA; i++) {
            spec[i] = absorbance[i];
        //    std::cout << spec[i] << std::endl;
        }
        break;
    case Scattering:
        for (int i = 0; i < NLAMBDA; i++)
            spec[i] = scattering[i];
        break;
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

double Nanoparticle::getSphereLayerRelativeRadius(int layer_num) const {
    /* Get the current relative radius for the sphere shape for the given layer */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return (double) LayerError;
    return sphereRelativeRadius[layer_num-1];
}

double Nanoparticle::getEllipsoidLayerZRelativeRadius(int layer_num) const {
    /* Get the current relative radius for the z-axis of the ellipsoid shape for the given layer */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return (double) LayerError;
    return ellipsoidRelativeRadius[layer_num-1][0];
}

double Nanoparticle::getEllipsoidLayerXYRelativeRadius(int layer_num) const {
    /* Get the current relative radius for the xy-axis of the ellipsoid shape for the given layer */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return (double) LayerError;
    return ellipsoidRelativeRadius[layer_num-1][1];
}

std::string Nanoparticle::getLayerMaterial(int layer_num) const {
    /* Get the current layer type for the given layer */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return std::string("LayerError");
    return materials[layer_num-1];
}

int Nanoparticle::getLayerIndex(int layer_num) const {
    /* Get the current layer index for the given layer */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return (int) LayerError;
    return materialIndex[layer_num-1];
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

NPSpec::ErrorCode Nanoparticle::setNLayers(int nlay) {
    /* Set the number of layers */
    // NLayers must be between 1 and maxlayers
    if (nlay > MAXLAYERS || nlay < 1)
        return LayerError;
    nLayers = nlay;
    updateRelativeRadius(shape);
    return NoError;
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

ErrorCode Nanoparticle::setSphereRadius(double rad) {
    /* Set the radius for a sphere */
    if (rad <= 0.0)
        return ValueError;
    sphereRadius = rad;
    updateRadius(shape);
    return NoError;
}

ErrorCode  Nanoparticle::setEllipsoidRadius(double zrad, double xyrad) {
    /* Set the radius for an ellipsoid */
    if (zrad <= 0.0 || xyrad <= 0.0)
        return ValueError;
    ellipsoidRadius[0] = zrad;
    ellipsoidRadius[1] = xyrad;
    updateRadius(shape);
    return NoError;
}

ErrorCode Nanoparticle::setSphereLayerRelativeRadius(int layer_num, double rrad) {
    /* Set the relative radius for the given layer of a sphere */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return LayerError;
    if (rrad < 0.0 || rrad > 1.0)
        return ValueError;
    distributeRelativeRadius(layer_num-1, rrad, sphereRelativeRadius);
    updateRelativeRadius(shape);
    return NoError;
}

ErrorCode Nanoparticle::setEllipsoidLayerRelativeRadius(int layer_num, double zrrad, double xyrrad) {
    /* Set the relative radius for the given layer of an ellipsoid */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return LayerError;
    if (zrrad < 0.0 || zrrad > 1.0)
        return ValueError;
    if (xyrrad < 0.0 || xyrrad > 1.0)
        return ValueError;
    double temparr[MAXLAYERS];
    for (int i = 0; i < MAXLAYERS; i++) { temparr[i] = ellipsoidRelativeRadius[i][0]; }
    distributeRelativeRadius(layer_num-1, zrrad, temparr);
    for (int i = 0; i < MAXLAYERS; i++) { ellipsoidRelativeRadius[i][0] = temparr[i]; }
    for (int i = 0; i < MAXLAYERS; i++) { temparr[i] = ellipsoidRelativeRadius[i][1]; }
    distributeRelativeRadius(layer_num-1, xyrrad, temparr);
    for (int i = 0; i < MAXLAYERS; i++) { ellipsoidRelativeRadius[i][1] = temparr[i]; }
    updateRelativeRadius(shape);
    return NoError;
}

ErrorCode Nanoparticle::setLayerMaterial(int layer_num, std::string mat){
    /* Set the material for the given layer */
    if (layer_num > MAXLAYERS || layer_num < 1)
        return LayerError;
    int tmp = material_index(mat.c_str());
    if ((ErrorCode) tmp == UnknownMaterial)
        return ValueError;
    materialIndex[layer_num-1] = tmp;
    materials[layer_num-1] = mat;
    return NoError;
}

ErrorCode Nanoparticle::setIncrement(int i) {
    /* Change the wavelength increment in the solver */
    if (i < 0)
        return ValueError;
    else if (std::fmod((double) NLAMBDA, (double) i) > 0.000001)
        return ValueError;
    increment = i;
    return NoError;
}

ErrorCode Nanoparticle::setPathLength(double len) {
    /* Change the light path length */
    if (len <= 0.0)
        return ValueError;
    pathLength = len;
    return NoError;
}

ErrorCode Nanoparticle::setConcentration(double conc) {
    /* Change the concentration of the solution */
    if (conc <= 0.0)
        return ValueError;
    concentration = conc;
    return NoError;
}

void Nanoparticle::setSizeCorrect(bool corr) {
    /* Change size correction */
    sizeCorrect = corr;
}

ErrorCode Nanoparticle::setMediumRefractiveIndex(double mref) {
    /* Change the refractive index of the surrounding medium */
    if (mref <= 0.0)
        return ValueError;
    mediumRefractiveIndex = mref;
    return NoError;
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

void Nanoparticle::updateRelativeRadius(NanoparticleShape npshape) {
    /* Update the nanoparticle relative radii */
    switch (npshape) {
    case Sphere:
        for (int i = 0; i < nLayers; i++) {
            relativeRadius[i][0] = sphereRelativeRadius[i];
            relativeRadius[i][1] = sphereRelativeRadius[i];
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

void Nanoparticle::distributeRelativeRadius(int n, double rrad, double array[]) {
    /* Given a new relative radius for a layer, change the other layers
       so that they sum to 1.0 */

    /* Make an index array of the order we wish to look through the layers.
       We want to look at layers higher than the given layer, and then
       the lower ones in decreasing order. */
    int index[MAXLAYERS-1], i = 0, k = n + 1;
    while (k < nLayers) { index[i] = k; i++; k++; }
    k = n - 1;
    while (k >= 0) { index[i] = k; i++; k--; }

    /* First set the new value */
    array[n] = rrad;

    /* Reset other relative radii so that they sum to 1.0 */
    double sum = 0.0;
    for (int j = 0; j < nLayers; j++) { sum += array[j]; }
    double remainder = 1.0 - sum;
    for (int j = 0; j < nLayers-1; j++) { i = index[j]; /* Get the index order */

        /* Update the other layers if the sum is not yet 1.0 */
        if (std::abs(1.0 - sum) > 1e-6) {
            /* Sometimes we need to add excess, sometimes subtract */
            if (remainder > 0.0) {
                if (array[i] + remainder < 1.0)
                    array[i] += remainder;
                else
                    array[i] = 1.0;
            } else { /* Remainder negative or zero */
                if (array[i] + remainder > 0.0)
                    array[i] += remainder;
                else
                    array[i] = 0.0;
            }
        }

        /* Increment the sum */
        sum = 0.0;
        for (int m = 0; m < nLayers; m++) { sum += array[m]; }
        remainder = 1.0 - sum;

    }

}

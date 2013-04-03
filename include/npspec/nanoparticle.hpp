/*! \file nanoparticle.hpp
 *  \brief The C++-interface to the NPSpec library
 *
 *  This header file provides the C++ class that provides an simpler
 *  interface to the NPSpec library.  Importing this header will give
 *  access to the [Nanoparticle](\ref Nanoparticle) class as well as everything
 *  in the [constants](\ref constants.h) header.
 */

#ifndef NANOPARTICLE_H
#define NANOPARTICLE_H

#include "npspec/constants.h"
#include <string>

//! \brief Class to hold nanoparticle definition and calculate its spectrum.
/*! Blah blah blah... */
class Nanoparticle
{
public:
    /*! Constructor */
    Nanoparticle();

    //! \brief Calculate the spectra.
    /*!This should only be used after the nanoparticle is defined. */
    int calculateSpectrum();
    //! \brief Get the calculated spectrum.
    /*! This should only be used after calling 
     *  [calculateSpectrum](\ref calculateSpectrum).
     *
     *  \param [out] spec The calculated spectrum. */
    void getSpectrum(double spec[NPSpec::NLAMBDA]) const;

    //! Get the color associated with the calculated spectrum in RGB color space.
    /*! This should only be used after calling 
     *  [getSpectrum](\ref getSpectrum).
     *
     * \param [out] r The RED part of the color space, between 0 and 1 (inclusive).
     * \param [out] g The GREEN part of the color space, between 0 and 1 (inclusive).
     * \param [out] b The BLUE part of the color space, between 0 and 1 (inclusive). */
    void getRGB(double &r, double &g, double &b) const;
    //! Get the color associated with the calculated spectrum in HSV color space.
    /*! This should only be used after calling 
     *  [getSpectrum](\ref getSpectrum).
     *
     * \param [out] h The HUE of the color space, between 0 and 360 (inclusive).
     * \param [out] s The SATURATION part of the color space, between 0 and 1 (inclusive).
     * \param [out] v The VALUE part of the color space, between 0 and 1 (inclusive). */
    void getHSV(double &h, double &s, double &v) const;
    //! Get the opacity of the color associated with the calculated spectrum.
    /*! This should only be used after calling 
     *  [getSpectrum](\ref getSpectrum). */
    double getOpacity() const;

    // Setters
    void setNLayers(int nlay);
    void setShape(NPSpec::NanoparticleShape npshape);
    void setSpectraType(NPSpec::SpectraType stype);
    void setSpectraProperty(NPSpec::SpectraProperty spec);
    void setSphereRadius(double rad);
    void setEllipsoidRadius(double zrad, double xyrad);
    void setSphereLayerRelativeRadius(int layer_num, double rrad);
    void setEllipsoidLayerRelativeRadius(int layer_num, double zrrad, double xyrrad);
    void setLayerMaterial(int layer_num, std::string mat);
    void setIncrement(int i);
    void setPathLength(double len);
    void setConcentration(double conc);
    void setSizeCorrect(bool corr);
    void setMediumRefractiveIndex(double mref);

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
    void distributeRelativeRadius(int n, double rrad, double array[NPSpec::MAXLAYERS]);

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

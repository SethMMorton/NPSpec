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
/*! Currently, the following materials are implemented:
 *  Ag, AlAs, AlSb, Au, Be, CdS, CdSe, Co, Cr, Cu, Cu2O, CuO, Diamond, 
 *  Diamond_film, GaAs, GaP, Ge, Glass, Graphite, InAs, InP, InSb, Ir, 
 *  K, Li, Mo, Na, Nb, Ni, Os, PbS, PbSe, PbTe, Pd, Pt, Quartz, Rh, Si,
 *  SiC, SiO, Ta, Te, TiO2, V, W, ZnS, ZnSe, ZnTe
 *
 *  A maximum of 10 nanoparticle layers is allowed (layer numbers are 1 - 10).
 */
class Nanoparticle
{
public:
    //! Constructor
    /*! The constructor makes the following initiallizations:
     *  - All layers are Ag
     *  - The nanoparticle is one layer
     *  - The shape is a sphere with a radius of 10 nm
     *  - The spectra type is [Efficiency](\ref SpectraType::Efficiency)
     *  - The spectra proprty is [Absorbance](\ref SpectraProperty::Absorbance)
     *  - The spectra will not be calculated with size correction
     *  - The Beer's law path length and concentration is 1 cm and \f$10^{-6}\f$ *M*, 
     *    respectively
     *  - The surrounding medium's refractive index is 1.0
     */
    Nanoparticle();

    //! \brief Calculate the spectra.
    /*! This should only be used after the nanoparticle is defined.
     *  \return retcode Returns 0 if the calculation was sucessful and
     *                  returns 1 if the nanoparticle may be too large and
     *                  thus the spectrum may not be trustworthy.
     *  \exception std::out_of_range This is thrown if the number of layers is out of range
     *  \exception std::invalid_argument Unknown material or invalid increment
     *  \exception std::domain_error Radii, path length, concentration, or refractive index is negative, 
     *                               relative radii don't sum to 1.0
     *
     *  \note For Python, the above exceptions are mapped as:
     *  - **out_of_range** -> **IndexError**
     *  - **domain_error** -> **ValueError**
     *  - **invalid_argument** -> **ValueError**
     */
    int calculateSpectrum();

    //! \brief Get the calculated spectrum.
    /*! This should only be used after calling 
     *  [calculateSpectrum](\ref calculateSpectrum).
     *
     *  \param [out] spec The calculated spectrum.
     *
     *  \note
     *  For Python, `spec` is a numpy array that is returned by the function,
     *  so this would be called as `spec = Nanoparticle.calculateSpectrum()`.
     */
    void getSpectrum(double spec[NPSpec::NLAMBDA]) const;

    //! Get the color associated with the calculated spectrum in RGB color space.
    /*! This should only be used after calling 
     *  [getSpectrum](\ref getSpectrum).  
     *
     *  TODO: Calculate the color for absorbtion and transmission differently.
     *
     * \param [out] r The RED part of the color space, between 0 and 1 (inclusive).
     * \param [out] g The GREEN part of the color space, between 0 and 1 (inclusive).
     * \param [out] b The BLUE part of the color space, between 0 and 1 (inclusive).
     *
     * \note
     * For Python, `r`, `g`, and `b` are returned as a 3-tuple, so this would be called
     * as `r, g, b = Nanoparticle.getRGB()`.
     */
    void getRGB(double &r, double &g, double &b) const;

    //! Get the color associated with the calculated spectrum in HSV color space.
    /*! This should only be used after calling 
     *  [getSpectrum](\ref getSpectrum).
     *
     *  TODO: Calculate the color for absorbtion and transmission differently.
     *
     * \param [out] h The HUE of the color space, between 0 and 360 (inclusive).
     * \param [out] s The SATURATION part of the color space, between 0 and 1 (inclusive).
     * \param [out] v The VALUE part of the color space, between 0 and 1 (inclusive). 
     *
     * \note
     * For Python, `h`, `s`, and `v` are returned as a 3-tuple, so this would be called
     * as `h, s, v = Nanoparticle.getHSV()`.
     */
    void getHSV(double &h, double &s, double &v) const;

    //! Get the opacity of the color associated with the calculated spectrum.
    /*! This should only be used after calling 
     *  [getSpectrum](\ref getSpectrum). */
    double getOpacity() const;

    //! Get the number of layers currently in the nanoparticle.
    /*! \return The number of layers in the nanoparticle. */
    int getNLayers() const;

    //! Get the current shape of the nanoparticle.
    /*! \return The nanoparticle shape. */
    NPSpec::NanoparticleShape getShape() const;

    //! Get the current spectra type of the nanoparticle.
    /*! \return The type of the spectrum. */
    NPSpec::SpectraType getSpectraType() const;

    //! Get the current spectra property of the nanoparticle.
    /*! \return The property the spectrum was calculated. */
    NPSpec::SpectraProperty getSpectraProperty() const;

    //! Get the current sphere radius.
    /*! \return The nanoparticle sphere radius. 
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this will return the same value no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     */
    double getSphereRadius() const;

    //! Get the current Z-component of the ellipsoid radius in nm.
    /*! \return The Z-component of the ellipsoid radius in nm. 
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this will return the same value no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     */
    double getEllipsoidZRadius() const;

    //! Get the current XY-component of the ellipsoid radius in nm.
    /*! \return The XY-component of the ellipsoid radius in nm. 
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this will return the same value no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     *
     *  \remark
     *  The X- and Y- components of the radius are locked so that there are
     *  only two degrees of freedom of the radius (Z and XY).  This allows
     *  us to use analytical formulas for the polarizabilty.
     */
    double getEllipsoidXYRadius() const;

    //! Get the sphere's relative radius in nm of a particular layer.
    /*! \param layer_num The layer of which you wish to know the relative radius.
     *  \return The sphere's relative radius in nm of the chosen layer.
     *  \exception std::out_of_range The layer number is invalid.
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this will return the same value no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     *
     *  \remark
     *  The X- and Y- components of the radius are locked so that there are
     *  only two degrees of freedom of the radius (Z and XY).  This allows
     *  us to use analytical formulas for the polarizabilty.
     *
     *  \note For Python, an **IndexError** is raised instead of **out_of_range**. 
     */
    double getSphereLayerRelativeRadius(int layer_num) const;

    //! Get the ellipsoid's relative radius of a particular layer in the Z-direction.
    /*! \param layer_num The layer of which you wish to know the relative radius.
     *  \return The ellipsoid's relative radius in the Z-direction of the chosen layer.
     *  \exception std::out_of_range The layer number is invalid.
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this will return the same value no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     *
     *  \remark
     *  The X- and Y- components of the radius are locked so that there are
     *  only two degrees of freedom of the radius (Z and XY).  This allows
     *  us to use analytical formulas for the polarizabilty.
     *
     *  \note For Python, an **IndexError** is raised instead of **out_of_range**. 
     */
    double getEllipsoidLayerZRelativeRadius(int layer_num) const;

    //! Get the ellipsoid's relative radius of a particular layer in the XY-direction.
    /*! \param layer_num The layer of which you wish to know the relative radius.
     *  \return The ellispoid's relative radius in the XY-direction of the chosen layer.
     *  \exception std::out_of_range The layer number is invalid.
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this will return the same value no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     *
     *  \remark
     *  The X- and Y- components of the radius are locked so that there are
     *  only two degrees of freedom of the radius (Z and XY).  This allows
     *  us to use analytical formulas for the polarizabilty.
     *
     *  \note For Python, an **IndexError** is raised instead of **out_of_range**. 
     */
    double getEllipsoidLayerXYRelativeRadius(int layer_num) const;

    //! Get the material for the given layer.
    /*! \param layer_num The layer of which you wish to know the relative radius.
     *  \return The material for the chosen layer.
     *  \exception std::out_of_range The layer number is invalid.
     *  
     *  \note For Python, an **IndexError** is raised instead of **out_of_range**. 
     */
    std::string getLayerMaterial(int layer_num) const;

    //! Get the index corresponding to the material for the given layer.
    /*! \param layer_num The material index of which you wish to know the relative radius.
     *  \return The material index for the chosen layer.
     *  \exception std::out_of_range The layer number is invalid.
     *  
     *  \note For Python, an **IndexError** is raised instead of **out_of_range**. 
     */
    int getLayerIndex(int layer_num) const;

    //! Get the increment used when calculating the spectrum.
    /*! \return The increment for calculating the spectrum. */
    int getIncrement() const;

    //! Get the Beer's law path length in centimeters.
    /*! \return The Beer's law path length in centimeters. */
    double getPathLength() const;

    //! Get the Beer's law concentration in molarity.
    /*! \return The Beer's law concentration in molarity. */
    double getConcentration() const;

    //! Get the size correction flag.
    /*! \return True if size correction is enabled, False otherwise. */
    bool getSizeCorrect() const;

    //! Get the refractive index of the surrounding medium.
    /*! \return The surrounding medium's refractive index. */
    double getMediumRefractiveIndex() const;

    //! Sets the number of layers currently in the nanoparticle.
    /*! \param nlay The number of layers you wish the nanoparticle to have.  The default is 1.
     *  \exception std::out_of_range The requested number of layers is illegal.
     *
     *  \warning
     *  When you change the number of layers, the relative radii of each layer
     *  is not adjusted so you should reset the relative radii after changing
     *  the number of layers.
     *
     *  \note For Python, an **IndexError** is raised in place of **out_of_range**. 
     */
    void setNLayers(int nlay);

    //! Sets the current nanoparticle shape.
    /* \param npshape The new shape of the nanoparticle.
     *
     * \remark
     * When you change the shape of the nanoparticle, the parameters for
     * the other shape remain in memory so you can switch back and forth
     * and not have to reset the parameters.
     */
    void setShape(NPSpec::NanoparticleShape npshape);

    //! Sets the type of spectrum to calculate.
    /*! \param stype The type of the spectrum you wish to calculate.
     */
    void setSpectraType(NPSpec::SpectraType stype);

    //! Sets the property of the spectrum for which to calculate.
    /*! \param spec The property of the spectrum you wish to calculate.
     */
    void setSpectraProperty(NPSpec::SpectraProperty spec);

    //! Sets the current spherical radius in nm.
    /*! \param rad The radius to set for a spherical nanoparticle.
     *  \exception std::domain_error A non-positive radius was given.
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this can be set no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     *
     *  \note
     *  For Python, a **ValueError** is raised in place of **domain_error**.
     */
    void setSphereRadius(double rad);

    //! Sets the current ellipsoid radius in nm.
    /*! \param zrad The Z-component radius to set for a spherical nanoparticle.
     *  \param xyrad The XY-component radius to set for a spherical nanoparticle.
     *  \exception std::domain_error A non-positive radius was given.
     *
     *  \remark 
     *  Internally, the sphere radius and ellipsoid radius are stored in
     *  different variables, so this can be set no matter
     *  if the nanoparticle is currently a sphere or ellipsoid.
     *
     *  \remark
     *  The X- and Y- components of the radius are locked so that there are
     *  only two degrees of freedom of the radius (Z and XY).  This allows
     *  us to use analytical formulas for the polarizabilty.
     *
     *  \note
     *  For Python, a **ValueError** is raised in place of **domain_error**.
     */
    void setEllipsoidRadius(double zrad, double xyrad);

    //! Sets the spherical relative radius for the given layer.
    /*! \param layer_num The layer of which you wish to change the relative radius.
     *  \param rrad The new relative radius of the given layer.
     *              The relative radius must be between 0 and 1.
     *  \exception std::out_of_range The requested number of layers is illegal.
     *  \exception std::domain_error A non-positive relative radius was given.
     *
     *  \remark
     *  The sum of the relative radii must be 1.0.  When a new relative radius
     *  for a layer is set, the program will attempt to adjust the other relative
     *  radii so that the sum remains 1.0.  Layers higher than the given layer
     *  will be adjusted first, and then the lower layers in decreasing order.
     *
     *  \note
     *  For Python, the above exceptions are mapped as:
     *  - **out_of_range** -> **IndexError**
     *  - **domain_error** -> **ValueError**
     */
    void setSphereLayerRelativeRadius(int layer_num, double rrad);

    //! Sets the ellipsoid relative radiis for the given layer.
    /*! \param layer_num The layer of which you wish to change the relative radius.
     *  \param zrrad The new Z-component relative radius of the given layer.
     *               The relative radius must be between 0 and 1.
     *  \param xyrrad The new XY-component relative radius of the given layer.
     *                The relative radius must be between 0 and 1.
     *  \exception std::out_of_range The requested number of layers is illegal.
     *  \exception std::domain_error A non-positive relative radius was given.
     *
     *  \remark
     *  The sum of the relative radii must be 1.0.  When a new relative radius
     *  for a layer is set, the program will attempt to adjust the other relative
     *  radii so that the sum remains 1.0.  Layers higher than the given layer
     *  will be adjusted first, and then the lower layers in decreasing order.
     *
     *  \remark
     *  The X- and Y- components of the radius are locked so that there are
     *  only two degrees of freedom of the radius (Z and XY).  This allows
     *  us to use analytical formulas for the polarizabilty.
     *
     *  \note
     *  For Python, the above exceptions are mapped as:
     *  - **out_of_range** -> **IndexError**
     *  - **domain_error** -> **ValueError**
     */
    void setEllipsoidLayerRelativeRadius(int layer_num, double zrrad, double xyrrad);

    //! Sets the material for a particular layer of the nanoparticle
    /*! \param layer_num The layer to set the material for.
     *  \param mat String specifying the material to set.  
     *             The default for all layers is "Ag".
     *  \exception std::out_of_range The requested number of layers is illegal.
     *  \exception std::invalid_argument Unknown material.
     *
     *  \note
     *  For Python, the above exceptions are mapped as:
     *  - **out_of_range** -> **IndexError**
     *  - **invalid_argument** -> **ValueError**
     */
    void setLayerMaterial(int layer_num, std::string mat);

    //! Sets the increment used when calculating the spectrum.
    /*! \param i This is the increment to use when looping over the
     *           wavelengths.  There are 800 wavelengths in increments
     *           of 1 nm.  This value must be a factor 800.  This is
     *           useful if you have to repeatedly calculate the spectrum
     *           such as in a real-time GUI.  The default is 1.
     */
    void setIncrement(int i);

    //! Sets the Beer's law path length in centimeters.
    /*! \param len When calculating absorption, this is the
     *             Beer's law path length in cm to use.
     *             The default is 1.0 cm.
     */
    void setPathLength(double len);

    //! Sets the Beer's law concentration in molarity.
    /*! \param conc When calculating absorption, this is the
     *              Beer's law path concentration in molarity
     *              to use.  The default is \f$10^{-6}\f$ *M*.
     */
    void setConcentration(double conc);

    //! Sets whether size correction will be used in the spectrum calculation or not.
    /*! \param corr If true size correction will be enabled, if false it will not.
     *              The default is false.
     *  
     *  \remark
     *  Not all materials have size correction parameters implemented.  Currently,
     *  only Ag, Al, Au, Be, Cu, Pd and Pt are implemented.  Other materials
     *  will simply silently ignore this value.
     */
    void setSizeCorrect(bool corr);

    //! Sets the refractive index of the surrounding medium.
    /*! \param mref The refracive index of the surrounding medium.  
     *              The default is 1.0.
     */
    void setMediumRefractiveIndex(double mref);

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

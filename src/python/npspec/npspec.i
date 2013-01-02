/* File npspec.i */
%module npspec
%include "typemaps.i"

/* This block is like the header for SWIG.  It tells it the symbols it will
 * need to wrap the functions */
%{
#define SWIG_FILE_WITH_INIT
#include "npspec/nanoparticle.h"
#include "npspec/constants.h"
%}

/* Tell python how to handle C++ strings */
%typemap(in) (std::string mat) {
    $1 = PyString_AsString($input);
};

%typemap(out) std::string {
    $result = PyString_FromString($1.c_str());
};

/* Tell python how to output multiple return values */
%apply double *OUTPUT { double *r, double *g, double *b };
%apply double *OUTPUT { double *h, double *s, double *v };

/* Define how exception handling should be peformed for some functions */
%exception Nanoparticle::calculateSpectrum {
    $action
    switch(result) {
    case NPSpec::NanoparticleTooLarge:
        PyErr_SetString(PyExc_RuntimeError, "The nanoparticle is too large for the calculation to be successful");
        return NULL;
    case NPSpec::SizeParameterWarning:
        return PyInt_FromLong(1l);
    case NPSpec::InvalidRadius:
        PyErr_SetString(PyExc_ValueError, "The given radius is invalid");
        return NULL;
    case NPSpec::InvalidRelativeRadius:
        PyErr_SetString(PyExc_ValueError, "The given relative radius is invalid");
        return NULL;
    case NPSpec::InvalidIncrement:
        PyErr_SetString(PyExc_ValueError, "The increment must be a factor of NLAMBDA");
        return NULL;
    case NPSpec::InvalidPathLength:
        PyErr_SetString(PyExc_ValueError, "The path length must be positive");
        return NULL;
    case NPSpec::InvalidConcentration:
        PyErr_SetString(PyExc_ValueError, "The concentration must be positive");
        return NULL;
    case NPSpec::InvalidRefractiveIndex:
        PyErr_SetString(PyExc_ValueError, "The refractive index must be positive");
        return NULL;
    case NPSpec::InvalidNumberOfLayers:
        PyErr_SetString(PyExc_IndexError, "Number of layers is invalid");
        return NULL;
    default:
        Py_RETURN_NONE;
    }
}

%exception Nanoparticle::getSphereLayerRelativeRadius {
    $action
    if (result == (double) NPSpec::LayerError) {
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    } 
}

%exception Nanoparticle::getEllipsoidLayerZRelativeRadius {
    $action
    if (result == (double) NPSpec::LayerError) {
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    }
}

%exception Nanoparticle::getEllipsoidLayerXYRelativeRadius {
    $action
    if (result == (double) NPSpec::LayerError) {
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    }
}


%exception Nanoparticle::getLayerMaterial {
    $action
    if (result == "LayerError") {
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    }
}

%exception Nanoparticle::getLayerIndex {

    $action
    if (result == (int) NPSpec::LayerError) {
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    }
}

%exception Nanoparticle::setNLayers {
    $action
    if (result == NPSpec::LayerError) {
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    } else { Py_RETURN_NONE; }
}

%exception Nanoparticle::setSphereRadius {
    $action
    if (result == NPSpec::ValueError) {
        PyErr_SetString(PyExc_ValueError, "Radius must be positive");
        return NULL;
    } else { Py_RETURN_NONE; }
}

%exception Nanoparticle::setEllipsoidRadius {
    $action
    if (result == NPSpec::ValueError) {
        PyErr_SetString(PyExc_ValueError, "Radius must be positive");
        return NULL;
    } else { Py_RETURN_NONE; }
}

%exception Nanoparticle::setSphereLayerRelativeRadius {
    $action
    switch(result) {
    case NPSpec::ValueError:
        PyErr_SetString(PyExc_ValueError, "Relative radius must be between 0 and 1, inclusive");
        return NULL;
    case NPSpec::LayerError:
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    default:
        Py_RETURN_NONE;
    }
}

%exception Nanoparticle::setEllipsoidLayerRelativeRadius {
    $action
    switch(result) {
    case NPSpec::ValueError:
        PyErr_SetString(PyExc_ValueError, "Relative radius must be between 0 and 1, inclusive");
        return NULL;
    case NPSpec::LayerError:
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    default:
        Py_RETURN_NONE;
    }
}

%exception Nanoparticle::setLayerMaterial {
    $action
    switch(result) {
    case NPSpec::ValueError:
        PyErr_SetString(PyExc_ValueError, "Unknown material given");
        return NULL;
    case NPSpec::LayerError:
        PyErr_SetString(PyExc_IndexError, "Number of layers must be between 1 and MAXLAYERS");
        return NULL;
    default:
        Py_RETURN_NONE;
    }
}

%exception Nanoparticle::setIncrement {
    $action
    if (result == NPSpec::ValueError) {
        PyErr_SetString(PyExc_ValueError, "Increment must be a factor of NLAMBDA");
        return NULL;
    } else { Py_RETURN_NONE; }
}

%exception Nanoparticle::setPathLength {
    $action
    if (result == NPSpec::ValueError) {
        PyErr_SetString(PyExc_ValueError, "Path length must be positive");
        return NULL;
    } else { Py_RETURN_NONE; }
}

%exception Nanoparticle::setConcentration {
    $action
    if (result == NPSpec::ValueError) {
        PyErr_SetString(PyExc_ValueError, "Concentration must be positive");
        return NULL;
    } else { Py_RETURN_NONE; }
}

%exception Nanoparticle::setMediumRefractiveIndex {
    $action
    if (result == NPSpec::ValueError) {
        PyErr_SetString(PyExc_ValueError, "Refractive index must be positive");
        return NULL;
    } else { Py_RETURN_NONE; }
}

/* Below is a list of what to wrap */
%include "npspec/nanoparticle.h"
namespace NPSpec {
    %constant const int NLAMBDA = 800;
    %constant const int MAXLAYERS = 10;
    enum SpectraType { Efficiency, CrossSection, Molar, Absorption };
    enum NanoparticleShape { Sphere, Ellipsoid };
    enum SpectraProperty { Extinction, Absorbance, Scattering };
    %immutable wavelengths;
    extern const double wavelengths[];
}


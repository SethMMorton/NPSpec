/* File npspec.i */
%module(docstring="A python wrapper for the NPSpec C++ library") npspec
%include "std_string.i"

/* This block is like the header for SWIG.  It tells it the symbols it will
 * need to wrap the functions */
%{
#define SWIG_FILE_WITH_INIT
#include "npspec/nanoparticle.h"
#include <stdexcept>
%}

/* We need to run this function at the beginning of module loading. */
%include "numpy.i"
%init %{
    import_array();
%}

/* Tell python how to output multiple return values */
%apply double *OUTPUT { double &r, double &g, double &b };
%apply double *OUTPUT { double &h, double &s, double &v };

/* Tell python how to output C arrays */
%apply double ARGOUT_ARRAY1[ANY] { double spec[NPSpec::NLAMBDA] };
%apply double ARGOUT_ARRAY1[ANY] { double wv[NPSpec::NLAMBDA] };

/* Define how exception handling should be peformed */
%exception {
    try {
        $action
    } catch (std::out_of_range& e) {
        PyErr_SetString(PyExc_IndexError, e.what());
        return NULL;
    } catch (std::domain_error& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
        return NULL;
    } catch (std::invalid_argument& e) {
        PyErr_SetString(PyExc_ValueError, e.what());
        return NULL;
    }
}

/* Below is a list of what to wrap */
%include "npspec/nanoparticle.h"
namespace NPSpec {
    %constant const int NLAMBDA = 800;
    %constant const int MAXLAYERS = 10;
    enum SpectraType { Efficiency, CrossSection, Molar, Absorption };
    enum NanoparticleShape { Sphere, Ellipsoid };
    enum SpectraProperty { Extinction, Absorbance, Scattering };
}

/* Make a function to output the wavelengths 
   array to the module namespace */
%inline %{
void _get_wavelengths(double wv[NPSpec::NLAMBDA]) {
    for (int i = 0; i < NPSpec::NLAMBDA; i++)
        wv[i] = NPSpec::wavelengths[i];
}
%}

/* Python code to place it into the namespace */
%pythoncode %{
# Make the wavelengths a read-only module-level numpy array
wavelengths = _get_wavelengths()
wavelengths.flags.writeable = False
%}

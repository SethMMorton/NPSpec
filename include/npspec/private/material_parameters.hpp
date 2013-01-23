#ifndef MATERIAL_PARAMETERS_H
#define MATERIAL_PARAMETERS_H

#include "npspec/constants.h"
#include <complex>

extern const double drude_parameters[][3];
extern const std::complex<double> experimental_dielectrics[][NPSpec::NLAMBDA];

#endif // MATERIAL_PARAMETERS_H

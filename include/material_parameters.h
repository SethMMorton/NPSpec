#ifndef MATERIAL_PARAMETERS_H
#define MATERIAL_PARAMETERS_H

#include <complex>
#include "NPSolve.h"
using namespace std;

extern const double drude_parameters[][3];
extern const complex<double> experimental_dielectrics[][NLAMBDA];

#endif // MATERIAL_PARAMETERS_H

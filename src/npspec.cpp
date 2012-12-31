/*=========== Light Scattering by Spherical Particles ==============
*   Calculations of extinction, scattering, absorption, etc.
*   efficiency factors for n-layered spheres.
*------------------------------------------------------------------
*  Input data    :
*   nlayers      : number of layers
*   rad          : the radius of each layer (from inner surface
*                                            to outer surface)
*   rel_rad      : The relative 
*   mrefrac      : refractive index of surrounding medium
*   indx         : The index of the material for each later
*   size_correct : Whether to size correct dielectric or not
*------------------------------------------------------------------
*  Output data   :
*   spectra      : A collection of all forms of the spectra
*------------------------------------------------------------------
* Recursive algorithms of Wu & Wang (Radio Sci. 26, 1393, 1991)
* created by N.V. Voshchinnikov
* (c) 1999 Sobolev Astronomical Institute, St. Petersburg Univ.
*
* Adapted for C by Seth Morton, 2012.
* Added quasistatic approximation
*==================================================================*/

#include <cmath>
#include <complex>
#include <iostream>
#include "solvers.h"
#include "material_parameters.h"
#include "NPSpec.h"

using namespace std;
using namespace NPSpec;

const double pi        = 4.0 * atan(1.0);
const double hbar      = 6.5821189916e-16;
const double avogadro  = 6.0221412927e23;

/* Conversions */
inline double sqr(double x) { return x*x; }
inline double nm2ev(double x) { return 1239.0 / x; }
inline double mPerS2eV(double x, double r) { return x * hbar / ( r * 1.0E-9 ); }

/* Drude inline function */
inline complex<double> drude (double om, double plasmon, double gamma, double sizecorr) {
    return sqr(plasmon) / ( om * ( om + complex<double>(0.0, 1.0) * ( gamma + sizecorr ) ) );
}

int npspec (const int nlayers,              /* Number of layers */
            const double rad[2],            /* Radius of object */
            const double rel_rad[][2],      /* Relative radii of layers */
            const int indx[],               /* Material index of layers */
            const double mrefrac,           /* Refractive index of medium */
            const bool size_correct,        /* Use size correction? */
            const int increment,            /* Increment of wavelengths */
            const double path_length,       /* Path length for absorbance */
            const double concentration,     /* The concentration of solution */
            const SpectraType spectra_type, /* What spectra to return */
            double extinct[],               /* Extinction */
            double scat[],                  /* Scattering */
            double absorb[]                 /* Absorption */
          )
{

    /* Make sure the increment is a factor of 800, and is positive */
    if (increment < 0)
        return 3;
    else if (fmod((double) NLAMBDA, (double) increment) > 0.000001)
        return 3;

    /* If the second component is negative, it is a sphere
       and thus Mie theory is used.  Otherwise, quasistatic is used. */
    bool lmie = false;
    if (rad[1] < 0.0)
        lmie = true;

    /* Calculate radius of a sphere with an equivalent volume */
    double sphere_rad;
    if (lmie)
        sphere_rad = rad[0];
    else
        sphere_rad = cbrt(rad[0] * rad[1] * rad[1]);

    /***************************************************
     * Loop over each wavelength to calculate properties
     ***************************************************/

    int returnvalue = 0;
    for (int i = 0; i < NLAMBDA; i += increment) {

        /* Determine size parameter */
        double size_param = 2.0 * pi * sphere_rad * mrefrac / wavelengths[i];

        /* Skip if size_param is too small */
        if (size_param < 0.1E-6)
            continue;
        else if (!lmie && size_param > 3.5)
            return 4; /* Wicked bad value */
        else if (!lmie && size_param > 3.0)
            returnvalue = -1; /* Possibly bad value */

        /*****************************************************************
         * Calculate dielectric constant & refractive index for each layer
         *****************************************************************/

        /* Dielectric function and refractive index */
        complex<double> dielec[MAXLAYERS], refrac_indx[MAXLAYERS];

        for (int j = 0; j < nlayers; j++) {

            /* Grab dielectric from experiment */
            dielec[j] = experimental_dielectrics[indx[j]][i];

            /* Correct for size if the asked for */
            if (size_correct) {

                /* Extract the drude parameters */
                double pf = drude_parameters[indx[j]][0];
                double gm = drude_parameters[indx[j]][1];
                double sc = drude_parameters[indx[j]][2];

                /* Energy in electron volts (omega) */
                double om = nm2ev(wavelengths[i]);

                /* Use the drude model to size-correct experimental data */
                dielec[j] = dielec[j]
                          - drude(om, pf, gm, 0.0)
                          + drude(om, pf, gm, mPerS2eV(sc, sphere_rad));

            }

            /* Turn dielectric into refractive index if Mie theory */
            if (lmie) {
                double tmp0 = abs(dielec[j]);
                double tmp1 = sqrt(( tmp0 + real(dielec[j]) ) / 2.0);
                double tmp2 = sqrt(( tmp0 - real(dielec[j]) ) / 2.0);
                refrac_indx[j] = complex<double>(tmp1, tmp2);
            }

        }

        /* Solve using the appropriate inputs and theory */
        if (lmie) {
            /* Relative radius for sphere */
            double srrad[MAXLAYERS];
            for (int k = 0; k < nlayers; k++)
                srrad[k] = rel_rad[k][0];
            double backscat, rad_pressure, albedo, asymmetry;
            int retval = mie(nlayers, refrac_indx, srrad, size_param,
                             &extinct[i], &scat[i], &absorb[i],
                             &backscat, &rad_pressure, &albedo, &asymmetry);
            if (retval > 0) return 1;
        } else {
            int retval = quasi(nlayers, dielec, sqr(mrefrac), rel_rad,
                               rad, size_param,
                               &extinct[i], &scat[i], &absorb[i]);
            if (retval > 0) return 2;
        }
        /* Change the spectra type accordingly */
        if (spectra_type != Efficiency) {
            extinct[i] *= pi * sqr(sphere_rad);
            scat[i]    *= pi * sqr(sphere_rad);
            absorb[i]  *= pi * sqr(sphere_rad);
        }
        if (spectra_type == Molar || spectra_type == Absorbtion) {
            extinct[i] *= 1E-14 * avogadro / ( 1000 * log(10) );
            scat[i]    *= 1E-14 * avogadro / ( 1000 * log(10) );
            absorb[i]  *= 1E-14 * avogadro / ( 1000 * log(10) );
        }
        if (spectra_type == Absorbtion) {
            extinct[i] *= path_length * concentration;
            scat[i]    *= path_length * concentration;
            absorb[i]  *= path_length * concentration;
        }

    }

    return returnvalue;

}

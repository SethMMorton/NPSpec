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
#include "solvers.h"
#include "material_parameters.h"
#include "NPSolve.h"

#define MAXLAYERS 10
#define PI 3.14159265358979323846
#define HBAR 6.5821189916e-16
#define AVOGADRO 6.0221412927e23

using namespace std;

/* Conversions */
inline double sqr(double x) { return x*x; }
inline double NM2EV(double x) { return 1239.0 / x; }
inline double MPERS2EV(double x, double r) { return x * HBAR / ( r * 1.0E-9 ); }

/* Drude inline function */
inline complex<double> drude (double om, double plasmon, double gamma, double sizecorr) {
    return sqr(plasmon) / ( om * ( om + complex<double>(0.0, 1.0) * ( gamma + sizecorr ) ) );
}

int npsolve (int nlayers,         /* Number of layers */
             double rad[2],       /* Radius of object */
             double rel_rad[][2], /* Relative radii of layers */
             int indx[],          /* Material index of layers */
             double mrefrac,      /* Refractive index of medium */
             bool size_correct,   /* Use size correction? */
             double path_length,  /* Path length for absorbance */
             double concentration,/* The concentration of solution */
             int spectra_type,    /* What spectra to return */
             double extinct[],    /* Extinction */
             double scat[],       /* Scattering */
             double absorb[]      /* Absorption */
           )
{

    /* Dielectric function and refractive index */
    complex<double> dielec[MAXLAYERS], refrac_indx[MAXLAYERS];

    /* If the second or third components are negative, it is a sphere
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

    for (int i = 0; i < NLAMBDA; i++) {

        /* Determine size parameter */
        double size_param = 2.0 * PI * sphere_rad * mrefrac / wavelengths[i];

        /* Skip if size_param is too small */
        if (size_param < 0.1E-6)
            continue;

        /*****************************************************************
         * Calculate dielectric constant & refractive index for each layer
         *****************************************************************/

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
                double om = NM2EV(wavelengths[i]);

                /* Use the drude model to size-correct experimental data */
                dielec[j] = dielec[j]
                          - drude(om, pf, gm, 0.0)
                          + drude(om, pf, gm, MPERS2EV(sc, sphere_rad));

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
            if (retval == 1)
                return 1; /* Product of size param & ref index too large */
        } else {
            int retval = quasi(nlayers, dielec, sqr(mrefrac), rel_rad,
                               rad, size_param,
                               &extinct[i], &scat[i], &absorb[i]);
            if (retval == 1)
                return 2; /* Too many layers for quasistatic approx */
        }
        /* Change the spectra type accordingly */
        if (spectra_type != Efficiency) {
            extinct[i] *= PI * sqr(sphere_rad);
            scat[i]    *= PI * sqr(sphere_rad);
            absorb[i]  *= PI * sqr(sphere_rad);
        }
        if (spectra_type == Molar || spectra_type == Absorbance) {
            extinct[i] *= 1E-14 * AVOGADRO / ( 1000 * log(10) );
            scat[i]    *= 1E-14 * AVOGADRO / ( 1000 * log(10) );
            absorb[i]  *= 1E-14 * AVOGADRO / ( 1000 * log(10) );
        }
        if (spectra_type == Absorbance) {
            extinct[i] *= path_length * concentration;
            scat[i]    *= path_length * concentration;
            absorb[i]  *= path_length * concentration;
        }

    }

    return 0;

}

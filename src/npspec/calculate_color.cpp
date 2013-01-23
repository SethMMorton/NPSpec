#include "npspec/npspec.h"
#include "npspec/private/standard_color_matching.hpp"
#include <cmath>
#include <algorithm>
#include <numeric>
using namespace NPSpec;

/* Static variables that need to be calculated only once */
static bool first = true;
static double XARRAY[NLAMBDA], YARRAY[NLAMBDA], ZARRAY[NLAMBDA];

/* Convert a spectrum to RGB color space.  Note that RGV is on [0,1], but
 * the conversion may result in values <0 or >1, which means that not
 * all possible colors can be represented by RGB */
void RGB(const double spec_in[], 
         const int inc, 
         const bool trans, 
         double *r, 
         double *g,
         double *b) {

    /* These produts are the same each time.  Only calculate the first time */
    if (first) {
        for (int i = 0; i < NLAMBDA; ++i) {
            XARRAY[i] = CIE_X[i] * CIE_D65[i];
            YARRAY[i] = CIE_Y[i] * CIE_D65[i];
            ZARRAY[i] = CIE_Z[i] * CIE_D65[i];
        }
        first = false;
    }

    /* First, normalize the spectra.  How this is done is based on the type.
     * Also extract the numbers from the CIE arrays that are needed. */
    double XARR[NLAMBDA], YARR[NLAMBDA], ZARR[NLAMBDA];
    double spec[NLAMBDA];
    /* First find the max value */
    double invmax;
    if (!trans) {
        double max = -1;
        for (int i = 0; i < NLAMBDA; i += inc) {
            if (spec_in[i] > max)
                max = spec_in[i];
        }
        invmax = max != 0.0 ? 1.0 / max : 1.0;
    }
    invmax = 1.0;
    /* Now normalize to the max */
    int j = 0;
    for (int i = 0; i < NLAMBDA; i += inc) {
        spec[j] = trans ? std::pow(10, -spec_in[i]) : spec_in[i] * invmax;
        XARR[j] = XARRAY[i];
        YARR[j] = YARRAY[i];
        ZARR[j] = ZARRAY[i];
        j++;
    }

    /* Now, sum up the CIE arrays multiplied by the spectra */
    int nspec = NLAMBDA / inc;
    double XSUM  = std::accumulate(XARR, XARR+nspec, 0);
    double YSUM  = std::accumulate(YARR, YARR+nspec, 0);
    double ZSUM  = std::accumulate(ZARR, ZARR+nspec, 0);
    double XPROD = std::inner_product(XARR, XARR+nspec, spec, 0);
    double YPROD = std::inner_product(YARR, YARR+nspec, spec, 0);
    double ZPROD = std::inner_product(ZARR, ZARR+nspec, spec, 0);

    /* Place the above results into the XYZ array */
    double XYZ[3] = { XPROD / XSUM, YPROD / YSUM, ZPROD / ZSUM };

    /* Transform to RGB by multiplying by the CIE matrix */
    *r = CIE_Mat[0][0] * XYZ[0] + CIE_Mat[0][1] * XYZ[1] + CIE_Mat[0][2] * XYZ[2];
    *g = CIE_Mat[1][0] * XYZ[0] + CIE_Mat[1][1] * XYZ[1] + CIE_Mat[1][2] * XYZ[2];
    *b = CIE_Mat[2][0] * XYZ[0] + CIE_Mat[2][1] * XYZ[1] + CIE_Mat[2][2] * XYZ[2];

    /* One more transformation */
    double inv = 1.0 / 2.4;
    if (*r > 0.0031308)
        *r = 1.055 * std::pow(*r, inv) - 0.055;
    else
        *r *= 12.92;
    if (*g > 0.0031308)
        *g = 1.055 * std::pow(*g, inv) - 0.055;
    else
        *g *= 12.92;
    if (*b > 0.0031308)
        *b = 1.055 * std::pow(*b, inv) - 0.055;
    else
        *b *= 12.92;

    /* Make sure the values are between 0 and 1 */
    if (*r < 0)
        *r = 0;
    else if (*r > 1)
        *r = 1;
    if (*g < 0)
        *g = 0;
    else if (*g > 1)
        *g = 1;
    if (*b < 0)
        *b = 0;
    else if (*b > 1)
        *b = 1;

}

/* Convert RGB to HSV.  h [0, 360], s and v [0, 1] */
void RGB_to_HSV(const double r,
                const double g,
                const double b,
                double *h,
                double *s,
                double *v) {

    double min = std::min(std::min(r, g), b);
    double max = std::max(std::max(r, g), b);
    double delta = max - min;

    /* The "value", or brightness */
    *v = max;

    /* The "saturation" */
    if (max != 0)
        *s = delta / max;
    else {
        /* In this case, "hue" is undefined.  Default to 0 */
        *s = 0;
        *h = 0;
        return;
    }

    /* The "hue" */
    if (r == max)
        *h = ( g - b ) / delta;
    else if (g == max)
        *h = 2 + ( b - r ) / delta;
    else
        *h = 4 + ( r - g ) / delta;

    /* Convert to degrees between 0 and 360 */
    *h *= 60;
    if (*h < 0)
        *h += 360;

}

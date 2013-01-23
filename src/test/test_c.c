#include "npspec/npspec.h"
#include <assert.h>
#include <math.h>

int main() {

    double qext[NLAMBDA], qscat[NLAMBDA], qabs[NLAMBDA];
    const double radius[2] = { 20.0, -1.0 };
    int index[1];
    const double relative_radius[1][2] = { { 1.0, 1.0 } };
    enum ErrorCode result;
    double r, g, b, h, s, v;

    index[0] = material_index("Ag");

    /* Sanity tests */
    assert(800 == NLAMBDA);
    assert(200.0 == wavelengths[0]);
    assert(999.0 == wavelengths[799]);
    assert(4 ==  material_index("Au"));
    assert(UnknownMaterial == material_index("Kryptonite"));

    /* Do a quick calculate to verify it works */
    result = npspec(1, radius, relative_radius, index, 1.0, false, 1, 1.0, 1.0,
                    Efficiency, qext, qscat, qabs);

    assert(NoError == result);
    assert(fabs(2.2285778886683643 - qext[0]) < 1e-14);
    assert(fabs(0.3062750316703077 - qscat[0]) < 1e-14);
    assert(fabs(1.9223028569980565 - qabs[0]) < 1e-14);

    RGB(qabs, 1, false, &r, &g, &b);
    assert(fabs(0.12381521 - r) < 1e-7);
    assert(fabs(0.10589667 - g) < 1e-7);
    assert(fabs(0.48082513 - b) < 1e-7);
    RGB_to_HSV(r, g, b, &h, &s, &v);
    assert(fabs(242.86751  - h) < 1e-4);
    assert(fabs(0.77976054 - s) < 1e-7);
    assert(fabs(0.48082513 - v) < 1e-7);

    return 0;
}

#include <stdio.h>
#include "NPSolve.h"

#define NLAYERS 2
#define MEDIUMDIE 1.0
#define FALSE 0

int main(int argc, char *argv[])
{
    
    // Define the system
    double rel_rad_spheroid[NLAYERS][2] = { { 0.8, 0.8 },
                                            { 0.2, 0.2 }
                                            };
    double rad[2] = { 10.0, 10.0 };

    // Prep the material index
    initiallize_material_index();

    // Main solver test
    int i, indx[NLAYERS] = { material_index("Ag"), material_index("Au") };
    double qext[NLAMBDA], qscat[NLAMBDA], qabs[NLAMBDA];

    npsolve(NLAYERS, rad, rel_rad_spheroid, indx, MEDIUMDIE, FALSE, FALSE,
            1.0, 1.0, Efficiency, qext, qscat, qabs);
    printf("\n\nTHIS IS CTEST\n\n");
    for (i = 0; i < 20; i++) {
        printf("NPSolve: Ext %.16f, Sca %.16f, Abs %.16f\n", qext[i], qscat[i], qabs[i]);
    }

    return 0;
}

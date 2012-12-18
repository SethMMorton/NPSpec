#include "NPSolve.h"

#include <string>
#include <map>

static std::map<std::string,int> matindx;

void initiallize_material_index() {

    /* The array defining the known materials  */
    static const char mindx[][14] = {
        "Ag",            /* 0 */
        "Al",            /* 1 */
        "AlAs",          /* 2 */
        "AlSb",          /* 3 */
        "Au",            /* 4 */
        "Be",            /* 5 */
        "CdS",           /* 6 */
        "CdSe",          /* 7 */
        "Co",            /* 8 */
        "Cr",            /* 9 */
        "Cu",            /* 10 */
        "Cu2O",          /* 11 */
        "CuO",           /* 12 */
        "Diamond",       /* 13 */
        "Diamond_film",  /* 14 */
        "GaAs",          /* 15 */
        "GaP",           /* 16 */
        "Ge",            /* 17 */
        "Glass",         /* 18 */
        "Graphite",      /* 19 */
        "InAs",          /* 20 */
        "InP",           /* 21 */
        "InSb",          /* 22 */
        "Ir",            /* 23 */
        "K",             /* 24 */
        "Li",            /* 25 */
        "Mo",            /* 26 */
        "Na",            /* 27 */
        "Nb",            /* 28 */
        "Ni",            /* 29 */
        "Os",            /* 30 */
        "PbS",           /* 31 */
        "PbSe",          /* 32 */
        "PbTe",          /* 33 */
        "Pd",            /* 34 */
        "Pt",            /* 35 */
        "Quartz",        /* 36 */
        "Rh",            /* 37 */
        "Si",            /* 38 */
        "SiC",           /* 39 */
        "SiO",           /* 40 */
        "Ta",            /* 41 */
        "Te",            /* 42 */
        "TiO2",          /* 43 */
        "V",             /* 44 */
        "W",             /* 45 */
        "ZnS",           /* 46 */
        "ZnSe",          /* 47 */
        "ZnTe",          /* 48 */
    };
    
    /* Initiallize the map */
    int mindxsize = sizeof mindx / sizeof mindx[0];
    for (int i = 0; i < mindxsize; i++) {
        matindx.insert( std::pair<std::string,int>(mindx[i], i) );
    }

}

int material_index(char *material) {
    /* If the material is not in the list, return -1 */ 
    std::string mat(material);
    if (matindx.count(mat) > 0)
        return matindx[mat];
    else
        return -1;
}

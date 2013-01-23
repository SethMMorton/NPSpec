#include "npspec/private/material_parameters.hpp"

/* The array defining the known materials  
 * Drude parameters are taken from:
 *      Rakic et. al, Appl. Opt., 1998, 37(22), 5271
 * Fermi velocities are taken from:
 *      http://hyperphysics.phy-astr.gsu.edu/hbase/tables/fermi.html
 *      Bettac et. al, Surf. Sci., 1998, 402-404, 475
 *      Dye et. al, Phys. Rev. B, 1981, 23(2), 462
 * */

const double drude_parameters[][3] = {
    { 9.01, 0.048, 1.39E6 },   /* Ag */
    { 14.98, 0.047, 2.03E6 },  /* Al */
    { 0.0, 0.0, 0.0 },         /* AlAs */
    { 0.0, 0.0, 0.0 },         /* AlSb */
    { 9.03, 0.053, 1.40E6 },   /* Au */
    { 18.51, 0.035, 2.25E6 },  /* Be */
    { 0.0, 0.0, 0.0 },         /* CdS */
    { 0.0, 0.0, 0.0 },         /* CdSe */
    { 0.0, 0.0, 0.0 },         /* Co */
    { 10.75, 0.047, 0.0 },     /* Cr */
    { 10.83, 0.030, 1.57E6 },  /* Cu */
    { 0.0, 0.0, 0.0 },         /* Cu2O */
    { 0.0, 0.0, 0.0 },         /* CuO */
    { 0.0, 0.0, 0.0 },         /* Diamond */
    { 0.0, 0.0, 0.0 },         /* Diamond_film */
    { 0.0, 0.0, 0.0 },         /* GaAs */
    { 0.0, 0.0, 0.0 },         /* GaP */
    { 0.0, 0.0, 0.0 },         /* Ge */
    { 0.0, 0.0, 0.0 },         /* Glass */
    { 0.0, 0.0, 0.0 },         /* Graphite */
    { 0.0, 0.0, 0.0 },         /* InAs */
    { 0.0, 0.0, 0.0 },         /* InP */
    { 0.0, 0.0, 0.0 },         /* InSb */
    { 0.0, 0.0, 0.0 },         /* Ir */
    { 0.0, 0.0, 0.86E6 },      /* K */
    { 0.0, 0.0, 1.29E6 },      /* Li */
    { 0.0, 0.0, 0.0 },         /* Mo */
    { 0.0, 0.0, 1.07E6 },      /* Na */
    { 0.0, 0.0, 1.37E6 },      /* Nb */
    { 15.92, 0.048, 0.0 },     /* Ni */
    { 0.0, 0.0, 0.0 },         /* Os */
    { 0.0, 0.0, 0.0 },         /* PbS */
    { 0.0, 0.0, 0.0 },         /* PbSe */
    { 0.0, 0.0, 0.0 },         /* PbTe */
    { 9.72, 0.008, 0.58E6 },   /* Pd */
    { 9.59, 0.080, 1.1E6 },    /* Pt */
    { 0.0, 0.0, 0.0 },         /* Quartz */
    { 0.0, 0.0, 0.0 },         /* Rh */
    { 0.0, 0.0, 0.0 },         /* Si */
    { 0.0, 0.0, 0.0 },         /* SiC */
    { 0.0, 0.0, 0.0 },         /* SiO */
    { 0.0, 0.0, 0.0 },         /* Ta */
    { 0.0, 0.0, 0.0 },         /* Te */
    { 0.0, 0.0, 0.0 },         /* TiO2 */
    { 0.0, 0.0, 0.0 },         /* V */
    { 13.22, 0.064, 0.0 },     /* W */
    { 0.0, 0.0, 0.0 },         /* ZnS */
    { 0.0, 0.0, 0.0 },         /* ZnSe */
    { 0.0, 0.0, 0.0 },         /* ZnTe */
};

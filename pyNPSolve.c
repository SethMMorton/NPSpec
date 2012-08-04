/* This module provides an interface for python to access npsolve */

#include <stdio.h>
#include <Python.h>
#include <numpy/arrayobject.h>
#include "NPSolve.h"

typedef int BOOL;

/* Wrapper for initiallize_material_index */
static PyObject* npsolve_initiallize_material_index (PyObject *self) {
    initiallize_material_index();
    Py_RETURN_NONE;
}

/* Wrapper for material_index */
static PyObject* npsolve_material_index (PyObject *self, PyObject *arg) {

    /* Convert to a C string */
    char* material;
    if (!PyArg_ParseTuple(arg, "s", &material))
        return NULL;
    /* Calculate the index, convert to a python object, then return */
    return PyInt_FromLong((long) material_index(material));
}

/* Wrapper for the actual npsolve routine */
static PyObject* npsolve_npsolve (PyObject *self, PyObject *args) {
    int nlayers, spectra_type;
    double mrefrac, path_length, concentration;
    BOOL size_correct, coarse;
    PyObject *rad_in, *rel_rad_in, *indx_in;
    PyArrayObject *rad, *rel_rad, *indx;

    /* Read in the input parameters */
    if (!PyArg_ParseTuple(args, "iOOOdiiddi",
                          &nlayers, &rad_in, &rel_rad_in,
                          &indx_in, &mrefrac, &size_correct, &coarse,
                          &path_length, &concentration, &spectra_type))
    {
        return NULL;
    }

    
    /* Ensure the python objects are numpy arrays */
    rad = (PyArrayObject*) PyArray_ContiguousFromAny(rad_in, NPY_DOUBLE, 1, 1);
    if (rad == NULL)
        return NULL;
    rel_rad = (PyArrayObject*) PyArray_ContiguousFromAny(rel_rad_in, NPY_DOUBLE, 2, 2);
    if (rel_rad == NULL)
        return NULL;
    indx = (PyArrayObject*) PyArray_ContiguousFromAny(indx_in, NPY_INT, 1, 1);
    if (indx == NULL)
        return NULL;

    /* Verify that the size of rad, rel_rad, and indx is correct */
    if (rad->nd != 1 || rad->dimensions[0] != 2) {
        PyErr_SetString(PyExc_ValueError, 
                        "rad must be one-dimensional of length 2");
        return NULL;
    }
    if (rel_rad->nd != 2 || 
        rel_rad->dimensions[0] != nlayers ||
        rel_rad->dimensions[1] != 2) {
        PyErr_SetString(PyExc_ValueError, 
                        "rel_rad must be two-dimensional of length nlayers x 2");
        return NULL;
    }
    if (indx->nd != 1 || indx->dimensions[0] != nlayers) {
        PyErr_SetString(PyExc_ValueError, 
                        "indx must be one-dimensional of length nlayers");
        return NULL;
    }

    /* Prep the result data */
    npy_intp dims[1] = { 800 };
    PyArrayObject *qext, *qscat, *qabs;
    qext  = (PyArrayObject *) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    qscat = (PyArrayObject *) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    qabs  = (PyArrayObject *) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);

    /* Call the actual routine */
    int retcode = npsolve(nlayers, (double*) rad->data, (double (*)[2]) rel_rad->data,
                         (int *) indx->data, mrefrac, size_correct, coarse,
                         path_length, concentration, spectra_type,
                         (double*) qext->data, (double*) qscat->data,
                         (double*) qabs->data);

    /* Die on error */
    if (retcode == 1) {
        PyErr_SetString(PyExc_ValueError,
                        "Product of size param & ref index too large");
        return NULL;
    } else if (retcode == 2) {
        PyErr_SetString(PyExc_ValueError,
                        "Too many layers for quasistatic approx");
        return NULL;
    }

    /* Build a tuple of the results and return */
    PyObject *arrays = PyTuple_New(3);
    PyTuple_SetItem(arrays, 0, PyArray_Return(qext));
    PyTuple_SetItem(arrays, 1, PyArray_Return(qscat));
    PyTuple_SetItem(arrays, 2, PyArray_Return(qabs));
    return arrays;
}

/* An array of the methods contained within */
static PyMethodDef npsolve_funcs[] = {
    { "initiallize_material_index",
      (PyCFunction) npsolve_initiallize_material_index,
      METH_NOARGS, "Initiallizes the map used to locate materials" },
    { "material_index", (PyCFunction) npsolve_material_index,
      METH_VARARGS, "The corresponding index of a given material is returned" },
    { "npsolve", (PyCFunction) npsolve_npsolve,
      METH_VARARGS, "Given a nanoparticle description, its spectrum will be calculated" },
    { NULL, NULL, 0, NULL }
};


/* The initiallization routine */
PyMODINIT_FUNC initnpsolve(void) {
    PyObject *module = Py_InitModule3("npsolve", npsolve_funcs,
                                      "A python wrapper for the NPSolve C library");
    import_array();

    /* Define some python objects to be in the module */
    PyObject *a = PyInt_FromLong((long) NLAMBDA);
    npy_intp dims[1] = { NLAMBDA };
    PyArrayObject *b = (PyArrayObject*) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    PyArrayObject *c = (PyArrayObject*) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    PyArrayObject *d = (PyArrayObject*) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    PyArrayObject *e = (PyArrayObject*) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    PyArrayObject *f = (PyArrayObject*) PyArray_EMPTY(1, dims, NPY_DOUBLE, 0);
    npy_intp dims2[2] = { 3, 3 };
    PyArrayObject *g = (PyArrayObject*) PyArray_EMPTY(2, dims2, NPY_DOUBLE, 0);

    /* Add the data to these objects. */
    b->data = (char*) wavelengths;
    c->data = (char*) CIE_X;
    d->data = (char*) CIE_Y;
    e->data = (char*) CIE_Z;
    f->data = (char*) CIE_D65;
    g->data = (char (*)[3]) CIE_Mat;

    /* Some enums */
    PyObject *h = PyInt_FromLong((long) Efficiency);
    PyObject *i = PyInt_FromLong((long) CrossSection);
    PyObject *j = PyInt_FromLong((long) Molar);
    PyObject *k = PyInt_FromLong((long) Absorbance);

    /* Place these objects in the module namespace */
    PyObject_SetAttrString(module, "NLAMBDA", a);
    PyObject_SetAttrString(module, "wavelengths", (PyObject*) b);
    PyObject_SetAttrString(module, "CIE_X", (PyObject*) c);
    PyObject_SetAttrString(module, "CIE_Y", (PyObject*) d);
    PyObject_SetAttrString(module, "CIE_Z", (PyObject*) e);
    PyObject_SetAttrString(module, "CIE_D65", (PyObject*) f);
    PyObject_SetAttrString(module, "CIE_Mat", (PyObject*) g);
    PyObject_SetAttrString(module, "Efficiency", h);
    PyObject_SetAttrString(module, "CrossSection", i);
    PyObject_SetAttrString(module, "Molar", j);
    PyObject_SetAttrString(module, "Absorbance", k);

}

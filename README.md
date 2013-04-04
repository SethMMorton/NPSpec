Welcome to the NPSpec documentation! Thank you for using NPSpec!
================================================================

The NPSpec library aids in solving the optical properties of nanoparticles. 

Useful links related to NPSpec
------------------------------

- [API Documentation](https://SethMMorton.github.com/NPSpec)
- [Source code and packages](https://github.com/SethMMorton/NPSpec)
- [NPSpec Theory](\ref theory)

NPSpec Bindings
---------------

NPSpec provides interfaces for four languages: C, C++, Fortran,
and Python.  Below are links to the sections of the documentation that you
will find useful for each language.

- C++

  There is a high-level interface to the NPSpec library 
  written in C++ provided by the [Nanoparticle](\ref nanoparticle.hpp) header.
  You do not need to include any other (NPSpec) headers when using this class.

- Python

  The Python library is a direct wrapping of the C++ library. 
  Look at the [Nanoparticle](\ref Nanoparticle) class documentation for the 
  Python API.  Any differences between the C++ and Python API will be given
  there.  Or, after loading the library using `import npspec` you can take a
  look at the available methods and variables with the command `dir(npspec)`.

- C

  When using C, you must interface with the lower-level functions of the NPSpec
  library; these are provided in the [NPSpec](\ref npspec.h) header. 

- Fortran

  The low-level C functions of the NPSpec module are wrapped directly using
  the new [iso_c_binding](http://publib.boulder.ibm.com/infocenter/lnxpcomp/v8v101/index.jsp?topic=%2Fcom.ibm.xlf101l.doc%2Fxlflr%2Finterop-iso-c-binding-module.htm) Fortran module which requires
  a fairly modern compiler.
  You can find the API in the [NPSpecModule](\ref npspecmodule) module. 

Authors
-------

Seth M. Morton, Lasse Jensen

ChangeLog
---------

- 0.2.0
  * Added C++ API
  * Added python API
  * Added unit tests for all APIs
  * Made CMake build system more robust

TODO
----

- Make color calculation code more robust
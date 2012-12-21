Project(FortranSOLVERS Fortran)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Make sure that Fortran recognizes iso_c_binding
INCLUDE(${CMAKE_MODULE_PATH}/CheckFortranSupportsIsoCBinding.cmake)

SET(FSOLVE fSolve) 
SET(FSOLVE_SRC quasi.f90 mie.f90)
ADD_LIBRARY(${FSOLVE} STATIC ${FSOLVE_SRC})

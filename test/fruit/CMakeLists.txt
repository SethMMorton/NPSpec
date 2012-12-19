Project(FRUIT Fortran)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Make sure that Fortran recognizes iso_c_binding
INCLUDE(${CMAKE_MODULE_PATH}/CheckFortranSupportsIsoCBinding.cmake)

SET(FRUIT Fruit) 
SET(FRUIT_SRC fruit.f90 fruit_util.f90)
ADD_LIBRARY(${FRUIT} STATIC ${FRUIT_SRC})

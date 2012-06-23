# CMAKE file for the NPSim project

####################
# SET UP ENVIRONMENT
####################

PROJECT(NPSolve CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

##############################################
# DEFINE THE FILES AND FOLDERS IN THIS PACKAGE
##############################################

# The source and include directories
SET(SRC src)
SET(INC include)

# Directories containing include files
INCLUDE_DIRECTORIES(${INC})

###########################################
# DEFINE HOW TO COMPILE THE NPSOLVE LIBRARY
###########################################

# Name of NPSolve library
SET(NPSOLVESHARED  NPSolve)
SET(NPSOLVESTATIC sNPSolve)

# Add the headers and source for the NPSolve library
SET(NPSolve_HEADERS ${INC}/NPSolve.h
                    ${INC}/solvers.h
)

SET(NPSolve_SRC ${SRC}/npsolve.cpp
                ${SRC}/drude_parameters.cpp
                ${SRC}/experimental_dielectrics.cpp
                ${SRC}/mie.cpp
                ${SRC}/material_index.cpp
                ${SRC}/quasi.cpp
                ${SRC}/standard_color_matching.cpp
                ${SRC}/wavelengths.cpp
)

# Compile NPSolve into a shared and static library
ADD_LIBRARY(${NPSOLVESHARED} SHARED ${NPSolve_SRC} ${NPSolve_HEADERS})
ADD_LIBRARY(${NPSOLVESTATIC} STATIC ${NPSolve_SRC} ${NPSolve_HEADERS})

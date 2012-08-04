# CMAKE file for the NPSim project

####################
# SET UP ENVIRONMENT
####################

PROJECT(NPSolve CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Default to static library
OPTION(STATIC    "Compile a static library" OFF)
OPTION(SHARED    "Compile a shared library" OFF)
OPTION(FRAMEWORK "Compile a library framework (Mac OS X only)" OFF)

# Choose the library type.
IF(STATIC AND SHARED)
    MESSAGE(FATAL "Cannot compile both STATIC and SHARED")
ELSEIF(STATIC AND FRAMEWORK)
    MESSAGE(FATAL "Cannot compile both STATIC and FRAMEWORK")
ELSEIF(SHARED AND FRAMEWORK)
    MESSAGE(FATAL "Cannot compile both SHARED and FRAMEWORK")
ELSEIF(FRAMEWORK)
    IF(NOT APPLE)
        MESSAGE(FATAL "FRAMEWORK only available on Mac OS X")
    ENDIF(NOT APPLE)
ELSE()
    # Default to static
    SET(STATIC ON)
ENDIF()

# Define the compiler flags
INCLUDE(${CMAKE_MODULE_PATH}/SetCXXFlags.cmake)

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
SET(NPSOLVE NPSolve)

# Add the headers and source for the NPSolve library
SET(NPSolve_HEADERS ${INC}/NPSolve.h
                    ${INC}/solvers.h
)

SET(NPSolve_SRC ${SRC}/npsolve.cpp
                ${SRC}/drude_parameters.cpp
                ${SRC}/mie.cpp
                ${SRC}/material_index.cpp
                ${SRC}/quasi.cpp
                ${SRC}/standard_color_matching.cpp
                ${SRC}/wavelengths.cpp
)
# All these get the compile flag -O2
SET_SOURCE_FILES_PROPERTIES(${NPSolve_SRC} PROPERTIES
                            COMPILE_FLAGS "-O2")

# The experimental dielectrics file takes too long with -O2,
# so we change this one only to -O0
SET_SOURCE_FILES_PROPERTIES(${SRC}/experimental_dielectrics.cpp PROPERTIES
                            COMPILE_FLAGS "-O0")
# Now add this file to the source list
SET(NPSolve_SRC ${NPSolve_SRC} ${SRC}/experimental_dielectrics.cpp)

# Define the library
IF(STATIC OR PYTHON)
    ADD_LIBRARY(${NPSOLVE} STATIC ${NPSolve_SRC} ${NPSolve_HEADERS})
ELSE()
    ADD_LIBRARY(${NPSOLVE} SHARED ${NPSolve_SRC} ${NPSolve_HEADERS})
ENDIF(STATIC OR PYTHON)

# These headers are to be installed with the library
# This automatically adds this header to the framework
SET_PROPERTY(SOURCE ${INC}/NPSolve.h PROPERTY PUBLIC_HEADER)
SET_PROPERTY(SOURCE ${INC}/NPSolveModule.f90 PROPERTY PUBLIC_HEADER)

# Define the framework properties and location if framework
IF(FRAMEWORK)

    # Set the version of the framework
    SET_TARGET_PROPERTIES(${NPSOLVE}
        PROPERTIES FRAMEWORK TRUE
        VERSION v0.1
    )

    # Install to framework location
    INSTALL(TARGETS ${NPSOLVE}
        FRAMEWORK DESTINATION /Library/Frameworks
    )

# Otherwise install to the usual locations
ELSE()

    # Determine the install location for libraries
    SET(LIBRARY_INSTALL_DIR lib)
    IF(EXISTS "${CMAKE_INSTALL_PREFIX}/lib32/" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(LIBRARY_INSTALL_DIR lib32)
    ELSEIF(EXISTS "${CMAKE_INSTALL_PREFIX}/lib64/" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(LIBRARY_INSTALL_DIR lib64)
    ENDIF()

    # Install library and header
    INSTALL(TARGETS ${NPSOLVE}
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION include
    ) 

ENDIF(FRAMEWORK)

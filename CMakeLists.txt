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
SET(NPSOLVE NPSolve)

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

# Define the library
ADD_LIBRARY(${NPSOLVE} SHARED ${NPSolve_SRC} ${NPSolve_HEADERS})
# This header is to be installed with the library
# This automatically adds this header to the framework
SET_PROPERTY(SOURCE ${INC}/NPSolve.h PROPERTY PUBLIC_HEADER)

# Make a framework on Mac, otherwise just a shared library
IF(APPLE)

    # Set the version of the framework
    SET_TARGET_PROPERTIES(${NPSOLVE}
        PROPERTIES FRAMEWORK TRUE
        VERSION v0.1
    )

    # Install to framework location
    INSTALL(TARGETS ${NPSOLVE}
        FRAMEWORK DESTINATION /Library/Frameworks
    )

ELSE()

    # Determine the install library
    SET(LIBRARY_INSTALL_DIR lib)
    IF(EXISTS "${CMAKE_INSTALL_PREFIX}/lib32/" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(LIBRARY_INSTALL_DIR lib32)
    ELSEIF(EXISTS "${CMAKE_INSTALL_PREFIX}/lib64/" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(LIBRARY_INSTALL_DIR lib64)
    ENDIF()

    # Install library and header
    INSTALL(TARGETS ${NPSOLVE}
        LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
        PUBLIC_HEADER DESTINATION include
    ) 

ENDIF(APPLE)

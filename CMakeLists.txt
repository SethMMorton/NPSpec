# CMAKE file for the NPSim project

####################
# SET UP ENVIRONMENT
####################

PROJECT(NPSolve CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Choose the library type
IF(STATIC AND SHARED)
    MESSAGE(FATAL "Cannot define both STATIC and SHARED")
ELSEIF(STATIC AND FRAMEWORK)
    MESSAGE(FATAL "Cannot define both STATIC and FRAMEWORK")
ELSEIF(FRAMEWORK AND SHARED)
    MESSAGE(FATAL "Cannot define both FRAMEWORK and SHARED")
ELSEIF(STATIC)
    SET(STATIC 1 CACHE BOOL "Make static library" FORCE)
    SET(SHARED 0 CACHE BOOL "Make shared library" FORCE)
    SET(FRAMEWORK 0 CACHE BOOL "Make framework" FORCE)
ELSEIF(SHARED)
    SET(SHARED 1 CACHE BOOL "Make shared library" FORCE)
    SET(STATIC 0 CACHE BOOL "Make static library" FORCE)
    SET(FRAMEWORK 0 CACHE BOOL "Make framework" FORCE)
ELSEIF(FRAMEWORK)
    IF(NOT APPLE)
        MESSAGE(FATAL "FRAMEWORK only available on Mac OS X")
    ENDIF(NOT APPLE)
    SET(FRAMEWORK 1 CACHE BOOL "Make framework" FORCE)
    SET(STATIC 0 CACHE BOOL "Make static library" FORCE)
    SET(SHARED 0 CACHE BOOL "Make shared library" FORCE)
ELSE()
    MESSAGE(STATUS "No library type defined, assuming STATIC")
    SET(STATIC 1 CACHE BOOL "Make static library" FORCE)
    SET(SHARED 0 CACHE BOOL "Make shared library" FORCE)
    SET(FRAMEWORK 0 CACHE BOOL "Make framework" FORCE)
ENDIF()

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
IF(STATIC)
    ADD_LIBRARY(${NPSOLVE} STATIC ${NPSolve_SRC} ${NPSolve_HEADERS})
ELSE()
    ADD_LIBRARY(${NPSOLVE} SHARED ${NPSolve_SRC} ${NPSolve_HEADERS})
ENDIF(STATIC)

# This header is to be installed with the library
# This automatically adds this header to the framework
SET_PROPERTY(SOURCE ${INC}/NPSolve.h PROPERTY PUBLIC_HEADER)

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

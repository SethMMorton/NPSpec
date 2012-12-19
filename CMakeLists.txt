# Set up environment
PROJECT(NPSolve CXX)
SET(NPSOLVE_VERSION 0.2.0)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# Default to static library
OPTION(STATIC    "Compile a static library" OFF)
OPTION(SHARED    "Compile a shared library" OFF)
OPTION(FRAMEWORK "Compile a library framework (Mac OS X only)" OFF)

# Tests?
OPTION(TESTING   "Compile testing code" OFF)
OPTION(FORTRAN   "Test Fortran code"    OFF)

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

# The source and include directories
SET(SRC  ${PROJECT_SOURCE_DIR}/src)
SET(INC  ${PROJECT_SOURCE_DIR}/include)
SET(LIB  ${PROJECT_SOURCE_DIR}/lib)
SET(TEST ${PROJECT_SOURCE_DIR}/test)

# Name of NPSolve library
SET(NPSOLVE NPSolve)

# Build the NPSolve library and place it into lib
ADD_SUBDIRECTORY(${SRC} ${LIB})
#ADD_SUBDIRECTORY(${INC})

# Unit testing (only if requested)
IF (TESTING)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${TEST})
ENDIF(TESTING)

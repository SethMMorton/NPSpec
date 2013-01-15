# Set up environment
PROJECT(NPSpec CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

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
SET(SRC  ${CMAKE_SOURCE_DIR}/src)
SET(INC  ${CMAKE_SOURCE_DIR}/include)
SET(LIB  ${CMAKE_SOURCE_DIR}/lib)
SET(TEST ${CMAKE_SOURCE_DIR}/test)

SET(NPSRC ${SRC}/npspec)
SET(NPINC ${INC}/npspec)

# Name of NPSpec library
SET(NPSPEC NPSpec)

# Build the NPSpec library and place it into lib
ADD_SUBDIRECTORY(${NPSRC} ${LIB})

# Unit testing (only if requested)
IF(TESTING)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${SRC}/test ${TEST})
ENDIF(TESTING)

# Add a distclean target to the Makefile
ADD_CUSTOM_TARGET(distclean 
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/distclean.cmake
)

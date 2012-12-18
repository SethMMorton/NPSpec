# CMAKE file for the NPSim project

####################
# SET UP ENVIRONMENT
####################

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
        VERSION v${NPSOLVE_VERSION}
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

####################
# Unit testing setup
####################

IF (TESTING)

    ENABLE_TESTING()
    INCLUDE(ExternalProject)

    # Set default ExternalProject root directory
    SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

    # Install gtest as part of this project
    ExternalProject_Add(
        googletest
        SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
        TIMEOUT 10
        # Force separate output paths for debug and release builds to allow easy
        # identification of correct lib in subsequent TARGET_LINK_LIBRARIES
        # commands
        CMAKE_ARGS -Dgtest_force_shared_crt=ON -DBUILD_SHARED_LIBS=ON
        # Disable install step
        INSTALL_COMMAND ""
        # Wrap download, configure and build steps in a script to log output
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
    )

    # Specify include dir
    ExternalProject_Get_Property(googletest source_dir)
    INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${source_dir}/include)

    # Specify link dir
    ExternalProject_Get_Property(googletest binary_dir)
    LINK_DIRECTORIES(${INCLUDE_DIRECTORIES} ${binary_dir})

    # Add test executable target
    ADD_EXECUTABLE(testNPSolve ${PROJECT_SOURCE_DIR}/test/test_npsolve.cpp)

    # Create dependency of testNPSolve on googletest
    ADD_DEPENDENCIES(testNPSolve googletest)

    # Specify testNPSolve's link libraries
    TARGET_LINK_LIBRARIES(testNPSolve gtest NPSolve)

    # Add the c++ only unit tests
    ADD_TEST(testNPSolve "testNPSolve")

    # Test Fortran code?
    IF (FORTRAN)
        ENABLE_LANGUAGE(Fortran)
        # Make sure that Fortran recognizes iso_c_binding
        INCLUDE(${CMAKE_MODULE_PATH}/CheckFortranSupportsIsoCBinding.cmake)
        # The following function will properly link Fortran routines to C++
        INCLUDE(CMakeAddFortranSubdirectory)
        CMAKE_ADD_Fortran_SUBDIRECTORY(test/fsolve
            PROJECT FortranSOLVERS  # project name in toplevel CMakeLists.txt
            ARCHIVE_DIR test/fsolve # .lib location relative to root binary tree
            LIBRARIES fSolve # target libraries created
            NO_EXTERNAL_INSTALL TRUE # Don't install
        )
        # Create the executable and add depencencies, etc.
        ADD_EXECUTABLE(verifyNPSolve ${PROJECT_SOURCE_DIR}/test/verify_npsolve.cpp)
        ADD_DEPENDENCIES(verifyNPSolve googletest, fSolve)
        TARGET_LINK_LIBRARIES(verifyNPSolve gtest NPSolve fSolve)
        ADD_TEST(verifyNPSolve "verifyNPSolve")
    ENDIF(FORTRAN)

ENDIF(TESTING)

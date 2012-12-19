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
INCLUDE_DIRECTORIES(${INC} ${source_dir}/include)

# Specify link dir
ExternalProject_Get_Property(googletest binary_dir)
LINK_DIRECTORIES(${binary_dir})

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
    CMAKE_ADD_Fortran_SUBDIRECTORY(fsolve
        PROJECT FortranSOLVERS  # project name in toplevel CMakeLists.txt
        ARCHIVE_DIR fsolve # .lib location relative to root binary tree
        LIBRARIES fSolve # target libraries created
        NO_EXTERNAL_INSTALL TRUE # Don't install
    )
    # Create the executable and add depencencies, etc.
    ADD_EXECUTABLE(verifyNPSolve ${PROJECT_SOURCE_DIR}/test/verify_npsolve.cpp)
    ADD_DEPENDENCIES(verifyNPSolve googletest, fSolve)
    TARGET_LINK_LIBRARIES(verifyNPSolve gtest NPSolve fSolve)
    ADD_TEST(verifyNPSolve "verifyNPSolve")
ENDIF(FORTRAN)

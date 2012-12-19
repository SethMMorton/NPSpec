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
ADD_EXECUTABLE(testNPSolve test_npsolve.cpp)

# Create dependency of testNPSolve on googletest
ADD_DEPENDENCIES(testNPSolve googletest)

# Specify testNPSolve's link libraries
TARGET_LINK_LIBRARIES(testNPSolve gtest NPSolve)

# Add the c++ only unit tests
ADD_TEST(testNPSolve "testNPSolve")

# Test Fortran code?
IF (FORTRAN)
    # The following function will properly link Fortran routines to C++
    ENABLE_LANGUAGE(Fortran)
    INCLUDE(CMakeAddFortranSubdirectory)
    CMAKE_ADD_Fortran_SUBDIRECTORY(fsolve
        PROJECT FortranSOLVERS  # project name in toplevel CMakeLists.txt
        ARCHIVE_DIR fsolve # .lib location relative to root binary tree
        LIBRARIES fSolve # target libraries created
        NO_EXTERNAL_INSTALL TRUE # Don't install
    )
    # Create the executable for verifying C++ routines against Fortran standards
    ADD_EXECUTABLE(verifyNPSolve verify_npsolve.cpp)
    ADD_DEPENDENCIES(verifyNPSolve googletest fSolve NPSolve)
    TARGET_LINK_LIBRARIES(verifyNPSolve gtest NPSolve fSolve)
    ADD_TEST(verifyNPSolve "verifyNPSolve")
    # Create the executable that tests the Fortran wrappers of the C++ code
    #CMAKE_ADD_Fortran_SUBDIRECTORY(fruit
    #    PROJECT FRUIT ARCHIVE_DIR fruit LIBRARIES Fruit
    #    NO_EXTERNAL_INSTALL TRUE
    #)
    #ADD_EXECUTABLE(testFortranWrapper test_fortran_wrapper.f90)
    #ADD_DEPENDENCIES(testFortranWrapper Fruit NPSolve)
    #TARGET_LINK_LIBRARIES(testFortranWrapper Fruit NPSolve)
    #ADD_TEST(testFortranWrapper "testFortranWrapper")
ENDIF(FORTRAN)

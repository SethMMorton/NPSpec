# Add the googletest framework
ADD_SUBDIRECTORY(googletest ${LIB}/gtest)

# Specify include dir
#ExternalProject_Get_Property(googletest source_dir)
INCLUDE_DIRECTORIES(${INC} googletest/include)

# Specify link dir
#ExternalProject_Get_Property(googletest binary_dir)
LINK_DIRECTORIES(${LIB}/gtest)

# Add test executable target
ADD_EXECUTABLE(testNPSolve test_npsolve.cpp)

# Create dependency of testNPSolve on googletest
ADD_DEPENDENCIES(testNPSolve googletest)

# Specify testNPSolve's link libraries
TARGET_LINK_LIBRARIES(testNPSolve gtest NPSolve)

# Add OpenMP flags
IF(OPENMP_FOUND)
    IF("${OpenMP_CXX_FLAGS}" STREQUAL "-fopenmp")
        SET_TARGET_PROPERTIES (testNPSolve PROPERTIES
            COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
            LINK_FLAGS "-lgomp")
    ELSE()
        SET_TARGET_PROPERTIES (testNPSolve PROPERTIES
            COMPILE_FLAGS "${OpenMP_CXX_FLAGS}")
    ENDIF()
ENDIF(OPENMP_FOUND)

# Add the c++ only unit tests
ADD_TEST(testNPSolve "testNPSolve")

# Test Fortran code?
IF (FORTRAN)
    # The following function will properly link Fortran routines to C++
    ENABLE_LANGUAGE(Fortran)
    SET (CMAKE_Fortran_MODULE_DIRECTORY ${LIB}/modules)
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
    CMAKE_ADD_Fortran_SUBDIRECTORY(fruit
        PROJECT FRUIT ARCHIVE_DIR fruit LIBRARIES Fruit
        NO_EXTERNAL_INSTALL TRUE
    )
    ADD_EXECUTABLE(testFortranWrapper test_fortran_wrapper.f90
                                      ${INC}/NPSolveModule.f90)
    ADD_DEPENDENCIES(testFortranWrapper Fruit NPSolve)
    TARGET_LINK_LIBRARIES(testFortranWrapper Fruit NPSolve)
    ADD_TEST(testFortranWrapper "testFortranWrapper")
ENDIF(FORTRAN)

# Add the googletest framework
ADD_SUBDIRECTORY(googletest ${LIB}/gtest)

# Specify include dir
#ExternalProject_Get_Property(googletest source_dir)
INCLUDE_DIRECTORIES(${INC} googletest/include)

# Specify link dir
#ExternalProject_Get_Property(googletest binary_dir)
LINK_DIRECTORIES(${LIB}/gtest)

# Add test executable target
ADD_EXECUTABLE(testNPSpec test_npsolve.cpp)

# Create dependency of testNPSpec on googletest
ADD_DEPENDENCIES(testNPSpec googletest)

# Specify testNPSpec's link libraries
TARGET_LINK_LIBRARIES(testNPSpec gtest NPSpec)

# Add the c++ only unit tests
ADD_TEST(testNPSpec "testNPSpec")

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
    ADD_EXECUTABLE(verifyNPSpec verify_npsolve.cpp)
    ADD_DEPENDENCIES(verifyNPSpec googletest fSolve NPSpec)
    TARGET_LINK_LIBRARIES(verifyNPSpec gtest NPSpec fSolve)
    ADD_TEST(verifyNPSpec "verifyNPSpec")
    # Create the executable that tests the Fortran wrappers of the C++ code
    CMAKE_ADD_Fortran_SUBDIRECTORY(fruit
        PROJECT FRUIT ARCHIVE_DIR fruit LIBRARIES Fruit
        NO_EXTERNAL_INSTALL TRUE
    )
    ADD_EXECUTABLE(testFortranWrapper test_fortran_wrapper.f90
                                      ${INC}/NPSpecModule.f90)
    ADD_DEPENDENCIES(testFortranWrapper Fruit NPSpec)
    TARGET_LINK_LIBRARIES(testFortranWrapper Fruit NPSpec)
    ADD_TEST(testFortranWrapper "testFortranWrapper")
ENDIF(FORTRAN)

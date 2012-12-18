##########################################################
# Checks that your Fortran compiler supports ISO_C_BINDING
##########################################################

# There is no nice function to do this for FORTRAN, so we must manually
# create a test program and check if it compiles.
SET(TESTFILE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}")
SET(TESTFILE "${TESTFILE}/CMakeTmp/testFortranFlags.f90")
FILE (WRITE "${TESTFILE}"
"
program dummyprog
  use, intrinsic :: iso_c_binding
  integer(C_INT) :: n
  n = 5
end program dummyprog
")
TRY_COMPILE (SUPPORTS_ISO_C ${CMAKE_BINARY_DIR} ${TESTFILE})
            
IF(NOT SUPPORTS_ISO_C)
    MESSAGE(FATAL_ERROR "Your Fortran compiler does not support "
            "ISO_C_BINDING.  You need a more modern compiler")
ENDIF()

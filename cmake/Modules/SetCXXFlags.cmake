####################################################################
# Make sure that the default build type is RELEASE if not specified.
####################################################################

INCLUDE(${CMAKE_MODULE_PATH}/SetCompileFlag.cmake)

#########################################################
# If the compiler flags have already been set, return now
#########################################################

IF (CMAKE_CXX_FLAGS)
    RETURN ()
ENDIF (CMAKE_CXX_FLAGS)

# Set the compile flags.  First do release, then debug.
# This is done by checking each type of flag one at a time,
# finding the one that matches the current compiler.
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CXX REQUIRED "-fPIC")
#SET_COMPILE_FLAG(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CXX 
#                  "-Wall"     # GNU compilers
#                  "-warn all" # Intel compilers
#                 )

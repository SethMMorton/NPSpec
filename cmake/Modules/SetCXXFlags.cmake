#*************************************************
# Determine and set the CXX compiler flags we want
#*************************************************

####################################################################
# Make sure that the default build type is RELEASE if not specified.
####################################################################
INCLUDE(${CMAKE_MODULE_PATH}/SetCompileFlag.cmake)

# Make sure the build type is uppercase
STRING(TOUPPER "${CMAKE_BUILD_TYPE}" BT)

IF(BT STREQUAL "RELEASE")
    SET(CMAKE_BUILD_TYPE RELEASE CACHE STRING
      "Choose the type of build, options are DEBUG, RELEASE, or TESTING."
      FORCE)
ELSEIF(BT STREQUAL "DEBUG")
    SET (CMAKE_BUILD_TYPE DEBUG CACHE STRING
      "Choose the type of build, options are DEBUG, RELEASE, or TESTING."
      FORCE)
ELSEIF(NOT BT)
    SET(CMAKE_BUILD_TYPE RELEASE CACHE STRING
      "Choose the type of build, options are DEBUG or RELEASE."
      FORCE)
    MESSAGE(STATUS "CMAKE_BUILD_TYPE not given, defaulting to RELEASE")
ELSE()
    MESSAGE(FATAL_ERROR "CMAKE_BUILD_TYPE not valid, choices are DEBUG or RELEASE")
ENDIF(BT STREQUAL "RELEASE")

#########################################################
# If the compiler flags have already been set, return now
#########################################################

IF(FLAGS_SET)
    RETURN ()
ENDIF(FLAGS_SET)

########################################################################
# Determine the appropriate flags for this compiler for each build type.
# For each option type, a list of possible flags is given that work
# for various compilers.  The first flag that works is chosen.
# If none of the flags work, nothing is added (unless the REQUIRED 
# flag is given in the call).  This way unknown compiles are supported.
#######################################################################

#####################
### GENERAL FLAGS ###
#####################

# There is some bug where -march=native doesn't work on Mac
IF(APPLE)
    SET(GNUNATIVE "-mtune=native")
ELSE()
    SET(GNUNATIVE "-march=native")
ENDIF()

# Optimize for the host's architecture
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CXX
                 ${GNUNATIVE}    # GNU/Clang
                 "-xHost"        # Intel
                 "/QxHost"       # Intel Windows
                )

###################
### DEBUG FLAGS ###
###################

# NOTE: debugging symbols (-g or /debug:full) are already on by default

# Disable optimizations
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}"
                 CXX REQUIRED "-O0" # All compilers not on Windows
                              "/Od" # MSVC
                              "/Od" # Intel Windows
                )

# Turn on all warnings 
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}"
                 CXX "/Wall" # Intel Windows/MSVC
                     "-Wall" # GNU/Clang/Intel
                )

# Enable "Effective C++" warnings
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}"
                 CXX "-Weffc++" # GNU/Clang/Intel
                     "/Qeffc++" # Intel Windows
                )

#####################
### RELEASE FLAGS ###
#####################

# NOTE: agressive optimizations turned on by default

# Unroll loops
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}"
                 CXX "-funroll-loops" # GNU/Clang
                     "-unroll"        # Intel
                     "/unroll"        # Intel Windows
                )

# Remember that we were here
SET(FLAGS_SET TRUE CACHE INTERNAL "FLAGS SET")

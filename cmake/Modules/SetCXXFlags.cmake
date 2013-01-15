#*************************************************
# Determine and set the CXX compiler flags we want
#*************************************************

INCLUDE(${CMAKE_MODULE_PATH}/SetCompileFlag.cmake)

# If the compiler flags have already been set, return now
IF (CMAKE_CXX_FLAGS)
    RETURN ()
ENDIF (CMAKE_CXX_FLAGS)

##########################################################
# Set the compile flags. This is done by checking each
# type of flag one at a time, finding the one that matches
# the given compiler.
##########################################################

# The optimization level
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CXX REQUIRED "-O2")

# Position-independent code.  Required for libraries
SET_COMPILE_FLAG(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CXX REQUIRED "-fPIC")

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
                 "-ta=host"      # Portland Group
                )
# Debugging warnings
#SET_COMPILE_FLAG(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CXX 
#                  "-Wall"     # GNU/Clang
#                  "-warn all" # Intel
#                 )

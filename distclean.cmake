# This CMake script will delete build directories and files to bring the
# package back to it's distribution state

# We want to start from the top of the source dir, so if we are in build
# we want to start one directory up
GET_FILENAME_COMPONENT(BASEDIR ${CMAKE_SOURCE_DIR} NAME)
IF(${BASEDIR} STREQUAL "build")
    SET(TOPDIR "${CMAKE_SOURCE_DIR}/..")
ELSE()
    SET(TOPDIR "${CMAKE_SOURCE_DIR}")
ENDIF()

# Find directories and files that we will want to remove
FILE(GLOB_RECURSE EGGS "${TOPDIR}/*.egg-info")
FILE(GLOB_RECURSE PYCACHE "${TOPDIR}/*__pycache__*")
FILE(GLOB_RECURSE PYC "${TOPDIR}/*.pyc")
FILE(GLOB PYBUILDTEMP "${TOPDIR}/build/temp*")
FILE(GLOB PYBUILDLIB "${TOPDIR}/build/lib*")
FILE(GLOB_RECURSE SWIGWRAP "${TOPDIR}/*_wrap.cpp")
FILE(GLOB_RECURSE SWIGPY "${TOPDIR}/*npspec.py")
FILE(GLOB_RECURSE CMAKECACHE "${TOPDIR}/*CMakeCache.txt")
FILE(GLOB_RECURSE CMAKEINSTALL "${TOPDIR}/*cmake_install.cmake")
FILE(GLOB_RECURSE MAKEFILE "${TOPDIR}/*Makefile")
FILE(GLOB_RECURSE CMAKEFILES "${TOPDIR}/*CMakeFiles")
FILE(GLOB_RECURSE CMAKETESTFILES "${TOPDIR}/*CMakeTestfile.cmake")
FILE(GLOB TOPDIRECTORIES "${TOPDIR}/lib" 
                         "${TOPDIR}/dist" 
                         "${TOPDIR}/test"
)

# Place these files and directories into a list
SET(DEL ${TOPDIRECTORIES}
        ${EGGS}
        ${PYCACHE}
        ${PYC}
        ${PYBUILDTEMP}
        ${PYBUILDLIB}
        ${SWIGWRAP}
        ${SWIGPY}
        ${CMAKECACHE}
        ${CMAKEINSTALL}
        ${MAKEFILE}
        ${CMAKEFILES}
        ${CMAKETESTFILES}
)

# If we are not in the build dir, delete that as well
IF(NOT (${BASEDIR} STREQUAL "build"))
    FILE(GLOB BUILD "${TOPDIR}/build")
    SET(DEL ${DEL} ${BUILD})
ENDIF()

# Loop over the directories and delete each one
FOREACH(D ${DEL})
    IF(EXISTS ${D})
        FILE(REMOVE_RECURSE ${D})
    ENDIF()
ENDFOREACH()

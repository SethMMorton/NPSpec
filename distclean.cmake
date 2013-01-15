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

MACRO(GET_PARENT_DIRECTORIES search_string return_list grandparents)
    FILE(GLOB_RECURSE new_list ${search_string})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        # Remove an extra directory component to return grandparent
        IF(${grandparents})
            # Tack on a fake extension to trick CMake into removing a second
            # path component
            SET(dir_path "${dir_path}.tmp")
            GET_FILENAME_COMPONENT(dir_path ${dir_path} PATH)
        ENDIF(${grandparents})
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# Find directories and files that we will want to remove
FILE(GLOB EGGS "${TOPDIR}/*.egg-info" "${TOPDIR}/python/*.egg-info")
FILE(GLOB PYCACHE "${TOPDIR}/python/npspec/tests/__pycache__")
FILE(GLOB_RECURSE PYC "${TOPDIR}/*.pyc")
FILE(GLOB PYBUILDTEMP "${TOPDIR}/build/temp*")
FILE(GLOB PYBUILDLIB "${TOPDIR}/build/lib*")
FILE(GLOB PYBUILDBDIST "${TOPDIR}/build/bdist*")
FILE(GLOB_RECURSE CMAKECACHE "${TOPDIR}/*CMakeCache.txt")
FILE(GLOB_RECURSE CMAKEINSTALL "${TOPDIR}/*cmake_install.cmake")
FILE(GLOB_RECURSE MAKEFILE "${TOPDIR}/*Makefile")
FILE(GLOB_RECURSE CMAKETESTFILES "${TOPDIR}/*CTestTestfile.cmake")
FILE(GLOB TOPDIRECTORIES "${TOPDIR}/lib" 
                         "${TOPDIR}/dist" 
                         "${TOPDIR}/test"
)
# CMake has trouble finding directories recursively, so locate these
# files and then save the parent directory of the files
GET_PARENT_DIRECTORIES(Makefile.cmake CMAKEFILES 0)
GET_PARENT_DIRECTORIES(LastTest.log CMAKETESTING 1)

# Place these files and directories into a list
SET(DEL ${TOPDIRECTORIES}
        ${EGGS}
        ${PYCACHE}
        ${PYC}
        ${PYBUILDTEMP}
        ${PYBUILDLIB}
        ${PYBUILDBDIST}
        ${CMAKECACHE}
        ${CMAKEINSTALL}
        ${MAKEFILE}
        ${CMAKEFILES}
        ${CMAKETESTING}
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

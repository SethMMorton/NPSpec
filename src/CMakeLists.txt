# Directories containing include files
INCLUDE_DIRECTORIES(${INC})

SET(NPSolve_SRC npsolve.cpp
                drude_parameters.cpp
                experimental_dielectrics.cpp
                mie.cpp
                material_index.cpp
                quasi.cpp
                standard_color_matching.cpp
                wavelengths.cpp
)

# Define the library
IF(STATIC)
    ADD_LIBRARY(${NPSOLVE} STATIC ${NPSolve_SRC} ${INC}/NPSolve.h)
ELSE()
    ADD_LIBRARY(${NPSOLVE} SHARED ${NPSolve_SRC} ${INC}/NPSolve.h)
ENDIF(STATIC)

# These headers are to be installed with the library
IF(FRAMEWORK)
    SET_PROPERTY(SOURCE ${INC}/NPSolve.h 
        PROPERTY MACOSX_PACKAGE_LOCATION Headers)
ENDIF()

# Define the framework properties and location if framework
IF(FRAMEWORK)

    # Set the version of the framework
    SET_TARGET_PROPERTIES(${NPSOLVE} PROPERTIES FRAMEWORK TRUE
                                     VERSION v${VERSION})

    # Install to framework location
    SET(FRAMELIB /Library/Frameworks)
    SET(FRAMENAME ${NPSOLVE}.framework)
    INSTALL(TARGETS ${NPSOLVE} FRAMEWORK DESTINATION ${FRAMELIB})
    INSTALL(FILES ${INC}/NPSolveModule.f90 
        DESTINATION ${FRAMELIB}/${FRAMENAME}/Versions/v${VERSION}/Headers)

# Otherwise install to the usual locations
ELSE()

    # Determine the install location for libraries
    SET(LIBRARY_INSTALL_DIR lib)
    IF(EXISTS "${CMAKE_INSTALL_PREFIX}/lib32/" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(LIBRARY_INSTALL_DIR lib32)
    ELSEIF(EXISTS "${CMAKE_INSTALL_PREFIX}/lib64/" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(LIBRARY_INSTALL_DIR lib64)
    ENDIF()

    # Install library and header
    INSTALL(TARGETS ${NPSOLVE} LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
                               ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}) 
    INSTALL(FILES ${INC}/NPSolve.h ${INC}/NPSolveModule.f90 DESTINATION include)

ENDIF(FRAMEWORK)

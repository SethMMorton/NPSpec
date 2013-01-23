# Directories containing include files
INCLUDE_DIRECTORIES(${INC})

# All cpp files
SET(NPSpec_SRC npspec.cpp
               nanoparticle.cpp
               calculate_color.cpp
               drude_parameters.cpp
               experimental_dielectrics.cpp
               mie.cpp
               material_index.cpp
               quasi.cpp
               standard_color_matching.cpp
               wavelengths.cpp
)

# These are the headers to be public
SET(NPSpec_HEADERS ${NPINC}/npspec.h
                   ${NPINC}/constants.h
                   ${NPINC}/nanoparticle.hpp
                   ${NPINC}/version.h
)

# Define the library
IF(STATIC)
    ADD_LIBRARY(${NPSPEC} STATIC ${NPSpec_SRC} ${NPSpec_HEADERS})
ELSE()
    ADD_LIBRARY(${NPSPEC} SHARED ${NPSpec_SRC} ${NPSpec_HEADERS})
ENDIF(STATIC)

# These headers are to be installed with the library
IF(FRAMEWORK)
    SET_PROPERTY(SOURCE ${NPSpec_HEADERS}
        PROPERTY MACOSX_PACKAGE_LOCATION Headers/npspec)
ENDIF()

# Define the framework properties and location if framework
IF(FRAMEWORK)

    # Set the version of the framework
    SET_TARGET_PROPERTIES(${NPSPEC} PROPERTIES FRAMEWORK TRUE
                                     VERSION v${VERSION})

    # Install to framework location
    SET(FRAMELIB /Library/Frameworks)
    SET(FRAMENAME ${NPSPEC}.framework)
    INSTALL(TARGETS ${NPSPEC} FRAMEWORK DESTINATION ${FRAMELIB})
    INSTALL(FILES ${NPINC}/NPSpecModule.f90 
        DESTINATION ${FRAMELIB}/${FRAMENAME}/Versions/v${VERSION}/Headers/npspec)

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
    INSTALL(TARGETS ${NPSPEC} LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
                              ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}) 
    INSTALL(FILES ${NPSpec_HEADERS} ${NPINC}/NPSpecModule.f90 
            DESTINATION include/npspec)

ENDIF(FRAMEWORK)

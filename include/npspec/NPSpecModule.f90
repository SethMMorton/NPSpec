Module NPSpecModule

!   ***********************************************************************
!   This module conatins interfaces needed to call the NPSpec library from
!   Fortran, plus a helper function to make a Fortran string a C-string
!   ***********************************************************************

    use, intrinsic :: iso_c_binding

    Private

!   Parameters
    Public NLAMBDA
    Public Efficiency
    Public CrossSection
    Public Molar
    Public Absorption
    Public wavelengths

!   ErrorCodes
    Public NoError
    Public NanoparticleTooLarge
    Public SizeParameterWarning
    Public InvalidRadius
    Public InvalidRelativeRadius
    Public InvalidIncrement
    Public InvalidPathLength
    Public InvalidConcentration
    Public InvalidRefractiveIndex
    Public InvalidNumberOfLayers
    Public UnknownMaterial

!   Functions
    Public material_index
    Public npspec
    Public RGB
    Public RGB_to_HSV
    Public make_C_string

!   Here is the wavelength and color matching arrays from the NPSpec header
    Integer(C_INT), Parameter                    :: NLAMBDA = 800
    Real(C_DOUBLE), Bind(C, name="wavelengths")  :: wavelengths(NLAMBDA)

!   Had a hell of a time getting an enum to bind, so I am just redefining it here
    Integer(C_INT), Parameter :: Efficiency   = 0
    Integer(C_INT), Parameter :: CrossSection = 1
    Integer(C_INT), Parameter :: Molar        = 2
    Integer(C_INT), Parameter :: Absorption   = 3

!   ErrorCode enum
    Integer(C_INT), Parameter :: NoError                = 0
    Integer(C_INT), Parameter :: NanoparticleTooLarge   = -1
    Integer(C_INT), Parameter :: SizeParameterWarning   = -3
    Integer(C_INT), Parameter :: InvalidRadius          = -4
    Integer(C_INT), Parameter :: InvalidRelativeRadius  = -5
    Integer(C_INT), Parameter :: InvalidIncrement       = -6
    Integer(C_INT), Parameter :: InvalidPathLength      = -7
    Integer(C_INT), Parameter :: InvalidConcentration   = -8
    Integer(C_INT), Parameter :: InvalidRefractiveIndex = -9
    Integer(C_INT), Parameter :: InvalidNumberOfLayers  = -10
    Integer(C_INT), Parameter :: UnknownMaterial        = -11

!   Interfaces to the C routines
    Interface
        Integer(C_INT) Function material_index (material)  Bind (C)
            use, intrinsic :: iso_c_binding
            !Character(Kind=C_CHAR), Dimension(*), Intent(In) :: material
            Character(Kind=C_CHAR), Intent(In) :: material
        End Function material_index
    End Interface

    Interface
        Integer(C_INT) Function npspec (nlayers, rad, rel_rad, indx, mrefrac,   &
                            size_correct, increment, path_length, concentration, &
                            spectra_type, qext, qscat, qabs) Bind (C)
            use, intrinsic :: iso_c_binding
            Integer(C_INT),  Intent(In), Value  :: nlayers
            Real(C_DOUBLE),  Intent(In)         :: rad(2)
            Real(C_DOUBLE),  Intent(In)         :: rel_rad(nlayers,*)
            Integer(C_INT),  Intent(In)         :: indx(*)
            Real(C_DOUBLE),  Intent(In), Value  :: mrefrac
            Logical(C_BOOL), Intent(In), Value  :: size_correct
            Integer(C_INT),  Intent(In), Value  :: increment
            Real(C_DOUBLE),  Intent(In), Value  :: path_length
            Real(C_DOUBLE),  Intent(In), Value  :: concentration
            Integer(C_INT),  Intent(In), Value  :: spectra_type
            Real(C_DOUBLE),  Intent(Out)        :: qext(*)
            Real(C_DOUBLE),  Intent(Out)        :: qscat(*)
            Real(C_DOUBLE),  Intent(Out)        :: qabs(*)
        End Function npspec
    End Interface

    Interface
        Subroutine RGB (spec_in, inc, trans, r, g, b, o) Bind(C)
            use, intrinsic :: iso_c_binding
            Real(C_DOUBLE),  Intent(In)         :: spec_in(*)
            Integer(C_INT),  Intent(In),  Value :: inc
            Logical(C_BOOL), Intent(In),  Value :: trans
            Real(C_DOUBLE),  Intent(Out)        :: r
            Real(C_DOUBLE),  Intent(Out)        :: g
            Real(C_DOUBLE),  Intent(Out)        :: b
            Real(C_DOUBLE),  Intent(Out)        :: o
        End Subroutine RGB
    End Interface

    Interface
        Subroutine HSV_to_RGB (r, g, b, h, s, v) Bind(C)
            use, intrinsic :: iso_c_binding
            Real(C_DOUBLE),  Intent(In),  Value :: r
            Real(C_DOUBLE),  Intent(In),  Value :: g
            Real(C_DOUBLE),  Intent(In),  Value :: b
            Real(C_DOUBLE),  Intent(Out)        :: h
            Real(C_DOUBLE),  Intent(Out)        :: s
            Real(C_DOUBLE),  Intent(Out)        :: v
        End Subroutine HSV_to_RGB
    End Interface

Contains

    Subroutine make_C_string(stringin, stringout)
        use, intrinsic :: iso_c_binding
        Character(*),                  Intent(In)  :: stringin
        Character(Kind=C_CHAR, Len=*), Intent(Out) :: stringout
!       NULL terminate the string
        stringout = stringin//C_NULL_CHAR
    End Subroutine make_C_string

End Module NPSpecModule

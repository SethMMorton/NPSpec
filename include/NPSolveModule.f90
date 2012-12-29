Module NPSolveModule

!   ***********************************************************************
!   This module conatins interfaces needed to call the NPSolve library from
!   Fortran, plus a helper function to make a Fortran string a C-string
!   ***********************************************************************

    use, intrinsic :: iso_c_binding

    Private

    Public NLAMBDA
    Public Efficiency
    Public CrossSection
    Public Molar
    Public Absorbance
    Public wavelengths
    Public material_index
    Public npsolve
    Public RGB
    Public RGB_to_HSV
    Public make_C_string

!   Here is the wavelength and color matching arrays from the NPSolve header
    Integer(C_INT), Parameter                    :: NLAMBDA = 800
    Real(C_DOUBLE), Bind(C, name="wavelengths")  :: wavelengths(NLAMBDA)

!   Had a hell of a time getting an enum to bind, so I am just redefining it here
    Integer(C_INT), Parameter :: Efficiency   = 0
    Integer(C_INT), Parameter :: CrossSection = 1
    Integer(C_INT), Parameter :: Molar        = 2
    Integer(C_INT), Parameter :: Absorbance   = 3

!   Interfaces to the C routines
    Interface
        Integer(C_INT) Function material_index (material)  Bind (C)
            use, intrinsic :: iso_c_binding
            Character(Kind=C_CHAR), Dimension(*), Intent(In) :: material
        End Function material_index
    End Interface

    Interface
        Integer(C_INT) Function npsolve (nlayers, rad, rel_rad, indx, mrefrac,   &
                            size_correct, increment, path_length, concentration, &
                            spectra_type, qext, qscat, qabs) Bind (C)
            use, intrinsic :: iso_c_binding
            Integer(C_INT),  Intent(In), Value  :: nlayers
            Real(C_DOUBLE),  Intent(In)         :: rad(*)
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
        End Function npsolve
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

End Module NPSolveModule

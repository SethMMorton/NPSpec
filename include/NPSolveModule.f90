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
    Public CIE_X
    Public CIE_Y
    Public CIE_Z
    Public CIE_D65
    Public CIE_Mat
    Public initiallize_material_index
    Public material_index
    Public npsolve
    Public make_C_string

!   Bind the C enum
    !Enum, Bind(C)
    !    Enumerator :: Efficiency, CrossSection, Molar, Absorbance
    !End Enum

!   Here is the wavelength and color matching arrays from the NPSolve header
    Integer(C_INT), Parameter                    :: NLAMBDA = 800
    Integer(C_INT), Bind(C, name="Efficiency")   :: Efficiency
    Integer(C_INT), Bind(C, name="CrossSection") :: CrossSection
    Integer(C_INT), Bind(C, name="Molar")        :: Molar
    Integer(C_INT), Bind(C, name="Absorbance")   :: Absorbance
    Real(C_DOUBLE), Bind(C, name="wavelengths")  :: wavelengths(NLAMBDA)
    Real(C_DOUBLE), Bind(C, name="CIE_X")        :: CIE_X(NLAMBDA)
    Real(C_DOUBLE), Bind(C, name="CIE_Y")        :: CIE_Y(NLAMBDA)
    Real(C_DOUBLE), Bind(C, name="CIE_Z")        :: CIE_Z(NLAMBDA)
    Real(C_DOUBLE), Bind(C, name="CIE_D65")      :: CIE_D65(NLAMBDA)

!   Because of the difference in storage order between Fortran and C,
!   it is easier to simply redefine this matrix
    Real(C_DOUBLE), Parameter                    :: CIE_Mat(3,3) =      &
                                RESHAPE((/ 3.2410, -0.9692,  0.0556,    &
                                          -1.5374,  1.8760, -0.2040,    &
                                          -0.4986,  0.0416,  1.0570 /), &
                                        (/3, 3/))

!   Interfaces to the C routines
    Interface
        Subroutine initiallize_material_index () Bind (C)
        End Subroutine initiallize_material_index
    End Interface

    Interface
        Integer(C_INT) Function material_index (material)  Bind (C)
            use, intrinsic :: iso_c_binding
            Character(Kind=C_CHAR), Dimension(*), Intent(In) :: material
        End Function material_index
    End Interface

    Interface
        Subroutine npsolve (nlayers, rad, rel_rad, indx, mrefrac,             &
                            size_correct, coarse, path_length, concentration, &
                            spectra_type, qext, qscat, qabs) Bind (C)
            use, intrinsic :: iso_c_binding
            Integer(C_INT),  Intent(In), Value  :: nlayers
            Real(C_DOUBLE),  Intent(In)         :: rad(*)
            Real(C_DOUBLE),  Intent(In)         :: rel_rad(nlayers,*)
            Integer(C_INT),  Intent(In)         :: indx(*)
            Real(C_DOUBLE),  Intent(In), Value  :: mrefrac
            Logical(C_BOOL), Intent(In), Value  :: size_correct
            Logical(C_BOOL), Intent(In), Value  :: coarse
            Real(C_DOUBLE),  Intent(In), Value  :: path_length
            Real(C_DOUBLE),  Intent(In), Value  :: concentration
            Integer(C_INT),  Intent(In), Value  :: spectra_type
            Real(C_DOUBLE),  Intent(Out)        :: qext(*)
            Real(C_DOUBLE),  Intent(Out)        :: qscat(*)
            Real(C_DOUBLE),  Intent(Out)        :: qabs(*)
        End Subroutine npsolve
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

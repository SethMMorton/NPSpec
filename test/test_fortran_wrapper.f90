module test_fortran_wrapper

    use, intrinsic :: iso_c_binding
    use fruit

    Implicit None

    Private
    Public run_all_tests

!    Integer(C_INT), Parameter :: NLAYERS = 2
!    Real(C_DOUBLE)  :: rad(2) = (/ 10.0, 10.0 /)
!    Integer(C_INT)  :: indx(NLAYERS)
!    Real(C_DOUBLE)  :: qext(NLAMBDA), qscat(NLAMBDA), qabs(NLAMBDA)
!    Logical(C_BOOL) :: size_correct = .false.
!    Logical(C_BOOL) :: coarse = .false.
!    Character(Kind=C_CHAR,Len=14) :: mat
!    Integer :: i
!    Character(*), Parameter :: f = "('NPSOLVE: Ext ',F18.16, ', Sca ', F18.16, ', Abs ', F18.16)"

!   Note that this must be transposed from what the C routine expects because of
!   the way C lays out arrays in memory
!    Real(C_DOUBLE)  :: rel_rad_spheroid(NLAYERS,2) = RESHAPE((/0.8, 0.8, 0.2, 0.2/), (/NLAYERS, 2/))

!    Real(C_DOUBLE), Parameter :: ONE = 1.0_C_DOUBLE

contains

    subroutine run_all_tests
        call run_test_case(NLAMBDA_check, 'NLAMBDA_Check')
        call run_test_case(wavelengths_check, 'Wavelengths')
        call run_test_case(CIE_MAT_check, 'CIE_MAT')
        call run_test_case(mat_indx, 'MatIndx')
    end subroutine

    subroutine NLAMBDA_check
        use NPSolveModule, ONLY : NLAMBDA
        call assert_equals (NLAMBDA, 800)
    end subroutine NLAMBDA_check

    subroutine wavelengths_check
        use NPSolveModule, ONLY : wavelengths
        call assert_equals (wavelengths(1),   200.0d0)
        call assert_equals (wavelengths(2),   201.0d0)
        call assert_equals (wavelengths(11),  210.0d0)
        call assert_equals (wavelengths(501), 700.0d0)
        call assert_equals (wavelengths(800), 999.0d0)
    end subroutine wavelengths_check

    subroutine CIE_MAT_check
        use NPSolveModule, ONLY : CIE_MAT
        Double Precision :: check(3,3) = RESHAPE((/ 3.2410, -0.9692,  0.0556, &
                                                   -1.5374,  1.8760, -0.2040, &
                                                   -0.4986,  0.0416,  1.0570  &
                                                  /), (/3, 3/))
        call assert_equals (CIE_MAT, check, 3, 3)
    end subroutine CIE_MAT_check

    subroutine mat_indx
        use NPSolveModule, ONLY : material_index, make_C_string
        character(kind=C_CHAR, len=14) :: mat
        call make_C_string("Au", mat)
        call assert_equals(material_index(mat), 4)
        call make_C_string("Diamond", mat)
        call assert_equals(material_index(mat), 13)
        call make_C_string("TiO2", mat)
        call assert_equals(material_index(mat), 43)
        call make_C_string("Kryptonite", mat)
        call assert_equals(material_index(mat), -1)
    end subroutine mat_indx

!   Now solve.  
!    call npsolve (NLAYERS, rad, rel_rad_spheroid, indx, ONE, &
!                  size_correct, coarse, ONE, ONE,            &
!                  Efficiency, qext, qscat, qabs)

end module test_fortran_wrapper

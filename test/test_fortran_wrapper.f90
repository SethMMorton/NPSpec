module test_fortran_wrapper

    use, intrinsic :: iso_c_binding
    use fruit
    use NPSolveModule, ONLY : NLAMBDA

    implicit none

    private
    public run_all_tests

    real(C_DOUBLE), parameter :: relative_radius_spheroid1(1,2) = & 
                                    RESHAPE((/1.0, 1.0/), (/1, 2/))
    real(C_DOUBLE), parameter :: relative_radius_spheroid2(2,2) = &
                                    RESHAPE((/0.6, 0.6, 0.4, 0.4/), (/2, 2/))
    real(C_DOUBLE), parameter :: relative_radius_spheroid3(3,2) = &
                                    RESHAPE((/0.2, 0.2, 0.3, 0.3, 0.5, 0.5/), (/3, 2/))

    integer(C_INT) :: index1(1), index2(2), index3(3)
    real(C_DOUBLE) :: qext(NLAMBDA), qabs(NLAMBDA), qscat(NLAMBDA)

contains

    subroutine run_all_tests
        call run_test_case(NLAMBDA_check, 'NLAMBDA_Check')
        call run_test_case(wavelengths_check, 'Wavelengths')
        call run_test_case(CIE_MAT_check, 'CIE_MAT')
        call run_test_case(mat_indx, 'MatIndx')
    end subroutine

    subroutine setup
        use NPSolveModule, ONLY : material_index, make_C_string
        character(kind=C_CHAR, len=14) :: matAg, matSiO2, matTiO2
        call make_C_string("Ag", matAg)
        call make_C_string("TiO2", matSiO2)
        call make_C_string("SiO2", matTiO2)
        index1 = (/ material_index(matAg) /)
        index2 = (/ material_index(matAg), material_index(matSiO2) /)
        index3 = (/ material_index(matAg), material_index(matSiO2), &
                    material_index(matTiO2) /)
    end subroutine setup

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
        double precision :: check(3,3) = RESHAPE((/ 3.2410, -0.9692,  0.0556, &
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

    subroutine Mie1Layer
        use NPSolveModule, ONLY : NLAMBDA, Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_dielectric = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 20.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false., coarse = .false.
        integer(C_INT) :: retval

!       Now solve.  
        retval = npsolve (NLAYERS, radius, relative_radius_spheroid1, index1,&
                      medium_dielectric, size_correct, coarse, 1.0d0, 1.0d0, &
                      Efficiency, qext, qscat, qabs)

        call assert_equals(retval, 0);
!       Extinction
        call assert_equals(qext(1),   2.2285778886683643d0, 1d-14)
        call assert_equals(qext(251), 0.2386953127709600d0, 1d-14)
        call assert_equals(qext(501), 0.0133494242710007d0, 1d-14)
!       Scattering
        call assert_equals(qscat(1),   0.3062750316703077d0, 1d-14)
        call assert_equals(qscat(251), 0.0572390734717247d0, 1d-14)
        call assert_equals(qscat(501), 0.0039184205430821d0, 1d-14)
!       Absorption
        call assert_equals(qabs(1),   1.9223028569980565d0, 1d-14)
        call assert_equals(qabs(251), 0.1814562392992353d0, 1d-14)
        call assert_equals(qabs(501), 0.0094310037279186d0, 1d-14)

    end subroutine Mie1Layer

end module test_fortran_wrapper

program fruit_driver
    use fruit
    use test_fortran_wrapper, ONLY : run_all_tests
    use NPSolveModule, ONLY : initiallize_material_index
    integer :: numfailures
    call initiallize_material_index
    call init_fruit
    call run_all_tests
    call fruit_summary
    call get_failed_count(numfailures)
    if (numfailures > 0) then
        call exit(1)
    else
        call exit(0)
    end if
end program fruit_driver


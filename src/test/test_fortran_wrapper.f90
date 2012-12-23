module test_fortran_wrapper

    use, intrinsic :: iso_c_binding
    use fruit
    use NPSolveModule, ONLY : NLAMBDA

    implicit none

    private
    public run_all_tests

    real(C_DOUBLE) :: relative_radius_spheroid1(1,2) = &
                                    RESHAPE((/1.0d0, 1.0d0/), (/1, 2/))
    real(C_DOUBLE) :: relative_radius_spheroid2(2,2) = &
                                    RESHAPE((/0.6d0, 0.6d0, 0.4d0, 0.4d0/), (/2, 2/))
    real(C_DOUBLE) :: relative_radius_spheroid3(3,2) = &
                                    RESHAPE((/0.2d0, 0.2d0, 0.3d0, 0.3d0, 0.5d0, 0.5d0/), (/3, 2/))

    integer(C_INT) :: index1(1), index2(2), index3(3)
    real(C_DOUBLE) :: qext(NLAMBDA), qabs(NLAMBDA), qscat(NLAMBDA)
    real(C_DOUBLE) :: pi
    real(C_DOUBLE) :: avogadro

contains

    subroutine run_all_tests
        call run_test_case(NLAMBDA_check, 'NLAMBDA_Check')
        call run_test_case(wavelengths_check, 'Wavelengths')
        call run_test_case(enum_check, 'Enum_Check')
        call run_test_case(CIE_MAT_check, 'CIE_MAT')
        call run_test_case(mat_indx, 'MatIndx')
        call run_test_case(Mie1Layer, 'Mie1Layer')
        call run_test_case(Mie2Layer, 'Mie2Layer')
        call run_test_case(Mie3Layer, 'Mie3Layer')
        call run_test_case(Quasi1Layer, 'Quasi1Layer')
        call run_test_case(Quasi2Layer, 'Quasi2Layer')
        call run_test_case(Quasi3Layer, 'Quasi3Layer')
        call run_test_case(QuasiProlate, 'QuasiProlate')
        call run_test_case(QuasiOblate, 'QuasiOblate')
        call run_test_case(TestIncrement5, 'TestIncrement5')
        call run_test_case(TestIncrement7, 'TestIncrement7')
        call run_test_case(TestIncrementNegative, 'TestIncrementNegative')
        call run_test_case(TestSizeCorrect, 'TestSizeCorrect')
        call run_test_case(TestMediumRefractiveIndex, 'TestMediumRefractiveIndex')
        call run_test_case(TestMieQuasiSmall, 'TestMieQuasiSmall')
        call run_test_case(TestCrossSection, 'TestCrossSection')
        call run_test_case(TestMolar, 'TestMolar')
        call run_test_case(TestAbsorbance, 'TestAbsorbance')
    end subroutine

    subroutine setup
        use NPSolveModule, ONLY : material_index, make_C_string
        character(kind=C_CHAR, len=14) :: mat
        call make_C_string("Ag", mat)
        index1(1) = material_index(mat)
        index2(1) = material_index(mat)
        index3(1) = material_index(mat)
        call make_C_string("Quartz", mat)
        index2(2) = material_index(mat)
        index3(2) = material_index(mat)
        call make_C_string("TiO2", mat)
        index3(3) = material_index(mat)
        pi        = 4.0d0 * atan(1.0d0)
        avogadro  = 6.0221412927d23
        qext  = 0.0d0
        qscat = 0.0d0
        qabs  = 0.0d0
    end subroutine setup

    subroutine NLAMBDA_check
        use NPSolveModule, ONLY : NLAMBDA
        call setup
        call assert_equals (NLAMBDA, 800)
    end subroutine NLAMBDA_check

    subroutine wavelengths_check
        use NPSolveModule, ONLY : wavelengths
        call setup
        call assert_equals(200.0d0, wavelengths(1))
        call assert_equals(201.0d0, wavelengths(2))
        call assert_equals(210.0d0, wavelengths(11))
        call assert_equals(700.0d0, wavelengths(501))
        call assert_equals(999.0d0, wavelengths(800))
    end subroutine wavelengths_check

    subroutine enum_check
        use NPSolveModule, ONLY : Efficiency, CrossSection, Molar, Absorbance
        call setup
        call assert_equals(0, Efficiency)
        call assert_equals(1, CrossSection)
        call assert_equals(2, Molar)
        call assert_equals(3, Absorbance)
end subroutine enum_check

    subroutine CIE_MAT_check
        use NPSolveModule, ONLY : CIE_MAT
        double precision :: check(3,3) = RESHAPE((/ 3.2410, -0.9692,  0.0556, &
                                                   -1.5374,  1.8760, -0.2040, &
                                                   -0.4986,  0.0416,  1.0570  &
                                                  /), (/3, 3/))
        call setup
        call assert_equals (CIE_MAT, check, 3, 3)
    end subroutine CIE_MAT_check

    subroutine mat_indx
        use NPSolveModule, ONLY : material_index, make_C_string
        character(kind=C_CHAR, len=14) :: mat
        call setup
        call make_C_string("Au", mat)
        call assert_equals(4, material_index(mat))
        call make_C_string("Diamond", mat)
        call assert_equals(13, material_index(mat))
        call make_C_string("TiO2", mat)
        call assert_equals(43, material_index(mat))
        call make_C_string("Kryptonite", mat)
        call assert_equals(-1, material_index(mat))
    end subroutine mat_indx

    subroutine Mie1Layer
        use NPSolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 20.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve (nlayers, radius, relative_radius_spheroid1, index1,&
                      medium_refrac, size_correct, 1, 1.0d0, 1.0d0, &
                      Efficiency, qext, qscat, qabs)

        call assert_equals(0, retval)
!       Extinction
        call assert_equals(2.2285778886683643d0, qext(1),   1d-14)
        call assert_equals(0.2386953127709600d0, qext(251), 1d-14)
        call assert_equals(0.0133494242710007d0, qext(501), 1d-14)
!       Scattering
        call assert_equals(0.3062750316703077d0, qscat(1),   1d-14)
        call assert_equals(0.0572390734717247d0, qscat(251), 1d-14)
        call assert_equals(0.0039184205430821d0, qscat(501), 1d-14)
!       Absorption
        call assert_equals(1.9223028569980565d0, qabs(1),   1d-14)
        call assert_equals(0.1814562392992353d0, qabs(251), 1d-14)
        call assert_equals(0.0094310037279186d0, qabs(501), 1d-14)

    end subroutine Mie1Layer

    subroutine Mie2Layer
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 2
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 20.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, TRANSPOSE(relative_radius_spheroid2), index2,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency, &
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(0.5373534948346590d0, qext(1),   1d-14)
        call assert_equals(0.3143612207865766d0, qext(251), 1d-14)
        call assert_equals(0.0058357432042841d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.0482912065377480d0, qscat(1),   1d-14)
        call assert_equals(0.0250656892007021d0, qscat(251), 1d-14)
        call assert_equals(0.0010568992342033d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(0.4890622882969110d0, qabs(1),   1d-14)
        call assert_equals(0.2892955315858746d0, qabs(251), 1d-14)
        call assert_equals(0.0047788439700809d0, qabs(501), 1d-14)

    end subroutine

    subroutine Mie3Layer
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 3
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 20.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid3, index3,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(2.0261050243604539d0, qext(1),   1d-14)
        call assert_equals(0.0248674748630347d0, qext(251), 1d-14)
        call assert_equals(0.0012280674844302d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.3388504569262775d0, qscat(1),   1d-14)
        call assert_equals(0.0081054247786561d0, qscat(251), 1d-14)
        call assert_equals(0.0011010132200970d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(1.6872545674341763d0, qabs(1),   1d-14)
        call assert_equals(0.0167620500843786d0, qabs(251), 1d-14)
        call assert_equals(0.0001270542643331d0, qabs(501), 1d-14)

    end subroutine

    subroutine Quasi1Layer
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, 10.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(1.0611537055667197d0, qext(1),   1d-14)
        call assert_equals(0.0764461773630003d0, qext(251), 1d-14)
        call assert_equals(0.0042801575796004d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.0241764183390374d0, qscat(1),   1d-14)
        call assert_equals(0.0029871788218089d0, qscat(251), 1d-14)
        call assert_equals(0.0002336823836763d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(1.0369772872276823d0, qabs(1),   1d-14)
        call assert_equals(0.0734589985411914d0, qabs(251), 1d-14)
        call assert_equals(0.0040464751959240d0, qabs(501), 1d-14)

    end subroutine

    subroutine Quasi2Layer
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 2
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, 10.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid2, index2,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(0.2170155342365016d0, qext(1),   1d-14)
        call assert_equals(0.1117949449870051d0, qext(251), 1d-14)
        call assert_equals(0.0022927389984260d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.0033678648313046d0, qscat(1),   1d-14)
        call assert_equals(0.0012481734552626d0, qscat(251), 1d-14)
        call assert_equals(0.0000636722865861d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(0.2136476694051971d0, qabs(1),   1d-14)
        call assert_equals(0.1105467715317425d0, qabs(251), 1d-14)
        call assert_equals(0.0022290667118399d0, qabs(501), 1d-14)

    end subroutine

    subroutine Quasi3Layer
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 3
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, 10.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid3, index3,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        ! This should fail - 2 means too many layers for quasi
        call assert_equals(2, retval)

    end subroutine

    subroutine QuasiProlate
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, 5.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(0.6549347175126272d0, qext(1),   1d-14)
        call assert_equals(0.0713320530670000d0, qext(251), 1d-14)
        call assert_equals(0.0030711419580141d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.0036651236274123d0, qscat(1),   1d-14)
        call assert_equals(0.0006166697027189d0, qscat(251), 1d-14)
        call assert_equals(0.0000411265232696d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(0.6512695938852150d0, qabs(1),   1d-14)
        call assert_equals(0.0707153833642811d0, qabs(251), 1d-14)
        call assert_equals(0.0030300154347445d0, qabs(501), 1d-14)

    end subroutine

    subroutine QuasiOblate
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 5.0d0, 10.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(0.8371294510751321d0, qext(1),   1d-14)
        call assert_equals(0.0785298784021552d0, qext(251), 1d-14)
        call assert_equals(0.0037899923685295d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.0093404960245316d0, qscat(1),   1d-14)
        call assert_equals(0.0014531069310511d0, qscat(251), 1d-14)
        call assert_equals(0.0001022886141183d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(0.8277889550506005d0, qabs(1),   1d-14)
        call assert_equals(0.0770767714711041d0, qabs(251), 1d-14)
        call assert_equals(0.0036877037544112d0, qabs(501), 1d-14)

    end subroutine

    subroutine TestIncrement5
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 5, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(1.0981095009299680d0, qext(1), 1d-14)
        call assert_equals(0.0d0, qext(2))
        call assert_equals(0.0d0, qext(3))
        call assert_equals(0.0d0, qext(4))
        call assert_equals(0.0d0, qext(5))
        call assert_equals(1.0405331345505673d0, qext(6), 1d-14)
        ! Scattering
        call assert_equals(0.0241147084928534d0, qscat(1), 1d-14)
        call assert_equals(0.0d0, qscat(2))
        call assert_equals(0.0d0, qscat(3))
        call assert_equals(0.0d0, qscat(4))
        call assert_equals(0.0d0, qscat(5))
        call assert_equals(0.0218343064818317d0, qscat(6), 1d-14)
        ! Absorption
        call assert_equals(1.0739947924371145d0, qabs(1), 1d-14)
        call assert_equals(0.0d0, qabs(2))
        call assert_equals(0.0d0, qabs(3))
        call assert_equals(0.0d0, qabs(4))
        call assert_equals(0.0d0, qabs(5))
        call assert_equals(1.0186988280687357d0, qabs(6), 1d-14)

    end subroutine

    subroutine TestIncrement7
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 7, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        ! Bad increment should return 3
        call assert_equals(3, retval)

    end subroutine

    subroutine TestIncrementNegative
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, -1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        ! Bad increment should return 3
        call assert_equals(3, retval)

    end subroutine

    subroutine TestSizeCorrect
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 5.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .true.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(0.5334378505066342d0, qext(1),   1d-14)
        call assert_equals(0.0025491434133546d0, qext(251), 1d-14)
        call assert_equals(0.0020526111029140d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(0.0015165842885118d0, qscat(1),   1d-14)
        call assert_equals(0.0001902864485487d0, qscat(251), 1d-14)
        call assert_equals(0.0000145576138027d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(0.5319212662181224d0, qabs(1),   1d-14)
        call assert_equals(0.0023588569648059d0, qabs(251), 1d-14)
        call assert_equals(0.0020380534891113d0, qabs(501), 1d-14)

    end subroutine

    subroutine TestMediumRefractiveIndex
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 2.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 20.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT) :: retval
        call setup
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)

        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(2.8328562530696506d0, qext(1),   1d-14)
        call assert_equals(2.0565291634339800d0, qext(251), 1d-14)
        call assert_equals(0.0970864882507346d0, qext(501), 1d-14)
        ! Scattering
        call assert_equals(1.0698792522825473d0, qscat(1),   1d-14)
        call assert_equals(1.4426586799505641d0, qscat(251), 1d-14)
        call assert_equals(0.0718406948561757d0, qscat(501), 1d-14)
        ! Absorption
        call assert_equals(1.7629770007871033d0, qabs(1),   1d-14)
        call assert_equals(0.6138704834834159d0, qabs(251), 1d-14)
        call assert_equals(0.0252457933945589d0, qabs(501), 1d-14)

    end subroutine

    subroutine TestMieQuasiSmall
        use NPsolveModule, ONLY : Efficiency, npsolve
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 3.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        real(C_DOUBLE),  parameter :: radius2(2) = (/ 3.0d0, 3.0d0 /)
        real(C_DOUBLE) :: qext2(NLAMBDA), qscat2(NLAMBDA), qabs2(NLAMBDA)
        integer(C_INT) :: retval
        call setup
        ! Mie
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)
        call assert_equals(0, retval)
        ! Quasistatic
        retval = npsolve(nlayers, radius2, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, 1, 1.0d0, 1.0d0, Efficiency,&
                         qext2, qscat2, qabs2)
        call assert_equals(0, retval)

        ! Extinction
        call assert_equals(qext2(1),   qext(1),   2d-3)
        call assert_equals(qext2(251), qext(251), 2d-3)
        call assert_equals(qext2(501), qext(501), 2d-3)
        ! Scattering
        call assert_equals(qscat2(1),   qscat(1),   2d-3)
        call assert_equals(qscat2(251), qscat(251), 2d-3)
        call assert_equals(qscat2(501), qscat(501), 2d-3)
        ! Absorption
        call assert_equals(qabs2(1),   qabs(1),   2d-3)
        call assert_equals(qabs2(251), qabs(251), 2d-3)
        call assert_equals(qabs2(501), qabs(501), 2d-3)

    end subroutine

    subroutine TestCrossSection
        use NPsolveModule, ONLY : Efficiency, CrossSection, npsolve, NLAMBDA
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT),  parameter :: inc = 80
        real(C_DOUBLE) :: qext2(NLAMBDA), qscat2(NLAMBDA), qabs2(NLAMBDA)
        integer(C_INT) :: retval
        call setup
        qext2  = 0.0d0
        qscat2 = 0.0d0
        qabs2  = 0.0d0
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, inc, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)
        call assert_equals(0, retval)
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, inc, 1.0d0, 1.0d0, CrossSection,&
                         qext2, qscat2, qabs2)
        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(qext2, qext * pi * radius(1)**2, NLAMBDA, 1d-14)
        ! Scattering
        call assert_equals(qscat2, qscat * pi * radius(1)**2, NLAMBDA, 1d-14)
        ! Absorbance
        call assert_equals(qabs2, qabs * pi * radius(1)**2, NLAMBDA, 1d-14)
    end subroutine

    subroutine TestMolar
        use NPsolveModule, ONLY : Efficiency, Molar, npsolve, NLAMBDA
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT),  parameter :: inc = 80
        real(C_DOUBLE) :: qext2(NLAMBDA), qscat2(NLAMBDA), qabs2(NLAMBDA)
        integer(C_INT) :: retval
        call setup
        qext2  = 0.0d0
        qscat2 = 0.0d0
        qabs2  = 0.0d0
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, inc, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)
        call assert_equals(0, retval)
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, inc, 1.0d0, 1.0d0, Molar,&
                         qext2, qscat2, qabs2)
        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(qext2, qext * pi * radius(1)**2 * 1d-14 * avogadro &
                                  / ( 1000d0 * log(10d0) ), NLAMBDA, 1d-6)
        ! Scattering
        call assert_equals(qscat2, qscat * pi * radius(1)**2 * 1d-14 * avogadro &
                                   / ( 1000d0 * log(10d0) ), NLAMBDA, 1d-6)
        ! Absorbance
        call assert_equals(qabs2, qabs * pi * radius(1)**2 * 1d-14 * avogadro &
                                  / ( 1000d0 * log(10d0) ), NLAMBDA, 1d-6)
    end subroutine

    subroutine TestAbsorbance
        use NPsolveModule, ONLY : Efficiency, Absorbance, npsolve, NLAMBDA
        integer(C_INT),  parameter :: nlayers = 1
        real(C_DOUBLE),  parameter :: medium_refrac = 1.0d0
        real(C_DOUBLE),  parameter :: radius(2) = (/ 10.0d0, -1.0d0 /)
        logical(C_BOOL), parameter :: size_correct = .false.
        integer(C_INT),  parameter :: inc = 80
        real(C_DOUBLE),  parameter :: path_length = 1.5d0
        real(C_DOUBLE),  parameter :: molarity = 0.004d0
        real(C_DOUBLE) :: qext2(NLAMBDA), qscat2(NLAMBDA), qabs2(NLAMBDA)
        integer(C_INT) :: retval
        call setup
        qext2  = 0.0d0
        qscat2 = 0.0d0
        qabs2  = 0.0d0
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, inc, 1.0d0, 1.0d0, Efficiency,&
                         qext, qscat, qabs)
        call assert_equals(0, retval)
        retval = npsolve(nlayers, radius, relative_radius_spheroid1, index1,&
                         medium_refrac, size_correct, inc, path_length, molarity,&
                         Absorbance, qext2, qscat2, qabs2)
        call assert_equals(0, retval)
        ! Extinction
        call assert_equals(qext2, qext * pi * radius(1)**2 &
                                       * 1d-14 * avogadro / ( 1000d0 * log(10d0) ) &
                                       * path_length * molarity, NLAMBDA, 1d-7)
        ! Scattering
        call assert_equals(qscat2, qscat * pi * radius(1)**2 &
                                         * 1d-14 * avogadro / ( 1000d0 * log(10d0) ) &
                                         * path_length * molarity, NLAMBDA, 1d-7)
        ! Absorbance
        call assert_equals(qabs2, qabs * pi * radius(1)**2 &
                                       * 1d-14 * avogadro / ( 1000d0 * log(10d0) ) &
                                       * path_length * molarity, NLAMBDA, 1d-7)
    end subroutine

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


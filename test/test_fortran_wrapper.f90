program main

    use, intrinsic :: iso_c_binding
    use NPSolveModule

    Implicit None

    Integer(C_INT), Parameter :: NLAYERS = 2
    Real(C_DOUBLE)  :: rad(2) = (/ 10.0, 10.0 /)
    Integer(C_INT)  :: indx(NLAYERS)
    Real(C_DOUBLE)  :: qext(NLAMBDA), qscat(NLAMBDA), qabs(NLAMBDA)
    Logical(C_BOOL) :: size_correct = .false.
    Logical(C_BOOL) :: coarse = .false.
    Character(Kind=C_CHAR,Len=14) :: mat
    Integer :: i
    Character(*), Parameter :: f = "('NPSOLVE: Ext ',F18.16, ', Sca ', F18.16, ', Abs ', F18.16)"

!   Note that this must be transposed from what the C routine expects because of
!   the way C lays out arrays in memory
    Real(C_DOUBLE)  :: rel_rad_spheroid(NLAYERS,2) = RESHAPE((/0.8, 0.8, 0.2, 0.2/), (/NLAYERS, 2/))

    Real(C_DOUBLE), Parameter :: ONE = 1.0_C_DOUBLE

!   Prep the material index
    call initiallize_material_index ()

!   Get the indices for Ag and Au
    call make_C_string("Ag", mat)
    indx(1) = material_index(mat)
    call make_C_string("Au", mat)
    indx(2) = material_index(mat)

!   Now solve.  
    call npsolve (NLAYERS, rad, rel_rad_spheroid, indx, ONE, &
                  size_correct, coarse, ONE, ONE,            &
                  Efficiency, qext, qscat, qabs)
    write(*,*) 
    write(*,*) 
    write(*,'(a)') "THIS IS FTEST"
    write(*,*) 
    do i = 1, 20
        write(*,f) qext(i), qscat(i), qabs(i)
    end do

end program main

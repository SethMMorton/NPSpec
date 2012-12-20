program fruit_driver
    use fruit
    use test_fortran_wrapper, ONLY : run_all_tests
    use NPSolveModule, ONLY : initiallize_material_index
    call initiallize_material_index
    call init_fruit
    call run_all_tests
    call fruit_summary
end program fruit_driver

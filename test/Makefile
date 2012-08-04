###############
# COMPILE FLAGS
###############

FC=gfortran
CXX=g++
CC=gcc

INC=-I../include
CCFLAGS=-fPIC -g -Wall ${INC} -Wstrict-prototypes -fno-strict-aliasing
CXXFLAGS=-fPIC -g -Wall ${INC} -fno-strict-aliasing
FCFLAGS=-fPIC -g -Wall ${INC} -J.

all: verify c fortran python

#############
# THE LIBRARY
#############

libNPSolve.a: quasi.o mie.o npsolve.o experimental_dielectrics.o wavelengths.o drude_parameters.o standard_color_matching.o material_index.o
	ar cr $@ $^
	ranlib $@

npsolve.o: ../src/npsolve.cpp mie.o quasi.o experimental_dielectrics.o wavelengths.o drude_parameters.o
	${CXX} ${CXXFLAGS} -c $< -o $@

mie.o: ../src/mie.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

quasi.o: ../src/quasi.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

experimental_dielectrics.o: ../src/experimental_dielectrics.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

wavelengths.o: ../src/wavelengths.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

standard_color_matching.o: ../src/standard_color_matching.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

material_index.o: ../src/material_index.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

drude_parameters.o: ../src/drude_parameters.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

#######
# CTEST
#######

c: ctest

ctest: main.c libNPSolve.a
	${CC} ${CCFLAGS} -lstdc++ -o $@ $^

##############
# FORTRAN TEST
##############

fortran: ftest

ftest: main.f90 libNPSolve.a npsolvemodule.o
	${FC} ${FCFLAGS} -lstdc++ -o $@ $^

npsolvemodule.o: ../NPSolveModule.f90
	${FC} ${FCFLAGS} -c $< -o $@

#############
# VERIFY TEST
#############

verify: main.cpp quasif.o mief.o libNPSolve.a
	${CXX} ${CXXFLAGS} -lgfortran  -o $@ $^

mief.o: mie.f90
	${FC} ${FCFLAGS} -c $< -o $@

quasif.o: quasi.f90
	${FC} ${FCFLAGS} -c $< -o $@

#############
# PYTHON TEST
#############

python: npsolve.so

npsolve.so: pyNPSolve.o libNPSolve.a
	${CC} ${CCFLAGS} -lstdc++ -shared -Wl,-O1 -Wl,-Bsymbolic-functions -o $@ $^

pyNPSolve.o: ../pyNPSolve.c
	${CC} ${CCFLAGS} -I/usr/include/python2.6 -c $< -o $@

########
# EXTRAS
########

test:
	if test -e verify; then ./verify; fi
	if test -e ctest; then ./ctest; fi
	if test -e ftest; then ./ftest; fi
	if test -e npsolve.so; then ./ptest; fi

clean:
	rm -rf *.o *.so *.a *.mod pyNPSolve.c setup.py verify ctest ftest build/ *.dSYM/

#  Make sure we don't remove this by accident if interrupted at the wrong time.
.PRECIOUS: Makefile

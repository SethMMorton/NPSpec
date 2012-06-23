FC=gfortran
CXX=g++
CC=gcc

INC=-I../include
CCFLAGS=-fPIC -g -Wall ${INC} -Wstrict-prototypes -fno-strict-aliasing
CXXFLAGS=-fPIC -g -Wall ${INC} -fno-strict-aliasing
FCFLAGS=-fPIC -g -Wall -fbacktrace ${INC} -J../extensions
CXXLINK=-shared -Wl,-soname,libNPSolve.so

all: verify ctest ftest pynpsolve.so

ftest: libNPSolve.so npsolvemodule.o
	${FC} ${FCFLAGS} -L. -lNPSolve -o $@ main.f90 npsolvemodule.o

ctest: libNPSolve.so
	${CC} ${CCFLAGS} -L. -lNPSolve -o $@ main.c

verify: main.cpp quasif.o mief.o libNPSolve.so
	${CXX} ${CXXFLAGS} -lgfortran -L. -lNPSolve -o $@  main.cpp quasif.o mief.o

pynpsolve.so: pyNPSolve.o
	${CC} ${CCFLAGS} -L. -lNPSolve -shared -Wl,-O1 -Wl,-Bsymbolic-functions -o $@ $^

pyNPSolve.o: ../extensions/pyNPSolve.c libNPSolve.so
	${CC} ${CCFLAGS} -I/usr/include/python2.6 -c $< -o $@

libNPSolve.so: quasic.o miec.o npsolve.o experimental_dielectrics.o wavelengths.o drude_parameters.o standard_color_matching.o material_index.o
	${CXX} ${CXXFLAGS} ${CXXLINK} -o $@ $^

npsolvemodule.o: ../extensions/NPSolveModule.f90
	${FC} ${FCFLAGS} -c $< -o $@

npsolve.o: ../src/npsolve.cpp miec.o quasic.o experimental_dielectrics.o wavelengths.o drude_parameters.o
	${CXX} ${CXXFLAGS} -c $< -o $@

miec.o: ../src/mie.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

quasic.o: ../src/quasi.cpp
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

mief.o: mie.f90
	${FC} ${FCFLAGS} -c $< -o $@

quasif.o: quasi.f90
	${FC} ${FCFLAGS} -c $< -o $@

test:
	LD_LIBRARY_PATH=.
	./verify
	./ctest
	./ftest
	#./ptest

clean:
	rm -rf *.o *.so pyNPSolve.c setup.py verify ctest ftest build/

#  Make sure we don't remove this by accident if interrupted at the wrong time.
.PRECIOUS: Makefile

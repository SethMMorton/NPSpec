#! /usr/bin/env python

from distutils.core import setup, Extension
from numpy import get_include

setup(name='pynpsolve',
      version='0.1',
      ext_modules=[
        Extension('pynpsolve', ['extensions/pyNPSolve.c'],
                  include_dirs=['include', get_include()],
                  library_dirs=['.'],
                  libraries=['sNPSolve'],)
        ])

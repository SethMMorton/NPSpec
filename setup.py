#! /usr/bin/env python

# In case setuptools is not installed
from distribute_setup import use_setuptools
use_setuptools()

from os.path import join
from glob import glob
from setuptools import setup, Extension
import numpy

# Create a list of all the source files
sourcefiles = ['pyNPSolve.c'] + glob(join('src', '*'))

# Define the build
setup(name='npsolve',
      version='0.1',
      author='Seth M. Morton',
      author_email='drtuba78@gmail.com',
      description="Easily solve for a nanoparticle's spectral properties analytically",
      url='https://github.com/SethMMorton/NPSolve',
      ext_modules=[
        Extension('npsolve', sourcefiles,
                  include_dirs=['include', numpy.get_include()],
                 )
        ],
      )

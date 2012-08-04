#! /usr/bin/env python

# In case setuptools is not installed
import ez_setup
ez_setup.use_setuptools()

from os.path import join, exists
from glob import glob
from setuptools import setup, Extension
import numpy

# Create a list of all the source files
if not exists('libNPSolve.a'):
    from sys import exit
    exit('''\
Please compile the static version of libNPSolve and place it into
this directory before attempting to compile the python extension.
''')
sourcefiles = ['pyNPSolve.c']

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
                  libraries=['NPSolve', 'stdc++'], library_dirs=['.'],
                 )
        ],
      )

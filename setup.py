#! /usr/bin/env python

from os.path import join
from glob import glob
from distutils.core import setup, Extension
from numpy import get_include

# Create a list of all the source files
sourcefiles = [join('extensions', 'pyNPSolve.c')]
for f in glob(join('src', '*')):
    sourcefiles.append(f)

# Define the build
setup(name='npsolve',
      version='0.1',
      author='Seth M. Morton',
      author_email='drtuba78@gmail.com',
      description="Easily solve for a nanoparticle's spectral properties analytically",
      url='https://github.com/SethMMorton/NPSolve',
      ext_modules=[
        Extension('npsolve', sourcefiles,
                  include_dirs=['include', get_include()])
        ])

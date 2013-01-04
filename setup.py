#! /usr/bin/env python

# In case setuptools is not installed
from distribute_setup import use_setuptools
use_setuptools()
from setuptools import setup, Extension, find_packages
from setuphelp import current_version, PyTest

from os.path import join, abspath
from glob import glob
import numpy

DESCRIPTION = "Easily solve for a nanoparticle's spectral properties analytically"
try:
    with open('README.rst') as fl:
        LONG_DESCRIPTION = fl.read()
except IOError:
    LONG_DESCRIPTION = DESCRIPTION

# Create a list of all the source files
sourcefiles  = [join('python', 'npspec', 'npspec.i')]
sourcefiles += glob(join('src', 'npspec', '*.cpp'))

# Extension definition
includes  = [abspath('include'), numpy.get_include()]
swigopts  = ['-modern', '-c++', '-outputtuple']
swigopts += ['-I'+i for i in includes]
ext = Extension('_npspec', sourcefiles, 
                include_dirs=includes, swig_opts=swigopts)

# Define the build
setup(name='npspec',
      version=current_version(),
      author='Seth M. Morton',
      author_email='drtuba78@gmail.com',
      url='https://github.com/SethMMorton/NPSpec',
      license='MIT',
      ext_modules=[ext],
      ext_package='npspec',
      packages=find_packages(join('python')),
      package_dir={'':join('python')},
      description=DESCRIPTION,
      long_description=LONG_DESCRIPTION,
      use_2to3=True,
      tests_require=['pytest'],
      cmdclass={'test':PyTest},
      classifiers=(
                  'Development Status :: 4 - Beta',
                  #'Development Status :: 5 - Production/Stable',
                  'Intended Audience :: Science/Research',
                  'Operating System :: OS Independent',
                  'License :: OSI Approved :: MIT License',
                  'Natural Language :: English',
                  #'Programming Language :: Python :: 2.6',
                  #'Programming Language :: Python :: 2.7',
                  #'Programming Language :: Python :: 3',
                  'Topic :: Scientific/Engineering',
     )
)

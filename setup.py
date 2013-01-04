#! /usr/bin/env python

# In case setuptools is not installed
from distribute_setup import use_setuptools
use_setuptools()
from setuptools import setup, Extension, find_packages, Command
from setuptools.command.test import test as TestCommand

from os.path import join, abspath
from os import remove, getcwd
from glob import glob
from subprocess import check_call, CalledProcessError
import re
import sys
import numpy

DESCRIPTION = "Easily solve for a nanoparticle's spectral properties analytically"
try:
    with open('README.rst') as fl:
        LONG_DESCRIPTION = fl.read()
except IOError:
    LONG_DESCRIPTION = DESCRIPTION

def current_version():
    '''Get the current version number'''
    VERSIONFILE = join('include', 'npspec', 'version.h')
    with open(VERSIONFILE, "rt") as fl:
        versionstring = fl.readline().strip()
    m = re.search(r"^#define \w+_VERSION \"(.*)\"", versionstring)
    if m:
        return m.group(1)
    else:
        s = "Unable to locate version string in {0}"
        raise RuntimeError (s.format(VERSIONFILE))

class SWIG(Command):
    '''Class to regenerate the SWIG wrappers'''
    description = "(re)generate SWIG wrappers"
    user_options = []
    def initialize_options(self):
        self.cwd = None
    def finalize_options(self):
        self.cwd = getcwd()
    def run(self):
        # First delete the previously wrapped files
        wrapped = join('python', 'npspec', 'npspec_wrap.cpp')
        pyfile  = join('python', 'npspec', 'npspec.py')
        for f in (wrapped, pyfile):
            try:
                remove(f)
            except OSError:
                pass
        # Now rewrap the files
        c = ['swig', '-python', '-modern', '-c++', '-outputtuple',
             '-Iinclude', '-I'+numpy.get_include(), '-o', wrapped,
             join('python', 'npspec', 'npspec.i')]
        print ' '.join(c)
        try:
            check_call(c)
        except CalledProcessError:
            sys.exit("Error wrapping C++ library... is SWIG installed?")

class PyTest(TestCommand):
    '''Define how to use pytest to test the code'''
    def finalize_options(self):
        TestCommand.finalize_options(self)
        self.test_args = ['build']
        self.test_suite = True
    def run_tests(self):
        #import here, cause outside the eggs aren't loaded
        import pytest
        sys.exit(pytest.main(self.test_args))

# Create a list of all the source files
sourcefiles  = [join('python', 'npspec', 'npspec_wrap.cpp')]
sourcefiles += glob(join('src', 'npspec', '*.cpp'))

# Extension definition
includes  = [abspath('include'), numpy.get_include()]
ext = Extension('_npspec', sourcefiles, include_dirs=includes)

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
      package_data={'': [join('tests', '*.py')]},
      description=DESCRIPTION,
      long_description=LONG_DESCRIPTION,
      use_2to3=True,
      tests_require=['pytest'],
      cmdclass={'test':PyTest, 'swig':SWIG},
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

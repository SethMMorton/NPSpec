from npspec import *
from numpy import arange, array_equal
from pytest import raises

def test_lengths():
    assert 800 == NLAMBDA
    assert 10  == MAXLAYERS

def test_NanoparticleShape():
    assert 0 == Sphere
    assert 1 == Ellipsoid

def test_SpectraType():
    assert 0 == Efficiency
    assert 1 == CrossSection
    assert 2 == Molar
    assert 3 == Absorption

def test_SpectraProperty():
    assert 0 == Extinction
    assert 1 == Absorbance
    assert 2 == Scattering

def test_no_ErrorCodes():
    with raises(NameError):
        assert  0  == NoError

def test_wavelengths():
    assert array_equal(arange(200.0, 1000.0), wavelengths)

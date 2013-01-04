from npspec import *
from numpy.testing import assert_approx_equal
from pytest import raises

def test_defaults():
    np = Nanoparticle()
    assert 1 == np.getNLayers()
    assert Efficiency == np.getSpectraType()
    assert Absorbance == np.getSpectraProperty()
    assert False == np.getSizeCorrect()
    assert_approx_equal(1.0, np.getPathLength())
    assert_approx_equal(1.0e-6, np.getConcentration())
    assert_approx_equal(1.0, np.getMediumRefractiveIndex())
    assert_approx_equal(1.0, np.getIncrement())
    assert Sphere == np.getShape()
    assert_approx_equal(10.0, np.getSphereRadius())
    assert_approx_equal(10.0, np.getEllipsoidZRadius())
    assert_approx_equal(10.0, np.getEllipsoidXYRadius())
    for i in xrange(1, MAXLAYERS+1):
        assert "Ag" == np.getLayerMaterial(i)
        assert 0 == np.getLayerIndex(i)
        if i == 1:
            assert_approx_equal(1.0, np.getSphereLayerRelativeRadius(i))
            assert_approx_equal(1.0, np.getEllipsoidLayerZRelativeRadius(i))
            assert_approx_equal(1.0, np.getEllipsoidLayerXYRelativeRadius(i))
        else:
            assert_approx_equal(0.0, np.getSphereLayerRelativeRadius(i))
            assert_approx_equal(0.0, np.getEllipsoidLayerZRelativeRadius(i))
            assert_approx_equal(0.0, np.getEllipsoidLayerXYRelativeRadius(i))

def test_NLayers():
    np = Nanoparticle()
    np.setNLayers(4)
    assert 4 == np.getNLayers()
    with raises(IndexError):
        np.setNLayers(11)
    assert 4 == np.getNLayers()
    with raises(IndexError):
        np.setNLayers(0)
    assert 4 == np.getNLayers()

def test_Shape():
    np = Nanoparticle()
    np.setShape(Ellipsoid)
    assert Ellipsoid == np.getShape()
    np.setShape(Sphere)
    assert Sphere == np.getShape()

def test_SpectraType():
    np = Nanoparticle()
    np.setSpectraType(CrossSection)
    assert CrossSection == np.getSpectraType()
    np.setSpectraType(Molar)
    assert Molar == np.getSpectraType()
    np.setSpectraType(Absorption)
    assert Absorption == np.getSpectraType()
    np.setSpectraType(Efficiency)
    assert Efficiency == np.getSpectraType()

def test_SpectraProperty():
    np = Nanoparticle()
    np.setSpectraProperty(Scattering)
    assert Scattering == np.getSpectraProperty()
    np.setSpectraProperty(Absorbance)
    assert Absorbance == np.getSpectraProperty()
    np.setSpectraProperty(Extinction)
    assert Extinction == np.getSpectraProperty()

def test_SphereRadius():
    np = Nanoparticle()
    np.setSphereRadius(30.0)
    assert_approx_equal(30.0, np.getSphereRadius())
    np.setSphereRadius(1.0)
    assert_approx_equal(1.0, np.getSphereRadius())
    with raises(ValueError):
        np.setSphereRadius(0.0)
    assert_approx_equal(1.0, np.getSphereRadius())

def test_RelativeRadius():
    np = Nanoparticle()
    np.setNLayers(4)
    np.setSphereLayerRelativeRadius(1, 0.3)
    np.setSphereLayerRelativeRadius(2, 0.15)
    np.setSphereLayerRelativeRadius(3, 0.2)
    # The 4th layer should now be set to 0.35
    assert_approx_equal(0.3,  np.getSphereLayerRelativeRadius(1))
    assert_approx_equal(0.15, np.getSphereLayerRelativeRadius(2))
    assert_approx_equal(0.2,  np.getSphereLayerRelativeRadius(3))
    assert_approx_equal(0.35, np.getSphereLayerRelativeRadius(4))
    # Adjust a layer and check result
    np.setSphereLayerRelativeRadius(2, 0.5)
    assert_approx_equal(0.3,  np.getSphereLayerRelativeRadius(1))
    assert_approx_equal(0.5,  np.getSphereLayerRelativeRadius(2))
    assert_approx_equal(0.0,  np.getSphereLayerRelativeRadius(3))
    assert_approx_equal(0.2, np.getSphereLayerRelativeRadius(4))
    # Adjust a layer and check result
    np.setSphereLayerRelativeRadius(4, 0.1)
    assert_approx_equal(0.3,  np.getSphereLayerRelativeRadius(1))
    assert_approx_equal(0.5,  np.getSphereLayerRelativeRadius(2))
    assert_approx_equal(0.1,  np.getSphereLayerRelativeRadius(3))
    assert_approx_equal(0.1, np.getSphereLayerRelativeRadius(4))
    # Adjust a layer and check result
    np.setSphereLayerRelativeRadius(3, 0.9)
    assert_approx_equal(0.1,  np.getSphereLayerRelativeRadius(1))
    assert_approx_equal(0.0,  np.getSphereLayerRelativeRadius(2))
    assert_approx_equal(0.9,  np.getSphereLayerRelativeRadius(3))
    assert_approx_equal(0.0, np.getSphereLayerRelativeRadius(4))
    # Make sure the errors work
    with raises(IndexError):
        np.setSphereLayerRelativeRadius(0, 0.5)
    with raises(IndexError):
        np.setSphereLayerRelativeRadius(11, 0.5)
    with raises(ValueError):
        np.setSphereLayerRelativeRadius(2, 1.1)
    with raises(ValueError):
        np.setSphereLayerRelativeRadius(2, -0.1)

def test_EllipsoidRadius():
    np = Nanoparticle()
    np.setEllipsoidRadius(30.0, 30.0)
    assert_approx_equal(30.0, np.getEllipsoidZRadius())
    assert_approx_equal(30.0, np.getEllipsoidXYRadius())
    np.setEllipsoidRadius(1.0, 15.0)
    assert_approx_equal(1.0, np.getEllipsoidZRadius())
    assert_approx_equal(15.0, np.getEllipsoidXYRadius())
    with raises(ValueError):
        np.setEllipsoidRadius(0.0, 15.0)
    with raises(ValueError):
        np.setEllipsoidRadius(15.0, 0.0)
    with raises(ValueError):
        np.setEllipsoidRadius(0.0, 0.0)
    assert_approx_equal(1.0, np.getEllipsoidZRadius())
    assert_approx_equal(15.0, np.getEllipsoidXYRadius())

def test_EllipsoidRelativeRadius():
    np = Nanoparticle()
    np.setNLayers(2)
    np.setEllipsoidLayerRelativeRadius(1, 0.6, 0.4)
    # The 4th layer should now be set to 0.35
    assert_approx_equal(0.6, np.getEllipsoidLayerZRelativeRadius(1))
    assert_approx_equal(0.4, np.getEllipsoidLayerXYRelativeRadius(1))
    assert_approx_equal(0.4, np.getEllipsoidLayerZRelativeRadius(2))
    assert_approx_equal(0.6, np.getEllipsoidLayerXYRelativeRadius(2))
    # Adjust a layer and check result
    np.setEllipsoidLayerRelativeRadius(2, 0.7, 0.6)
    assert_approx_equal(0.3, np.getEllipsoidLayerZRelativeRadius(1))
    assert_approx_equal(0.4, np.getEllipsoidLayerXYRelativeRadius(1))
    assert_approx_equal(0.7, np.getEllipsoidLayerZRelativeRadius(2))
    assert_approx_equal(0.6, np.getEllipsoidLayerXYRelativeRadius(2))
    # Adjust a layer and check result
    np.setEllipsoidLayerRelativeRadius(1, 1.0, 1.0)
    assert_approx_equal(1.0, np.getEllipsoidLayerZRelativeRadius(1))
    assert_approx_equal(1.0, np.getEllipsoidLayerXYRelativeRadius(1))
    assert_approx_equal(0.0, np.getEllipsoidLayerZRelativeRadius(2))
    assert_approx_equal(0.0, np.getEllipsoidLayerXYRelativeRadius(2))
    # Make sure the errors work
    with raises(IndexError):
        np.setEllipsoidLayerRelativeRadius(0, 0.5, 0.5)
    with raises(IndexError):
        np.setEllipsoidLayerRelativeRadius(11, 0.5, 0.5)
    with raises(ValueError):
        np.setEllipsoidLayerRelativeRadius(2, 1.1, 0.5)
    with raises(ValueError):
        np.setEllipsoidLayerRelativeRadius(2, -0.1, 0.5)
    with raises(ValueError):
        np.setEllipsoidLayerRelativeRadius(2, -0.1, 1.1)

def test_LayerMaterial():
    np = Nanoparticle()
    Al = "Al"
    np.setLayerMaterial(2, "Au")
    assert "Au" == np.getLayerMaterial(2)
    assert 4 == np.getLayerIndex(2)
    np.setLayerMaterial(4, Al)
    assert "Al" == np.getLayerMaterial(4)
    assert 1 == np.getLayerIndex(4)
    with raises(IndexError):
        np.setLayerMaterial(0, Al)
    with raises(IndexError):
        np.setLayerMaterial(11, Al)
    with raises(ValueError):
        np.setLayerMaterial(1, "Kryptonite")
    with raises(IndexError):
        np.getLayerMaterial(11)
    with raises(IndexError):
        np.getLayerIndex(11)

def test_Increment():
    np = Nanoparticle()
    np.setIncrement(5)
    assert 5 == np.getIncrement()
    with raises(ValueError):
        np.setIncrement(-1)
    with raises(ValueError):
        np.setIncrement(7)
    assert 5 == np.getIncrement()

def test_PathLength():
    np = Nanoparticle()
    np.setPathLength(5.0)
    assert_approx_equal(5.0, np.getPathLength())
    with raises(ValueError):
        np.setPathLength(0.0)
    assert_approx_equal(5.0, np.getPathLength())

def test_Concentration():
    np = Nanoparticle()
    np.setConcentration(1e-3)
    assert_approx_equal(1e-3, np.getConcentration())
    with raises(ValueError):
        np.setConcentration(0.0)
    assert_approx_equal(1e-3, np.getConcentration())

def test_MediumRefractiveIndex():
    np = Nanoparticle()
    np.setMediumRefractiveIndex(2.0)
    assert_approx_equal(2.0, np.getMediumRefractiveIndex())
    with raises(ValueError):
        np.setMediumRefractiveIndex(0.0)
    assert_approx_equal(2.0, np.getMediumRefractiveIndex())

def test_SizeCorrect():
    np = Nanoparticle()
    np.setSizeCorrect(True)
    assert np.getSizeCorrect()
    np.setSizeCorrect(False)
    assert not np.getSizeCorrect()

def test_DefaultCalculations():
    np = Nanoparticle()
    np.calculateSpectrum()
    spec = np.getSpectrum()
    assert_approx_equal(1.0739947924371145,    spec[0])
    assert_approx_equal(0.077512335915926767,  spec[250])
    assert_approx_equal(0.0042230869218847257, spec[500])

def test_MieLayers():
    np = Nanoparticle()
    np.setNLayers(3)
    np.setSphereRadius(20.0)
    np.setLayerMaterial(1, "Ag")
    np.setLayerMaterial(2, "Quartz")
    np.setLayerMaterial(3, "TiO2")
    np.setSphereLayerRelativeRadius(1, 0.2)
    np.setSphereLayerRelativeRadius(2, 0.3)
    np.calculateSpectrum()
    spec = np.getSpectrum()
    assert_approx_equal(1.6872545674341763, spec[0])
    assert_approx_equal(0.0167620500843786, spec[250])
    assert_approx_equal(0.0001270542643331, spec[500])

def test_QuasiLayers():
    np = Nanoparticle()
    np.setNLayers(2)
    np.setEllipsoidRadius(10.0, 10.0)
    np.setLayerMaterial(1, "Ag")
    np.setLayerMaterial(2, "Quartz")
    np.setEllipsoidLayerRelativeRadius(1, 0.6, 0.6)
    np.setShape(Ellipsoid)
    np.calculateSpectrum()
    spec = np.getSpectrum()
    assert_approx_equal(0.2136476694051971, spec[0])
    assert_approx_equal(0.1105467715317425, spec[250])
    assert_approx_equal(0.0022290667118399, spec[500])

def test_MediumRefractiveIndex():
    np = Nanoparticle()
    np.setSphereRadius(20.0)
    np.setSpectraProperty(Scattering)
    np.setMediumRefractiveIndex(2.0)
    np.calculateSpectrum()
    spec = np.getSpectrum()
    assert_approx_equal(1.0698792522825473, spec[0])
    assert_approx_equal(1.4426586799505641, spec[250])
    assert_approx_equal(0.0718406948561757, spec[500])

def test_Color():
    np = Nanoparticle()
    np.setSphereRadius(20.0)
    np.calculateSpectrum()
    r, g, b = np.getRGB()
    assert_approx_equal(0.12381521, r)
    assert_approx_equal(0.10589667, g)
    assert_approx_equal(0.48082513, b)
    h, s, v = np.getHSV()
    assert_approx_equal(242.86751,  h)
    assert_approx_equal(0.77976054, s)
    assert_approx_equal(0.48082513, v)
    assert_approx_equal(0.48082513, np.getOpacity())

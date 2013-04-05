# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.


"""
A python wrapper for the NPSpec C++ library
"""


from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_npspec', [dirname(__file__)])
        except ImportError:
            import _npspec
            return _npspec
        if fp is not None:
            try:
                _mod = imp.load_module('_npspec', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _npspec = swig_import_helper()
    del swig_import_helper
else:
    import _npspec
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


class Nanoparticle(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self): 
        this = _npspec.new_Nanoparticle()
        try: self.this.append(this)
        except: self.this = this
    def calculateSpectrum(self): return _npspec.Nanoparticle_calculateSpectrum(self)
    def getSpectrum(self): return _npspec.Nanoparticle_getSpectrum(self)
    def getRGB(self): return _npspec.Nanoparticle_getRGB(self)
    def getHSV(self): return _npspec.Nanoparticle_getHSV(self)
    def getOpacity(self): return _npspec.Nanoparticle_getOpacity(self)
    def setNLayers(self, *args): return _npspec.Nanoparticle_setNLayers(self, *args)
    def setShape(self, *args): return _npspec.Nanoparticle_setShape(self, *args)
    def setSpectraType(self, *args): return _npspec.Nanoparticle_setSpectraType(self, *args)
    def setSpectraProperty(self, *args): return _npspec.Nanoparticle_setSpectraProperty(self, *args)
    def setSphereRadius(self, *args): return _npspec.Nanoparticle_setSphereRadius(self, *args)
    def setEllipsoidRadius(self, *args): return _npspec.Nanoparticle_setEllipsoidRadius(self, *args)
    def setSphereLayerRelativeRadius(self, *args): return _npspec.Nanoparticle_setSphereLayerRelativeRadius(self, *args)
    def setEllipsoidLayerRelativeRadius(self, *args): return _npspec.Nanoparticle_setEllipsoidLayerRelativeRadius(self, *args)
    def setLayerMaterial(self, *args): return _npspec.Nanoparticle_setLayerMaterial(self, *args)
    def setIncrement(self, *args): return _npspec.Nanoparticle_setIncrement(self, *args)
    def setPathLength(self, *args): return _npspec.Nanoparticle_setPathLength(self, *args)
    def setConcentration(self, *args): return _npspec.Nanoparticle_setConcentration(self, *args)
    def setSizeCorrect(self, *args): return _npspec.Nanoparticle_setSizeCorrect(self, *args)
    def setMediumRefractiveIndex(self, *args): return _npspec.Nanoparticle_setMediumRefractiveIndex(self, *args)
    def getNLayers(self): return _npspec.Nanoparticle_getNLayers(self)
    def getShape(self): return _npspec.Nanoparticle_getShape(self)
    def getSpectraType(self): return _npspec.Nanoparticle_getSpectraType(self)
    def getSpectraProperty(self): return _npspec.Nanoparticle_getSpectraProperty(self)
    def getSphereRadius(self): return _npspec.Nanoparticle_getSphereRadius(self)
    def getEllipsoidZRadius(self): return _npspec.Nanoparticle_getEllipsoidZRadius(self)
    def getEllipsoidXYRadius(self): return _npspec.Nanoparticle_getEllipsoidXYRadius(self)
    def getSphereLayerRelativeRadius(self, *args): return _npspec.Nanoparticle_getSphereLayerRelativeRadius(self, *args)
    def getEllipsoidLayerZRelativeRadius(self, *args): return _npspec.Nanoparticle_getEllipsoidLayerZRelativeRadius(self, *args)
    def getEllipsoidLayerXYRelativeRadius(self, *args): return _npspec.Nanoparticle_getEllipsoidLayerXYRelativeRadius(self, *args)
    def getLayerMaterial(self, *args): return _npspec.Nanoparticle_getLayerMaterial(self, *args)
    def getLayerIndex(self, *args): return _npspec.Nanoparticle_getLayerIndex(self, *args)
    def getIncrement(self): return _npspec.Nanoparticle_getIncrement(self)
    def getPathLength(self): return _npspec.Nanoparticle_getPathLength(self)
    def getConcentration(self): return _npspec.Nanoparticle_getConcentration(self)
    def getSizeCorrect(self): return _npspec.Nanoparticle_getSizeCorrect(self)
    def getMediumRefractiveIndex(self): return _npspec.Nanoparticle_getMediumRefractiveIndex(self)
    __swig_destroy__ = _npspec.delete_Nanoparticle
    __del__ = lambda self : None;
Nanoparticle_swigregister = _npspec.Nanoparticle_swigregister
Nanoparticle_swigregister(Nanoparticle)

NLAMBDA = _npspec.NLAMBDA
MAXLAYERS = _npspec.MAXLAYERS
Efficiency = _npspec.Efficiency
CrossSection = _npspec.CrossSection
Molar = _npspec.Molar
Absorption = _npspec.Absorption
Sphere = _npspec.Sphere
Ellipsoid = _npspec.Ellipsoid
Extinction = _npspec.Extinction
Absorbance = _npspec.Absorbance
Scattering = _npspec.Scattering

def _get_wavelengths():
  return _npspec._get_wavelengths()
_get_wavelengths = _npspec._get_wavelengths
# Make the wavelengths a read-only module-level numpy array
wavelengths = _get_wavelengths()
wavelengths.flags.writeable = False



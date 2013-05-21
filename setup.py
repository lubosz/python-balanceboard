from distutils.core import setup, Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize
 
#module1 = Extension('wiic', sources = ['wiic.c'], libraries=["wiic"])
 
#setup (name = 'WiiC',
#        version = '1.0',
#        description = 'Python WiiC Wrapper',
#        cmdclass = {"build_ext": build_ext},
#        ext_modules = [module1])

module1 = Extension(
  'wiicpp',
  sources = ['python-wiicpp.pyx'], 
  libraries=["wiicpp"],
  include_dirs=['/usr/include/wiic'],
  language="c++")

setup(name = 'wiicpp',
        version = '1.0',
        description = 'Python WiiC Wrapper',
        cmdclass = {"build_ext": build_ext},
        ext_modules = cythonize([module1]))

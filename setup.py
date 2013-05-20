from distutils.core import setup, Extension
 
module1 = Extension('wiic', sources = ['wiic.c'])
 
setup (name = 'WiiC',
        version = '1.0',
        description = 'Python WiiC Wrapper',
        ext_modules = [module1])

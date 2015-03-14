from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension

setup(
    ext_modules=cythonize(
        [Extension("engine", ['engine.pyx'], 
#                    language='c++', 
                   include_dirs=['../src'],
#                    extra_compile_args=['-std=c++11'],#,'-stdlib=libc++'],
#                    library_dirs=['../../../lib'],
#                    libraries=['engine'],
        )]
    )
)


from skbuild import setup

setup(
    name='py-klotski',
    version='0.0.1',
    description='Klotski multifunctional engine with high performance',
    author='Dnomd343',
    author_email='dnomd343@gmail.com',
    license='BSD-3-Clause',
    url='https://github.com/dnomd343/klotski',
    packages=['klotski'],
    package_dir={'': 'src'},
    cmake_install_dir='src/klotski',
    cmake_args=[
        '-DKLSK_ENABLE_TESTING:BOOL=OFF',
        '-DKLSK_ENABLE_BENCHMARK:BOOL=OFF',
        '-DKLSK_C_FFI:BOOL=OFF',
        '-DKLSK_RUST_FFI:BOOL=OFF',
        '-DKLSK_PYTHON_FFI:BOOL=ON',
    ],
    python_requires=">=3.7",
)

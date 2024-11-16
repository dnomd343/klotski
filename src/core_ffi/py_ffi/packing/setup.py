from skbuild import setup

setup(
    name="py-klotski",
    version="0.0.1",
    description="klotski engine",
    author="Dnomd343",
    license="MIT",
    packages=["klotski"],
    package_dir={"": "src"},
    cmake_install_dir="src/klotski",
    cmake_args=['-DKLSK_ENABLE_TESTING=OFF', '-DKLSK_ENABLE_BENCHMARK=OFF', '-DKLSK_C_FFI=OFF', '-DKLSK_RUST_FFI=OFF', '-DKLSK_PYTHON_FFI=ON'],
    python_requires=">=3.9",
)

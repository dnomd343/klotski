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
    python_requires=">=3.9",
)

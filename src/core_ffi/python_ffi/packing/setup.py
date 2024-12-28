#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from skbuild import setup

setup(
    packages=['klotski'],
    package_dir={'': 'src'},
    cmake_install_dir='src/klotski',
    cmake_args=[
        '-DKLSK_ENABLE_LTO:BOOL=ON',
        '-DKLSK_PYTHON_FFI:BOOL=ON',
    ]
)

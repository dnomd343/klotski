#!/usr/bin/env python3

import ctypes

lib = ctypes.CDLL('../cmake-build-release/src/klotski/libklotski.so')

lib.tmain()

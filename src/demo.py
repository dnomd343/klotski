#!/usr/bin/env python3

import ctypes

lib = ctypes.CDLL('../cmake-build-release/src/klotski_core/libklotski.so')

lib.tmain()

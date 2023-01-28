#!/usr/bin/env python3

import ctypes

lib = ctypes.CDLL('../bin/libklotski.so')

lib.tmain()

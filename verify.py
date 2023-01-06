#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import subprocess

process_list = []
CASE_NUMBER = 29334
# CASE_NUMBER = 29334498
thread_num = int(sys.argv[1])
sub_length = ((CASE_NUMBER - 1) // thread_num) + 1


def run_check(start_code: int, end_code: int) -> None:
    print('[%d, %d] -> %d' % (start_code, end_code, end_code - start_code + 1))
    process_list.append(
        subprocess.Popen(['./klotski', str(start_code), str(end_code)])
    )


start = 0
for i in range(0, thread_num):
    if i + 1 == thread_num:
        end = CASE_NUMBER - 1
    else:
        end = start + sub_length - 1
    run_check(start, end)
    start += sub_length

print('-' * 64)

for process in process_list:
    process.wait()

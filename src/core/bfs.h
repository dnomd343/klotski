#pragma once

#define BFS_INIT \
int next_addr; \
cache_size = 1; \
int current = 0;  \
cache[0].filter = 0; \
cache[0].addr = addr; \
cache[0].code = code;

#define BFS_LOAD \
code = cache[current].code; \
addr = cache[current].addr; \
int filter = cache[current++].filter;

#define BFS_STOP (current == cache_size)

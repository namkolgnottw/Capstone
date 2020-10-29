#!/bin/sh
clear
gcc http_server.c thread_pool.c network.c file_process.c  lru_cache.c cpu_usage.c memory_usage.c -lpthread


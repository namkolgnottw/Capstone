#!/bin/sh
gcc cache_server.c file_process.c network.c  -lpthread -o CacheServer


#!/bin/sh
clear
gcc hls_dlr_simulator.c check_status.c network.c file_process.c -lpthread -o simulator2


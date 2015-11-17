#!/bin/bash

set -e

gcc --std=c99 -I.. ../subdirs.c benchmark_subdirs.c -o benchmark_subdirs
time ./benchmark_subdirs

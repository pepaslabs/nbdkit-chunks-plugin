#!/bin/bash

set -e

gcc --std=c99 -I.. ../subdirs.c test_subdirs.c -o test_subdirs
./test_subdirs

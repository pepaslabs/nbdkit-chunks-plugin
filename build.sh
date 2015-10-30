#!/bin/sh

set -e

name=$(basename *.c .c)

gcc \
-std=gnu99 \
-DHAVE_CONFIG_H \
-I. -I../.. -I../../include \
-g -O2 \
-c ${name}.c \
-fPIC \
-DPIC \
-o nbdkit-${name}-plugin.o

gcc \
-shared \
-fPIC \
-DPIC \
nbdkit-${name}-plugin.o \
-O2 \
-Wl,-soname \
-Wl,nbdkit-${name}-plugin.so \
-o nbdkit-${name}-plugin.so

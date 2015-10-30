#!/bin/sh

set -e

name=chunks

for i in chunks_nbdkit
do
    gcc -g -O2 -std=gnu99 -fPIC -DPIC \
        -DHAVE_CONFIG_H -I. -I../.. -I../../include \
        -o ${i}.o -c ${i}.c
done

gcc -shared -O2 -fPIC -DPIC \
-Wl,-soname \
-Wl,nbdkit-${name}-plugin.so \
-o nbdkit-${name}-plugin.so \
chunks_nbdkit.o \

#!/bin/sh

set -e

rm -f *.o *.so

cd mkbd.chunks
./clean.sh
cd - >/dev/null

cd lsbd.chunks
./clean.sh
cd - >/dev/null

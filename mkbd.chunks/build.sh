#!/bin/bash

set -e -o pipefail

for c in *.c
do
    o=$( basename "${c}" .c )
    gcc -std=gnu99 -I.. -o ${o}.o -c ${c}
done

gcc -std=gnu99 -o mkbd.chunks *.o

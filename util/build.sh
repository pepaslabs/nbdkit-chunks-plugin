#!/bin/bash

set -e -o pipefail

if which gengetopt >/dev/null 2>&1
then
    cat cmdline.ggo | gengetopt
fi

for c in *.c
do
    o=$( basename "${c}" .c )
    gcc -std=gnu99 -I.. -o ${o}.o -c ${c}
done

gcc -std=gnu99 -o mkbd.chunks *.o

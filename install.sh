#!/bin/sh

set -e

name=$(basename *.c .c)

sudo cp nbdkit-${name}-plugin.so /usr/lib/x86_64-linux-gnu/nbdkit/plugins/
sudo chown root:root /usr/lib/x86_64-linux-gnu/nbdkit/plugins/nbdkit-${name}-plugin.so

#!/bin/sh

set -e

name=chunks

if [ -d /usr/lib/arm-linux-gnueabihf/nbdkit/plugins ]
then
    dest=/usr/lib/arm-linux-gnueabihf/nbdkit/plugins
elif [ -d /usr/lib/x86_64-linux-gnu/nbdkit/plugins ]
then
    dest=/usr/lib/x86_64-linux-gnu/nbdkit/plugins
else
    echo "nowhere to install to." >&2
    exit 1
fi

sudo cp nbdkit-${name}-plugin.so ${dest}/
sudo chown root:root ${dest}/nbdkit-${name}-plugin.so

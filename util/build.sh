#!/bin/sh

set -e

gcc -std=gnu99 -I.. -o mkbd.chunks mkbd.chunks.c

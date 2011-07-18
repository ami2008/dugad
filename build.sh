#!/bin/bash
gcc -I/usr/include/netpbm -L/usr/lib -lnetpbm -lm -o embed embed.c
gcc -I/usr/include/netpbm -L/usr/lib -lnetpbm -lm -o detect detect.c
gcc -I/usr/include/netpbm -L/usr/lib -lnetpbm -lm -o compare compare.c

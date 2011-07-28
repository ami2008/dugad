#!/bin/bash
gcc -I/opt/local/include -L/opt/local/lib -lnetpbm -lm -o embed embed.c
gcc -I/opt/local/include -L/opt/local/lib -lnetpbm -lm -o detect detect.c
gcc -I/opt/local/include -L/opt/local/lib -lnetpbm -lm -o compare compare.c

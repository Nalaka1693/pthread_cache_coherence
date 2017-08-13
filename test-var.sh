#!/usr/bin/env bash
gcc -w -o testv test-var.c
./testv.exe
sleep 5
rm -rf testv.exe

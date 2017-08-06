#!/usr/bin/env bash
gcc -w -o test test.c
./test.exe
sleep 5
rm -rf test.exe

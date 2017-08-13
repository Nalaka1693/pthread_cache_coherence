#!/usr/bin/env bash
gcc -w -o testa test-arr.c
./testa.exe
sleep 5
rm -rf testa.exe

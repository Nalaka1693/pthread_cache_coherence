#!/usr/bin/env bash
gcc -w -o testa test-arr.c
unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   ./testa
    sleep 5
    rm -rf testa
elif [[ "$unamestr" == 'MINGW64_NT-10.0' ]]; then
    ./testa.exe
    sleep 5
    rm -rf testa.exe
fi

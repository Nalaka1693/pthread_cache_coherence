#!/usr/bin/env bash
cd ../
gcc -o testv test-var.c -lpthread
unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   ./testv
    sleep 5
    rm -rf testv
elif [[ "$unamestr" == 'MINGW64_NT-10.0' ]]; then
    ./testv.exe
    sleep 5
    rm -rf testv.exe
fi

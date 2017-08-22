#!/usr/bin/env bash
cd ../
gcc -o cl cache_line.c -lpthread
unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   ./cl
    sleep 5
    rm -rf cl
elif [[ "$unamestr" == 'MINGW64_NT-10.0' ]]; then
    ./cl.exe
    sleep 5
    rm -rf cl.exe
fi

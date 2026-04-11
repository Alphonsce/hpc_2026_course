#!/usr/bin/env bash

cd demos
libomp_path=/opt/homebrew/opt/libomp

## 1:
gcc-15 Hello.c -o hello -fopenmp
OMP_NUM_THREADS=4 ./hello
rm hello

## 2:
g++-15 -std=c++17 -fopenmp OutMes.cpp -o outmes
OMP_NUM_THREADS=4 ./outmes
./outmes
rm outmes

## 3:
gcc-15 PrivateShared.c -o private_shared -fopenmp
OMP_NUM_THREADS=4 ./private_shared
rm private_shared
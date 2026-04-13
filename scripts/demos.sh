cd demos

## 1:
# gcc-15 Hello.c -o hello -fopenmp
# OMP_NUM_THREADS=4 ./hello
# rm hello

## 2:
# g++-15 -std=c++17 -fopenmp OutMes.cpp -o outmes
# OMP_NUM_THREADS=4 ./outmes
# ./outmes
# rm outmes

## 3:
# gcc-15 PrivateShared.c -o private_shared -fopenmp
# OMP_NUM_THREADS=4 ./private_shared
# rm private_shared

## 4:
# gcc-15 ParSec.c -o parsec -fopenmp
# OMP_NUM_THREADS=4 ./parsec
# rm parsec

## 5:
gcc-15 SumArray.c -o sumarray -fopenmp
./sumarray
rm sumarray
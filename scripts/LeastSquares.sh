#!/usr/bin/env bash
mkdir -p results

cd hw1
gcc-15 LeastSquares.c -O2 -o least_squares -fopenmp -lm

echo "N,threads,time_seconds" > ../results/least_squares.txt

for n in 100 1000 10000 100000 1000000 10000000
do
    for threads in 1 2 4 6 8 10 12 14 16
    do
        output="$(OMP_NUM_THREADS=$threads ./least_squares $n)"
        time_s="$(echo "$output" | awk '/^time =/ {print $3}')"
        echo "$n,$threads,$time_s" >> ../results/least_squares.txt
    done
done

rm least_squares

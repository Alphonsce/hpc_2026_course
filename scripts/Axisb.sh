mkdir -p results

cd hw1
gcc-15 Axisb.c -o axisb -fopenmp -lm

echo "threads,time_seconds" > ../results/axisb.txt

for threads in 1 2 4 6 8 10 12 14 16 20 24 28 30 32
do
    output="$(OMP_NUM_THREADS=$threads ./axisb)"
    time_s="$(echo "$output" | awk '/^time =/ {print $3}')"
    echo "$threads,$time_s" >> ../results/axisb.txt
done

rm axisb

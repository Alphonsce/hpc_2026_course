mkdir -p results

cd hw1
gcc-15 Pi.c -o pi -fopenmp

echo "threads,time_seconds,pi_output" >> ../results/pi.txt

for threads in 1 2 4 6 8 10 12 14 16 20 24 28 30 32
do
    output="$(OMP_NUM_THREADS=$threads ./pi)"
    time_s="$(echo "$output" | awk '/^time =/ {print $3}')"
    pi_out="$(echo "$output" | awk '/^pi =/ {print $0}')"
    echo "$threads,$time_s,$pi_out" >> ../results/pi.txt
done

rm pi
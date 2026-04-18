cd hw1

g++-15 Car.cpp -o car -fopenmp
./car
rm car

### Measure how save frequency affects runtime
g++-15 Car.cpp -o car -fopenmp
mkdir -p ../results
echo "save_every runtime_sec" > ../results/car_save_frequency.txt

for save_every in 1 2 5 10 20 50 100 300
do
  rm -rf frames
  ./car $save_every | awk -v s=$save_every '/^time =/ {print s, $3}' >> ../results/car_save_frequency.txt
done

rm car


### Create GIF from frames
# mkdir -p frames_png
# magick mogrify -path frames_png -format png "frames/frame_*.ppm"
# ffmpeg -framerate 12 -pattern_type glob -i 'frames_png/frame_*.png' car_animation.gif
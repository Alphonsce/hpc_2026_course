#include <fstream>
#include <iostream>
#include <omp.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <thread>

#define RGB_COMPONENT_COLOR 255

static const auto THREADS = std::thread::hardware_concurrency();

struct PPMPixel {
  int red;
  int green;
  int blue;
};

typedef struct {
  int x, y, all;
  PPMPixel *data;
} PPMImage;

void readPPM(const char *filename, PPMImage &img){
    std::ifstream file(filename);
    if (file) {
        std::string s;
        int rgb_comp_color;
        file >> s;
        if (s != "P3") {
            std::cout << "error in format" << std::endl;
            exit(9);
        }
        file >> img.x >> img.y;
        file >> rgb_comp_color;
        img.all = img.x * img.y;
        std::cout << s << std::endl;
        std::cout << "x=" << img.x << " y=" << img.y << " all=" << img.all
                  << std::endl;
        img.data = new PPMPixel[img.all];
        for (int i = 0; i < img.all; i++) {
            file >> img.data[i].red >> img.data[i].green >> img.data[i].blue;
        }
    } else {
        std::cout << "the file:" << filename << "was not found" << std::endl;
    }
    file.close();
}

void writePPM(const std::string &filename, const PPMImage &img) {
    std::ofstream file(filename, std::ofstream::out);
    file << "P3" << std::endl;
    file << img.x << " " << img.y << " " << std::endl;
    file << RGB_COMPONENT_COLOR << std::endl;

    for (int i = 0; i < img.all; i++) {
        file << img.data[i].red << " " << img.data[i].green << " "
             << img.data[i].blue << (((i + 1) % img.x == 0) ? "\n" : " ");
    }
    file.close();
}

int main(int argc, char *argv[]) {
    int save_every = 20;
    if (argc > 1) {
        save_every = std::atoi(argv[1]);
        if (save_every <= 0) {
            save_every = 20;
        }
    }

    PPMImage image;
    readPPM("car.ppm", image);

    const int width = image.x;
    const int height = image.y;
    const int steps = width;
    PPMPixel *with_extra_col = new PPMPixel[height * (width + 1)];
    mkdir("frames", 0777);

    double t1 = omp_get_wtime();
    for (int step = 0; step < steps; ++step) {
        #pragma omp parallel for
        for (int i = 0; i < height; ++i) {
            const int row_src = i * width;
            const int row_dst = i * (width + 1);

            for (int j = 0; j < width; ++j) {
                with_extra_col[row_dst + (j + 1)] = image.data[row_src + j];
            }
            with_extra_col[row_dst] = image.data[row_src + (width - 1)];

            for (int j = 0; j < width; ++j) {
                image.data[row_src + j] = with_extra_col[row_dst + j];
            }
        }

        if (step % save_every == 0) {
            writePPM("frames/frame_" + std::to_string(step) + ".ppm", image);
        }
    }

    double t2 = omp_get_wtime();
    writePPM("new_car.ppm", image);
    std::cout << "threads = " << THREADS << std::endl;
    std::cout << "time = " << (t2 - t1) << " sec" << std::endl;

    delete[] with_extra_col;
    delete[] image.data;
    return 0;
}





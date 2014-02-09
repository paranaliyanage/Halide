#include <static_image.h>
#include <image_io.h>
#include <sys/time.h>

#define TIMES 20
struct timeval start, end;

int print(Image<float> input) {
	for (int w = 0; w < input.size(3); w++) {
		for (int z = 0; z < input.channels(); z++) {
			for (int y = 0; y < input.height(); y++) {
				for (int x = 0; x < input.width(); x++) {
					printf("value in %d %d %d %d is %f\n", w, y, x, z,
							input(x, y, z, w));
				}
			}
		}
	}
	return 0;
}

extern "C" {
#include "heat_3d_np.h"
}
Image<float> halide_heat_3d(Image<float> input) {
	Image<float> output(input.width(), input.height(), input.channels(),
			input.size(3));
	heat_3d_np(input, output);
	gettimeofday(&start, 0);
	for (int i = 0; i < TIMES; ++i) {
		heat_3d_np(input, output);
	}
	gettimeofday(&end, 0);
	return output;
}

int main(int argc, char **argv) {
	const int BASE = 1024;
	if (argc < 3) {
		printf("argc < 3, quit! \n");
		exit(1);
	}
	int N_SIZE = atoi(argv[1]);
	int T_SIZE = atoi(argv[2]);
	printf("N_SIZE = %d, T_SIZE = %d\n", N_SIZE, T_SIZE);
	Image<float> input = Image<float>(N_SIZE, N_SIZE, N_SIZE, T_SIZE);

	for (int x = 0; x < input.width(); ++x) {
		for (int y = 0; y < input.height(); ++y) {
			for (int z = 0; z < input.channels(); ++z) {
				if (x == 0 || x == input.width() - 1 || y == 0
						|| y == input.height() - 1 || z == 0
						|| z == input.channels() - 1) {
					input(x, y, z, 0) = 0;
				} else {
					input(x, y, z, 0) = 1.0 * (rand() % BASE);
				}
				input(x, y, z, 1) = 0;
			}
		}
	}

	//print(input);

	Image<float> output = halide_heat_3d(input);

	float halide_time = ((end.tv_sec - start.tv_sec)
			+ (end.tv_usec - start.tv_usec) / 100000000.0f) / TIMES;

	//print(output);

	printf("Halide consumed time : %f ms\n", halide_time);

	return 0;
}

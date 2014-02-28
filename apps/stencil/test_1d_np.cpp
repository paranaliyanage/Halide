#include <static_image.h>
#include <image_io.h>
#include <sys/time.h>
#include <cmath>


#define TIMES 20
struct timeval start, end;

int print(Image<float> input) {
	for (int y = 0; y < input.height(); y++) {
		for (int x = 0; x < input.width(); x++) {
			printf("value in %d %d is %f\n", y, x, input(x, y));
		}
	}
	return 0;
}

extern "C" {
#include "heat_1d_np.h"
}Image<float> halide_heat_1d(Image<float> input) {
	Image<float> output(input.width());
	heat_1d_np(input, output);
	float halide_time = std::numeric_limits<float>::max();
	for (int i = 0; i < TIMES; ++i) {
		gettimeofday(&start, 0);
		heat_1d_np(input, output);
		gettimeofday(&end, 0);
		halide_time = std::min(halide_time,
				1.0e3
						* ((end.tv_sec - start.tv_sec)
								+ (end.tv_usec - start.tv_usec) / 1000000.0f));
	}
	printf("Halide consumed time : %f ms\n", halide_time);
	return output;
}

int main(int argc, char **argv) {
	const int BASE = 1024;
	if (argc < 2) {
		printf("argc < 2, quit! \n");
		exit(1);
	}
	int N_SIZE = atoi(argv[1]);
	printf("N_SIZE = %d \n", N_SIZE);
	Image<float> input = Image<float>(N_SIZE);

	for (int x = 0; x < input.width(); x++) {
		input(x) = 1.0 * (rand() % BASE);
		//input(x, 0) = 1.0f;
	}

	//print(input);

	Image<float> output = halide_heat_1d(input);

	//print(output);

	return 0;
}

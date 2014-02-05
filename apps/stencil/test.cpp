#include <static_image.h>
#include <image_io.h>
#include <sys/time.h>

#define TIMES 100
struct timeval start, end;

int print(Image<float> input, int row) {
	if (row == -1) {
		for (int y = 0; y < input.height(); y++) {
			for (int x = 0; x < input.width(); x++) {
				printf("%f\n", input(x, y));
			}
		}
	} else {
		for (int x = 0; x < input.width(); x++) {
			printf("%f\n", input(x, row));
		}
	}
	return 0;
}

extern "C" {
#include "heat_1d.h"
}

Image<float> heat_1d(Image<float> input) {
	Image<float> output(input.width(), input.height());
	heat_1d(input, output);
	gettimeofday(&start, 0);
	for (int i = 0; i < TIMES; ++i) {
		heat_1d(input, output);
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
	Image<float> input = Image<float>(N_SIZE, T_SIZE);

	for (int y = 0; y < input.height(); y++) {
		for (int x = 0; x < input.width(); x++) {
			if (y == 0) {
				input(x, y) = 1.0 * (rand() % BASE);
				//input(x, y) = 1.0 * (y * input.width() + x);
			} else {
				input(x, y) = 0;
			}
		}
	}

	print(input,0);
	printf("Input is printed now \n");

	Image<float> output = heat_1d(input);

	float halide_time = ((end.tv_sec - start.tv_sec)
			+ (end.tv_usec - start.tv_usec) / 100000000.0f) / TIMES;

	print(output,31);

	printf("time is %f\n", halide_time);

	return 0;
}

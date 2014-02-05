#include <Halide.h>
using namespace Halide;

int main(int argc, char **argv) {

	ImageParam in_img(Float(32), 2);
	Func heat_x("heat_x"), heat_y("heat_y");
	Var x("x"), y("y");

	Func input;
	input(x, y) = in_img(clamp(x, 1, in_img.width() - 2),
			clamp(y, 1, in_img.height() - 1));

	// The algorithm
	heat_x(x, y) = 0.125f
			* (input(x - 1, y) - 2.0f * input(x, y) + input(x + 1, y));
	heat_y(x, y) = heat_x(x, y - 1);

	// The schedule
	Halide::Var _x2, _x4, _y5;
	input.reorder(x, y).reorder_storage(y, x).vectorize(x, 4).compute_at(heat_y,
			_y5);
	heat_x.split(x, x, _x2, 32).reorder(_x2, x, y).reorder_storage(x, y).vectorize(
			_x2, 4).compute_at(heat_y, _y5);
	heat_y.split(x, x, _x4, 32).split(y, y, _y5, 8).reorder(_x4, _y5, x, y).reorder_storage(
			y, x).vectorize(_x4, 8).compute_root();

	heat_y.compile_to_file("heat_1d", in_img);

	return 0;
}

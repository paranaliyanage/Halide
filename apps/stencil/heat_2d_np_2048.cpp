#include <Halide.h>
using namespace Halide;

Func heat_step(Func input, Expr max_x, Expr max_y) {
	Var x("x"), y("y");
	Func heat_0("heat_0"), clamped_0("clamped_0"), heat_1("heat_1"), clamped_1(
			"clamped_1"), heat_2("heat_0"), clamped_2("clamped_0");
	clamped_0(x, y) = input(clamp(x, 1, max_x), clamp(y, 1, max_y));
	heat_0(x, y) = 0.125f
			* (clamped_0(x + 1, y) - 2.0f * clamped_0(x, y)
					+ clamped_0(x - 1, y))
			+ 0.125f
					* (clamped_0(x, y + 1) - 2.0f * clamped_0(x, y)
							+ clamped_0(x, y - 1)) + clamped_0(x, y);

	clamped_1(x, y) = heat_0(clamp(x, 1, max_x), clamp(y, 1, max_y));
	heat_1(x, y) = 0.125f
			* (clamped_1(x + 1, y) - 2.0f * clamped_1(x, y)
					+ clamped_1(x - 1, y))
			+ 0.125f
					* (clamped_1(x, y + 1) - 2.0f * clamped_1(x, y)
							+ clamped_1(x, y - 1)) + clamped_1(x, y);
	clamped_2(x, y) = heat_1(clamp(x, 1, max_x), clamp(y, 1, max_y));
	heat_2(x, y) = 0.125f
			* (clamped_2(x + 1, y) - 2.0f * clamped_2(x, y)
					+ clamped_2(x - 1, y))
			+ 0.125f
					* (clamped_2(x, y + 1) - 2.0f * clamped_2(x, y)
							+ clamped_2(x, y - 1)) + clamped_2(x, y);

	Halide::Var _x0, _y1, _x2, _y3, _x4, _y5, _x6, _y7, _x8, _y9, _x10, _y11;
	clamped_0.split(x, x, _x0, 4).split(y, y, _y1, 8).reorder(_y1, y, _x0, x).reorder_storage(
			x, y).compute_root();
	heat_0.split(x, x, _x2, 16).split(y, y, _y3, 2).reorder(_x2, x, _y3, y).reorder_storage(
			x, y).vectorize(_x2, 4).compute_at(clamped_1, x);
	clamped_1.split(x, x, _x4, 4).split(y, y, _y5, 2).reorder(_y5, y, _x4, x).reorder_storage(
			x, y).compute_at(heat_1, y);
	heat_1.split(x, x, _x6, 16).split(y, y, _y7, 2).reorder(_x6, _y7, y, x).reorder_storage(
			x, y).vectorize(_x6, 2).parallel(x).compute_root();
	clamped_2.split(x, x, _x8, 2).split(y, y, _y9, 8).reorder(_y9, y, _x8, x).reorder_storage(
			x, y).vectorize(_y9, 4).compute_at(heat_2, y);
	heat_2.split(x, x, _x10, 64).split(y, y, _y11, 8).reorder(_x10, _y11, x, y).reorder_storage(
			x, y).vectorize(_x10, 8).parallel(y).compute_root();

	return heat_2;

}

int main(int argc, char **argv) {
	ImageParam input(Float(32), 2);
	const unsigned int steps = 6;
	const unsigned int macro_steps = steps / 3;
	Var x("x"), y("y");
	Func heat[steps + 1];
	heat[0](x, y) = input(x, y);
	for (int t = 1; t <= macro_steps; t++) {
		heat[t] = heat_step(heat[t - 1], input.width() - 2, input.height() - 2);
	}
	//heat[macro_steps].compute_root();

	heat[macro_steps].compile_to_file("heat_2d_np", input);
	return 0;
}

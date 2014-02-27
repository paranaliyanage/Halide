#include <Halide.h>
using namespace Halide;

Func heat_step(Func input, Expr max_val) {

	Var x("x");
	Func heat_0("heat_0"), clamped_0("clamped_0"), heat_1("heat_1"), clamped_1(
			"clamped_1"), heat_2("heat_0"), clamped_2("clamped_0");
	clamped_0(x) = input(clamp(x, 1, max_val));
	heat_0(x) = 0.125f
			* (clamped_0(x - 1) - 2.0f * clamped_0(x) + clamped_0(x + 1));
	clamped_1(x) = heat_0(clamp(x, 1, max_val));
	heat_1(x) = 0.125f
			* (clamped_1(x - 1) - 2.0f * clamped_1(x) + clamped_1(x + 1));
	clamped_2(x) = heat_1(clamp(x, 1, max_val));
	heat_2(x) = 0.125f
			* (clamped_2(x - 1) - 2.0f * clamped_2(x) + clamped_2(x + 1));

	Halide::Var _x2, _x6, _y7, _x10, _y11;
	clamped_0.reorder(y, x).reorder_storage(y, x).compute_at(heat_1, y);
	heat_0.split(x, x, _x2, 16).reorder(_x2, x, y).reorder_storage(y, x).vectorize(
			_x2, 8).compute_at(heat_1, _y7);
	clamped_1.reorder(y, x).reorder_storage(x, y).vectorize(y, 8).compute_at(
			heat_1, _x6);
	heat_1.split(x, x, _x6, 8).split(y, y, _y7, 4).reorder(_x6, _y7, x, y).reorder_storage(
			x, y).vectorize(_x6, 2).parallel(y).compute_root();
	clamped_2.reorder(x, y).reorder_storage(x, y).vectorize(x, 4).compute_at(
			heat_2, x);
	heat_2.split(x, x, _x10, 64).split(y, y, _y11, 4).reorder(_x10, _y11, x, y).reorder_storage(
			y, x).vectorize(_x10, 8).parallel(y).compute_root();

	return heat_2;

}

int main(int argc, char **argv) {
	ImageParam input(Float(32), 1);
	const unsigned int steps = 6;
	const unsigned int macro_steps = steps / 3;
	Var x("x");
	Func heat[steps + 1];
	heat[0](x) = input(x);
	for (int t = 1; t <= macro_steps; t++) {
		heat[t] = heat_step(heat[t - 1], input.width() - 2);
	}
	//heat[macro_steps].compute_root();

	heat[macro_steps].compile_to_file("heat_1d_np", input);
	return 0;
}

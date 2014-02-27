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
	Halide::Var _x1, _x4, _x5;
	clamped_0.compute_root();
	heat_0.split(x, x, _x1, 16).reorder(_x1, x).vectorize(_x1, 2).compute_root();
	clamped_1.vectorize(x, 2).compute_at(heat_1, x);
	heat_1.vectorize(x, 8).compute_root();
	clamped_2.split(x, x, _x4, 8).reorder(_x4, x).vectorize(_x4, 4).compute_at(
			heat_2, x);
	heat_2.split(x, x, _x5, 64).reorder(_x5, x).vectorize(_x5, 8).compute_root();
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

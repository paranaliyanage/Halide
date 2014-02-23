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
	heat[macro_steps].compute_root();

	heat[macro_steps].compile_to_file("heat_1d_np", input);
	return 0;
}

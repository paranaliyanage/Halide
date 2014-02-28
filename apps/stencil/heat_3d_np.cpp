#include <Halide.h>
using namespace Halide;

Func heat_step(Func input, Expr max_x, Expr max_y, Expr max_z) {
	Var x("x"), y("y"), z("z");
	Func heat_0("heat_0"), clamped_0("clamped_0"), heat_1("heat_1"), clamped_1(
			"clamped_1"), heat_2("heat_0"), clamped_2("clamped_0");

	clamped_0(x, y, z) = input(clamp(x, 1, max_x), clamp(y, 1, max_y),
			clamp(z, 1, max_z));

	heat_0(x, y, z) = 0.125f
			* (clamped_0(x + 1, y, z) - 2.0f * clamped_0(x, y, z)
					+ clamped_0(x - 1, y, z))
			+ 0.125f
					* (clamped_0(x, y + 1, z) - 2.0f * clamped_0(x, y, z)
							+ clamped_0(x, y - 1, z))
			+ 0.125f
					* (clamped_0(x, y, z + 1) - 2.0f * clamped_0(x, y, z)
							+ clamped_0(x, y, z - 1)) + clamped_0(x, y, z);

	clamped_1(x, y, z) = heat_0(clamp(x, 1, max_x), clamp(y, 1, max_y),
			clamp(z, 1, max_z));
	heat_1(x, y, z) = 0.125f
			* (clamped_1(x + 1, y, z) - 2.0f * clamped_1(x, y, z)
					+ clamped_1(x - 1, y, z))
			+ 0.125f
					* (clamped_1(x, y + 1, z) - 2.0f * clamped_1(x, y, z)
							+ clamped_1(x, y - 1, z))
			+ 0.125f
					* (clamped_1(x, y, z + 1) - 2.0f * clamped_1(x, y, z)
							+ clamped_1(x, y, z - 1)) + clamped_1(x, y, z);

	clamped_2(x, y, z) = heat_1(clamp(x, 1, max_x), clamp(y, 1, max_y),
			clamp(z, 1, max_z));

	heat_2(x, y, z) = 0.125f
			* (clamped_2(x + 1, y, z) - 2.0f * clamped_2(x, y, z)
					+ clamped_2(x - 1, y, z))
			+ 0.125f
					* (clamped_2(x, y + 1, z) - 2.0f * clamped_2(x, y, z)
							+ clamped_2(x, y - 1, z))
			+ 0.125f
					* (clamped_2(x, y, z + 1) - 2.0f * clamped_2(x, y, z)
							+ clamped_2(x, y, z - 1)) + clamped_2(x, y, z);

	heat_2.compute_root();
	return heat_2;
}

int main(int argc, char **argv) {
	ImageParam input(Float(32), 3);
	const unsigned int steps = 6;
	const unsigned int macro_steps = steps / 3;
	Var x("x"), y("y"), z("z");
	Func heat[steps + 1];
	heat[0](x, y, z) = input(x, y, z);
	for (int t = 1; t <= macro_steps; t++) {
		heat[t] = heat_step(heat[t - 1], input.width() - 2, input.height() - 2,
				input.channels() - 2);
	}

	heat[macro_steps].compile_to_file("heat_3d_np", input);
	return 0;
}

#include <Halide.h>
using namespace Halide;

int main(int argc, char **argv) {

	ImageParam input(Float(32), 3);
	Var x("x"), y("y");

	Func heat("heat");
	RDom r(1, input.width() - 2, 1, input.height() - 2,1, input.channels() - 1);

	heat(x, y) = input(x, y);

	heat(r.x, r.y, r.z) = 0.125f
			* (heat(r.x + 1, r.y, r.z - 1) - 2.0f * heat(r.x, r.y, r.z - 1)
					+ heat(r.x - 1, r.y, r.z - 1))
			+ 0.125f
					* (heat(r.x, r.y + 1, r.z - 1)
							- 2.0f * heat(r.x, r.y, r.z - 1)
							+ heat(r.x, r.y - 1, r.z - 1))
			+ heat(r.x, r.y, r.z - 1);

	heat.compute_root();

	heat.compile_to_file("heat_2d_np_zero", input);

	return 0;
}

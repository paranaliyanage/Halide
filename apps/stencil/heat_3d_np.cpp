#include <Halide.h>
using namespace Halide;

int main(int argc, char **argv) {

	ImageParam input(Float(32), 4);
	Var x("x"), y("y"), z("z"), w("w");

	Func heat("heat");
	RDom r(1, input.width() - 2, 1, input.height() - 2, 1, input.channels() - 1,
			1, input.extent(3) - 1);

	heat(x, y, z, w) = input(x, y, z, w);

	heat(r.x, r.y, r.z, r.w) = 0.125f
			* (heat(r.x + 1, r.y, r.z, r.w - 1)
					- 2.0f * heat(r.x, r.y, r.z, r.w - 1)
					+ heat(r.x - 1, r.y, r.z, r.w - 1))
			+ 0.125f
					* (heat(r.x, r.y + 1, r.z, r.w - 1)
							- 2.0f * heat(r.x, r.y, r.z, r.w - 1)
							+ heat(r.x, r.y - 1, r.z, r.w - 1))
			+ 0.125f
					* (heat(r.x, r.y, r.z + 1, r.w - 1)
							- 2.0f * heat(r.x, r.y, r.z, r.w - 1)
							+ heat(r.x, r.y, r.z - 1, r.w - 1))
			+ heat(r.x, r.y, r.z, r.w - 1);

	heat.compute_root();

	heat.compile_to_file("heat_3d_np", input);

	return 0;
}

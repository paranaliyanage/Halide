include ../support/Makefile.inc

all: test_1d_np

$(LIB_HALIDE): ../.. ../../src
	$(MAKE) -C ../../ $(LIB_HALIDE)

heat_1d_np: heat_1d_np.cpp $(LIB_HALIDE)
	$(CXX) $(CPPFLAGS) heat_1d_np.cpp -I ../../include/  ../../$(LIB_HALIDE) -o heat_1d_np  -lpthread -ldl

heat_1d_np.o: heat_1d_np
	./heat_1d_np

test_1d_np: heat_1d_np.o test_1d_np.cpp
	$(CXX) -I../support -O3 -ffast-math -Wall -Werror test_1d_np.cpp heat_1d_np.o -lpthread -ldl -o test_1d_np  $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS)
	./test_1d_np 32

heat_2d_np: heat_2d_np.cpp $(LIB_HALIDE)
	$(CXX) $(CPPFLAGS) heat_2d_np.cpp -I ../../include/  ../../$(LIB_HALIDE) -o heat_2d_np  -lpthread -ldl

heat_2d_np.o: heat_2d_np
	./heat_2d_np

test_2d_np: heat_2d_np.o test_2d_np.cpp
	$(CXX) -I../support -O3 -ffast-math -Wall -Werror test_2d_np.cpp heat_2d_np.o -lpthread -ldl -o test_2d_np  $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS)
	./test_2d_np 64

heat_2d_np_zero: heat_2d_np_zero.cpp $(LIB_HALIDE)
	$(CXX) $(CPPFLAGS) heat_2d_np_zero.cpp -I ../../include/  ../../$(LIB_HALIDE) -o heat_2d_np_zero  -lpthread -ldl

heat_2d_np_zero.o: heat_2d_np_zero
	./heat_2d_np_zero

test_2d_np_zero: heat_2d_np_zero.o test_2d_np_zero.cpp
	$(CXX) -I../support -O3 -ffast-math -Wall -Werror test_2d_np_zero.cpp heat_2d_np_zero.o -lpthread -ldl -o test_2d_np_zero  $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS)
	./test_2d_np_zero 64

heat_3d_np: heat_3d_np.cpp $(LIB_HALIDE)
	$(CXX) $(CPPFLAGS) heat_3d_np.cpp -I ../../include/  ../../$(LIB_HALIDE) -o heat_3d_np  -lpthread -ldl

heat_3d_np.o: heat_3d_np
	./heat_3d_np

test_3d_np: heat_3d_np.o test_3d_np.cpp
	$(CXX) -I../support -O3 -ffast-math -Wall -Werror test_3d_np.cpp heat_3d_np.o -lpthread -ldl -o test_3d_np  $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS)
	./test_3d_np 64

clean:
	rm -f heat_1d_np heat_1d_np.o heat_1d_np.h test_1d_np heat_2d_np heat_2d_np.o heat_2d_np.h test_2d_np heat_2d_np_zero heat_2d_np_zero.o heat_2d_np_zero.h test_2d_np_zero
	rm -f heat_3d_np heat_3d_np.o heat_3d_np.h test_3d_np
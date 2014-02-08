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
	./test_1d_np 1024 1024

heat_2d_np: heat_2d_np.cpp $(LIB_HALIDE)
	$(CXX) $(CPPFLAGS) heat_2d_np.cpp -I ../../include/  ../../$(LIB_HALIDE) -o heat_2d_np  -lpthread -ldl

heat_2d_np.o: heat_2d_np
	./heat_2d_np

test_2d_np: heat_2d_np.o test_2d_np.cpp
	$(CXX) -I../support -O3 -ffast-math -Wall -Werror test_2d_np.cpp heat_2d_np.o -lpthread -ldl -o test_2d_np  $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS)
	./test_2d_np 5 5

clean:
	rm -f heat_1d_np heat_1d_np.o heat_1d_np.h test_1d_np heat_2d_np heat_2d_np.o heat_2d_np.h test_2d_np

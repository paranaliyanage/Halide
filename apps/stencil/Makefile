include ../support/Makefile.inc

all: test

$(LIB_HALIDE): ../.. ../../src
	$(MAKE) -C ../../ $(LIB_HALIDE)

heat_1d: heat_1d.cpp $(LIB_HALIDE)
	$(CXX) $(CPPFLAGS) heat_1d.cpp -I ../../include/  ../../$(LIB_HALIDE) -o heat_1d  -lpthread -ldl

heat_1d.o: heat_1d
	./heat_1d

test: heat_1d.o test.cpp
	$(CXX) -I../support -O3 -ffast-math -Wall -Werror test.cpp heat_1d.o -lpthread -ldl -o test  $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS)
	./test 1024 1024

clean:
	rm -f heat_1d heat_1d.o heat_1d.h test

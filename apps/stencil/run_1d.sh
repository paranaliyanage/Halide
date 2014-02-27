#!/bin/bash
for input in 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
    do
		g++  heat_1d_np_$input.cpp -I ../../include/  ../../bin/libHalide.a -o heat_1d_np  -lpthread -ldl
		./heat_1d_np
		g++ -I../support -O3 -ffast-math -Wall -Werror test_1d_np.cpp heat_1d_np.o -lpthread -ldl -o test_1d_np  -L/usr/lib/x86_64-linux-gnu -lpng12 -I/usr/include/libpng12  
		./test_1d_np $input

    done

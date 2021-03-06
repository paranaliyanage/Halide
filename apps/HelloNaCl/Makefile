
NACL_SDK_ROOT ?= $(HOME)/nacl_sdk/pepper_30

WARNINGS := -Wall -Wswitch-enum
CXXFLAGS := -pthread $(WARNINGS) -I $(NACL_SDK_ROOT)/include

UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
TC ?= $(NACL_SDK_ROOT)/toolchain/linux_x86_newlib
ARM_TC ?= $(NACL_SDK_ROOT)/toolchain/linux_arm_newlib
endif

ifeq ($(UNAME), Darwin)
TC ?= $(NACL_SDK_ROOT)/toolchain/mac_x86_newlib
ARM_TC ?= $(NACL_SDK_ROOT)/toolchain/mac_arm_newlib
endif

LIBDIR_32 = $(NACL_SDK_ROOT)/lib/newlib_x86_32/Release
LIBDIR_64 = $(NACL_SDK_ROOT)/lib/newlib_x86_64/Release
LIBDIR_ARM = $(NACL_SDK_ROOT)/lib/newlib_arm/Release

CXX_64 := $(TC)/bin/x86_64-nacl-g++ -m64 -O3 -s -Wl,--gc-sections
CXX_32 := $(TC)/bin/i686-nacl-g++ -m32 -O3 -s -Wl,--gc-sections
CXX_ARM := $(ARM_TC)/bin/arm-nacl-g++ -O3 -s -Wl,--gc-sections
LDFLAGS_64 := -L $(LIBDIR_64) -lppapi_cpp -lppapi
LDFLAGS_32 := -L $(LIBDIR_32) -lppapi_cpp -lppapi
LDFLAGS_ARM := -L $(LIBDIR_ARM) -lppapi_cpp -lppapi

OBJECTS_32 = hello_nacl_32.o halide_game_of_life_32.o c_game_of_life_32.o
OBJECTS_ARM = hello_nacl_arm.o halide_game_of_life_arm.o c_game_of_life_arm.o
OBJECTS_64 = hello_nacl_64.o halide_game_of_life_64.o c_game_of_life_64.o

all: hello_nacl.zip

hello_nacl.zip: hello_nacl_64.nexe hello_nacl_32.nexe hello_nacl_arm.nexe
	zip hello_nacl.zip hello_nacl.html hello_nacl.nmf hello_nacl_32.nexe hello_nacl_64.nexe hello_nacl_arm.nexe *game_of_life.cpp halide_game_of_life_32.s halide_game_of_life_arm.s halide_game_of_life_64.s

hello_nacl_64.nexe: $(OBJECTS_64)
	$(CXX_64) -o $@ -m64 $(OBJECTS_64) $(LDFLAGS_64)

hello_nacl_32.nexe: $(OBJECTS_32)
	$(CXX_32) -o $@ -m32 $(OBJECTS_32) $(LDFLAGS_32)

hello_nacl_arm.nexe: $(OBJECTS_ARM)
	$(CXX_ARM) -o $@ $(OBJECTS_ARM) $(LDFLAGS_ARM)

hello_nacl_64.o: hello_nacl.cpp halide_game_of_life_64.o
	$(CXX_64) -o $@ -m64 -c $< $(CXXFLAGS)

hello_nacl_32.o: hello_nacl.cpp halide_game_of_life_32.o
	$(CXX_32) -o $@ -m32 -c $< $(CXXFLAGS)

hello_nacl_arm.o: hello_nacl.cpp halide_game_of_life_arm.o
	$(CXX_ARM) -o $@ -c $< $(CXXFLAGS)

c_game_of_life_32.o: c_game_of_life.cpp
	$(CXX_32) -o $@ -m32 -c $< $(CXXFLAGS)

c_game_of_life_64.o: c_game_of_life.cpp
	$(CXX_64) -o $@ -m64 -c $< $(CXXFLAGS)

c_game_of_life_arm.o: c_game_of_life.cpp
	$(CXX_ARM) -o $@ -c $< $(CXXFLAGS)

halide_game_of_life_64.o: halide_game_of_life
	HL_TARGET=x86-64-sse41-nacl DYLD_LIBRARY_PATH=../../bin LD_LIBRARY_PATH=../../bin ./halide_game_of_life
	mv halide_game_of_life.o halide_game_of_life_64.o
	mv halide_game_of_life.s halide_game_of_life_64.s

halide_game_of_life_32.o: halide_game_of_life
	HL_TARGET=x86-32-sse41-nacl DYLD_LIBRARY_PATH=../../bin LD_LIBRARY_PATH=../../bin ./halide_game_of_life
	mv halide_game_of_life.o halide_game_of_life_32.o
	mv halide_game_of_life.s halide_game_of_life_32.s

halide_game_of_life_arm.o: halide_game_of_life
	HL_TARGET=arm-32-nacl DYLD_LIBRARY_PATH=../../bin LD_LIBRARY_PATH=../../bin ./halide_game_of_life
	mv halide_game_of_life.o halide_game_of_life_arm.o
	mv halide_game_of_life.s halide_game_of_life_arm.s

halide_game_of_life: halide_game_of_life.cpp
	$(CXX) halide_game_of_life.cpp -o halide_game_of_life -I ../../include -L ../../bin -lHalide -lpthread -ldl

clean:
	rm -f halide_game_of_life *.o *.nexe *.s

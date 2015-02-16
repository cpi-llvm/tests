BIN ?= ~/develop/builds/cps/bin
CC = $(BIN)/clang
CXX = $(BIN)/clang++
DIS = $(BIN)/llvm-dis
CFLAGS = -flto -fcps -O0
CXXFLAGS = -flto -fcps -O0 -std=c++11

BINARIES = basic \
	   arg_fct \
	   vtable
LL_FILES = $(BINARIES:%=%.ll)

all: $(LL_FILES) $(BINARIES)

%.bc: %.cc
	$(CXX) $(CXXFLAGS) -c -emit-llvm $< -o $@

%.bc: %.c
	$(CC) $(CFLAGS) -c -emit-llvm $< -o $@

%.ll: %.bc
	$(DIS) $<

%: %.c
	$(CC) $(CFLAGS) $< -o $@

%: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.ll *.bc

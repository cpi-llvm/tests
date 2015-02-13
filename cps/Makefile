BIN ?= ~/develop/builds/cps/bin
CC = $(BIN)/clang
DIS = $(BIN)/llvm-dis
CFLAGS = -flto -fcps

all: basic.ll basic

%.bc: %.c
	$(CC) $(CFLAGS) -c -emit-llvm $< -o $@

%.ll: %.bc
	$(DIS) $<

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.ll *.bc

all: build/sfDB2

test: all
	build/sfDB2

clean: build
	rm -rvf build

build:
	mkdir -pv build

build/sfDB2: build/sfDB2.o
	clang build/sfDB2.o -o build/sfDB2

build/sfDB2.o: build sfDB2.c sfDB2.h
	clang -c sfDB2.c -o build/sfDB2.o -O2 -march=native -mtune=native

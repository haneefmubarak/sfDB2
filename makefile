all: build/sfDB2

test: all
	build/sfDB2

clean: build
	rm -rvf build

build:
	mkdir -pv build

build/sfDB2: build/sfDB2.o build/table.o build/step.o build/coord.o
	clang build/sfDB2.o build/table.o build/step.o build/coord.o -o build/sfDB2 -lcrypto -lssl

build/sfDB2.o: build sfDB2.c sfDB2.h
	clang -c sfDB2.c -o build/sfDB2.o -O2 -march=native -mtune=native

build/table.o: build table.c sfDB2.h
	clang -c table.c -o build/table.o -O2 -march=native -mtune=native

build/step.o: build step.c sfDB2.h
	clang -c step.c -o build/step.o -O2 -march=native -mtune=native

build/coord.o: build coord.c sfDB2.h
	clang -c coord.c -o build/coord.o -O2 -march=native -mtune=native

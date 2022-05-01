compiler := clang

all: clean build

build:
	$(compiler) -o main main.c

run:
	./main

clean:
	rm -f ./main


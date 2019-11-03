## chris.rohlf@gmail.com - 2019
## vector_t Makefile

CC = clang
CFLAGS = -Wall
EXE_CFLAGS = -fPIE -pie
DEBUG_FLAGS = -DDEBUG -ggdb
LIBRARY = -fPIC -shared -fvisibility=default
ASAN = -fsanitize=address
TEST_FLAGS = -DVECTOR_UNIT_TEST=1

all: library

## Build the library
library: clean
	mkdir -p build/
	$(CC) $(CFLAGS) $(LIBRARY) vector.c -o build/libvector.so

## Build a debug version of the library
library_debug: clean
	mkdir -p build/
	$(CC) $(CFLAGS) $(LIBRARY) $(DEBUG_FLAGS) vector.c -o build/libvector.so

## Build the vector tests
test: library clean
	mkdir -p ../build/
	$(CC) $(ASAN) $(EXE_CFLAGS) $(DEBUG_FLAGS) $(CFLAGS) -o build/vector_test unit_tests.c \
		$(TEST_FLAGS) -Lbuild/ -lvector
	LD_LIBRARY_PATH=build/ build/vector_test

clean:
	rm -rf build/

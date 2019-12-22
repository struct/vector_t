## chris.rohlf@gmail.com - 2019
## vector_t Makefile

CC = clang
CFLAGS = -Wall
EXE_CFLAGS = -fPIE -pie
DEBUG_FLAGS = -DDEBUG -ggdb
WRITE_LOCK = -DWRITE_LOCK
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
	$(CC) $(CFLAGS) $(LIBRARY) $(DEBUG_FLAGS) $(WRITE_LOCK) vector.c -o build/libvector.so

## Build the vector tests
test: library_debug clean
	mkdir -p ../build/
	$(CC) $(ASAN) $(DEBUG_FLAGS) $(WRITE_LOCK) $(CFLAGS) -o build/vector_test unit_tests.c \
		$(TEST_FLAGS) -Lbuild/ -lvector
	$(CC) $(ASAN) $(DEBUG_FLAGS) $(WRITE_LOCK) $(CFLAGS) -o build/vector_write_lock_test write_lock_test.c \
		$(TEST_FLAGS) -Lbuild/ -lvector
	LD_LIBRARY_PATH=build/ build/vector_test
	LD_LIBRARY_PATH=build/ build/vector_write_lock_test

clean:
	rm -rf build/

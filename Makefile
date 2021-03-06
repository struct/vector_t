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
BUILD_DIR = build

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
STRIP = strip -s $(BUILD_DIR)/libvector.so
endif

all: library

## Build the library
library: clean
	mkdir -p $(BUILD_DIR)/
	$(CC) $(CFLAGS) $(LIBRARY) vector.c -o $(BUILD_DIR)/libvector.so
	$(STRIP)

## Build a debug version of the library
library_debug: clean
	mkdir -p $(BUILD_DIR)/
	$(CC) $(CFLAGS) $(LIBRARY) $(DEBUG_FLAGS) $(WRITE_LOCK) vector.c -o $(BUILD_DIR)/libvector.so

## Build the vector tests
test: library_debug clean
	mkdir -p ../$(BUILD_DIR)/
	$(CC) $(ASAN) $(DEBUG_FLAGS) $(WRITE_LOCK) $(CFLAGS) -o $(BUILD_DIR)/vector_test unit_tests.c \
		$(TEST_FLAGS) -Lbuild/ -lvector
	$(CC) $(ASAN) $(DEBUG_FLAGS) $(WRITE_LOCK) $(CFLAGS) -o $(BUILD_DIR)/vector_write_lock_test write_lock_test.c \
		$(TEST_FLAGS) -Lbuild/ -lvector
	LD_LIBRARY_PATH=$(BUILD_DIR)/ $(BUILD_DIR)/vector_test
	LD_LIBRARY_PATH=$(BUILD_DIR)/ $(BUILD_DIR)/vector_write_lock_test

format:
	clang-format *.c *.h -i

clean:
	rm -rf $(BUILD_DIR)/

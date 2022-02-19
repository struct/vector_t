## chris.rohlf@gmail.com - 2022
## vector_t Makefile

CC = clang
CFLAGS = -Wall -std=c17
EXE_CFLAGS = -fPIE -pie
LDFLAGS = -lpthread
DEBUG_FLAGS = -DDEBUG -ggdb
VECTOR_LOCK = -DVECTOR_LOCK=1
LIBRARY = -fPIC -shared -fvisibility=default
ASAN = -fsanitize=address
BUILD_DIR = build

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
STRIP = strip -s $(BUILD_DIR)/libvector.so
endif

all: library

## Build the library
library: clean
	mkdir -p $(BUILD_DIR)/
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBRARY) $(VECTOR_LOCK) vector.c -o $(BUILD_DIR)/libvector.so
	$(STRIP)

## Build a debug version of the library
library_debug: clean
	mkdir -p $(BUILD_DIR)/
	$(CC) $(ASAN) $(CFLAGS) $(LDFLAGS) $(LIBRARY) $(DEBUG_FLAGS) $(VECTOR_LOCK) vector.c -o $(BUILD_DIR)/libvector.so

## Build the vector tests
tests: library_debug clean
	mkdir -p ../$(BUILD_DIR)/
	$(CC) $(ASAN) $(DEBUG_FLAGS) $(VECTOR_LOCK) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/test tests/unit_tests.c \
		$(TEST_FLAGS) -Lbuild/ -lvector -I./
	$(CC) $(ASAN) $(DEBUG_FLAGS) $(VECTOR_LOCK) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/write_lock_test tests/write_lock_test.c \
		$(TEST_FLAGS) -Lbuild/ -lvector -I./
	LD_LIBRARY_PATH=$(BUILD_DIR)/ $(BUILD_DIR)/test
	LD_LIBRARY_PATH=$(BUILD_DIR)/ $(BUILD_DIR)/write_lock_test

perf_test: library clean
	$(CC) $(DEBUG_FLAGS) $(VECTOR_LOCK) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/write_perf_test tests/write_perf_test.c \
	$(TEST_FLAGS) -Lbuild/ -lvector -I./
	LD_LIBRARY_PATH=$(BUILD_DIR)/ $(BUILD_DIR)/write_perf_test

format:
	clang-format *.c *.h -i

clean:
	rm -rf $(BUILD_DIR)/

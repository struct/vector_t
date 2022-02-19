# vector_t library

**vector_t** is a simple vector library in C. It's designed to hold arbitrary `void *` pointers. It doesn't know what those pointers do or who owns them. If they point to malloc'd memory you need to free that memory.

The API convention is built off function callbacks for working with vector contents and very simple push/pop semantics for adding and removing elements.

The vector_t library is thread safe by design, and uses a simple mutex to enforce thread safe access to the underlying memory that backs the vector. It is enabled with the `-DVECTOR_LOCK` flag or by modifying the Makefile. See the bottom of the API section below for how to properly access the vector using the API from within `vector_for_each` callbacks.

The `write_perf_test` unit test currently writes 100 million pointers into a vector in about 1 second of wall clock time.

# API

The API documentation is below. You can also look at *unit_tests.c* for some examples of how to use it.

`void vector_init(vector_t *v);` Initialize the vector members. You must call this.

`int vector_push(vector_t *v, void *ptr);` Push a pointer to the vector, resizes the underlying memory store if necessary. Returns this new elements index in the vector.

`void *vector_pop(vector_t *v);` Returns and clears the last element of the vector

`void *vector_get_end(vector_t *v);` Returns the last element of the vector

`void *vector_get_at(vector_t *v, size_t index);` Return the vector element at an index

`void *vector_for_each(vector_t *v, vector_for_each_callback_t *fe, void *data);` Invokes a callback for each vector element. Stops if the callback returns non-NULL. Elements added to the vector from within the callback will not be iterated upon. If the size of the vector shrinks during iteration this function will return.

`void *vector_set_at(vector_t *v, int index, void *ptr);` Sets the vector element at an index

`void vector_delete_all(vector_t *v, vector_delete_callback_t *dc);` Delete the vector by pop'ing each element from the vector. Its callers responsibility to free them before calling this.

`void vector_delete_at(vector_t *v, size_t index);` May create a hole in the vector. The free_slot member tracks this. Its a cheap optimization.

`void vector_free(vector_t *v);` Free the underlying buffer that backs the vector

`size_t vector_used(vector_t *v);` Returns the number of entries in the vector

The functions `vector_push`, `vector_pop`, `vector_get_end`, `vector_set_at`, and `vector_get_at` each contain non-thread safe versions which can be invoked by preprending an underscore `_` to the function name (e.g. `_vector_push`). These functions are intended to be called from within a `vector_for_each` callback where the vector is already locked. If you're not compiling with `VECTOR_LOCK` then you can simply use the regular version of these functions.

# Building

`make library` builds `libvector.so`

`make library_debug` builds `libvector.so` with debug symbols

`make tests` builds and runs unit tests with Address Sanitizer

`make perf_test` builds and runs a performance test that pushes 100 million pointers into a vector

`make format` runs clang format

chris.rohlf@gmail.com - 2022

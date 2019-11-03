# vector_t Library

**vector_t** is a simple vector library in C. It's designed to hold arbitrary `void *` pointers. It doesn't know what those pointers do or who owns them. If they point to malloc'd memory you need to free that memory.

The API convention is built off function callbacks for working with vector contents and very simple push/pop semantics for adding and removing elements.

# API

The API documentation is below. You can also look at *unit_tests.c* for some examples of how to use it.

`void vector_init(vector_t *v);` Initialize the vector members. You must call this.

`int vector_push(vector_t *v, void *ptr);` Push a pointer to the vector, resizes the underlying memory store if necessary. Returns this new elements index in the vector

`void *vector_pop(vector_t *v);` Returns and clears the last element of the vector

`void *vector_get_end(vector_t *v);` Returns the last element of the vector

`void *vector_get_at(vector_t *v, size_t index);` Return the vector element at an index

`void *vector_for_each(vector_t *v, vector_for_each_callback_t *fe, void *data);` Invokes a callback for each vector element. Stops if the callback returns non-NULL

`void *vector_set_at(vector_t *v, int index, void *ptr);` Sets the vector element at an index

`void vector_delete_all(vector_t *v, vector_delete_callback_t *dc);` Delete the vector by pop'ing each element from the vector. Its callers responsibility to free them before calling this.

`void vector_delete_at(vector_t *v, size_t index);` May create a hole in the vector. The free_slot member tracks this. Its a cheap optimization.

`void vector_free(vector_t *v);` Free the underlying buffer that backs the vector

`size_t vector_used(vector_t *v);` Returns the number of entries in the vector

# Building

`make library` will build `libvector.c`
`make test` will build and run the unit tests

I recommend submoduling this repo or just copying out *vector.c* and *vector.h*

chris.rohlf@gmail.com - 2019

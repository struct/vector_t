/* chris.rohlf@gmail.com - 2019 */
/* vector.c - A simple pointer vector library */

#include "vector.h"

/* This vector is just a simple container for pointers. It
 * doesn't know whether your pointers are from malloc or
 * const static strings. It is useful for storing lists of
 * pointers and iterating over them quickly. Building
 * abstractions on top of it is very useful */

/* Push a pointer to the vector, resize if necessary.
 * Returns this new elements index in the vector */
int vector_push(vector_t *v, void *ptr) {
    if(v->size == 0) {
        v->size = VECTOR_MIN_SIZE;
        v->data = (void *) malloc(sizeof(void *) * v->size);
        memset(v->data, 0x0, v->size);
    }

    /* Sometimes the vector has a free slot, lets use it */
    if(v->free_slot != 0) {
        v->data[v->free_slot] = ptr;
        v->free_slot = 0;
        return v->free_slot;
    }

    if(v->end_slot == v->size) {
        v->size *= 2;
        v->data = (void *) realloc(v->data, sizeof(void *) * v->size);
    }

    v->data[v->end_slot] = ptr;
    v->end_slot++;
    return v->end_slot;
}

/* Returns and clears the last member of the vector */
void *vector_pop(vector_t *v) {
    if(v->end_slot == 0) {
        return NULL;
    }

    void *last = v->data[v->end_slot-1];
    size_t last_index = v->end_slot - 1;

    v->data[last_index] = 0x0;
    v->end_slot--;

    return last;
}

/* Returns the last member of the vector */
void *vector_get_end(vector_t *v) {
    return v->data[v->end_slot];
}

/* Return the vector member at an index */
void *vector_get_at(vector_t *v, size_t index) {
    if(index >= v->end_slot) {
        return NULL;
    }

    return v->data[index];
}

/* Invokes a callback for each vector member.
 * Stops if the callback returns non-NULL */
void *vector_for_each(vector_t *v, vector_for_each_callback_t *fe, void *data) {
    void *ret = NULL;

    if(fe == NULL) {
        return NULL;
    }

    for(size_t sz=0; sz < vector_used(v); sz++) {
        void *p = vector_get_at(v, sz);

        if(p == NULL) {
            continue;
        }

        ret = (fe)(p, data);

        if(ret != NULL) {
            return ret;
        }
    }

    return NULL;
}

/* Sets the vector member at an index */
void *vector_set_at(vector_t *v, int index, void *ptr) {
    if(index >= v->end_slot) {
        return NULL;
    }

    v->data[index] = ptr;
    v->end_slot++;

    return v->data[index];
}

/* Returns the number of entries in the vector */
size_t vector_used(vector_t *v) {
    return v->end_slot;
}

/* May create a hole in the vector. The free_slot
 * member tracks this. Its a cheap optimization */
void vector_delete_at(vector_t *v, size_t index) {
    v->data[index] = 0x0;
    v->free_slot = index;
}

/* Delete the vector by pop'ing each member from
 * the vector. Its callers responsibility to free
 * them before calling this. */
void vector_delete_all(vector_t *v, vector_delete_callback_t *dc) {
    void *p = NULL;

    while((p = vector_pop(v)) != NULL) {
        if(dc != NULL) {
            (dc)(p);
        }
    }
}

/* Free the underlying buffer */
void vector_free(vector_t *v) {
    memset(v->data, 0x0, v->size);
    free(v->data);
    v->size = 0;
    v->end_slot = 0;
}

/* Initialize the vector members */
void vector_init(vector_t *v) {
    v->data = NULL;
    v->size = 0;
    v->end_slot = 0;
    v->free_slot = 0;
}

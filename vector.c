/* chris.rohlf@gmail.com - 2022 */
/* vector.c - A simple pointer vector library */

#include "vector.h"

/* Push a pointer to the vector, resize if necessary.
 * Returns this new elements index in the vector */
int32_t _vector_push(vector_t *v, void *ptr) {
    if(v->size == 0) {
        v->size = VECTOR_MIN_SIZE;
        v->data = (void *) malloc(sizeof(void *) * v->size);
        memset(v->data, 0x0, v->size);
    }

    /* Sometimes the vector has a free slot, lets use it */
    if(v->free_slot != 0) {
        v->data[v->free_slot] = ptr;
        int32_t free_slot = v->free_slot;
        v->free_slot = 0;
        return free_slot;
    }

    if(v->end_slot == v->size) {
        v->size *= 2;
        v->data = (void *) realloc(v->data, sizeof(void *) * v->size);
    }

    v->data[v->end_slot] = ptr;
    v->end_slot++;
    int32_t ret = v->end_slot;
    return ret;
}

int32_t vector_push(vector_t *v, void *ptr) {
    LOCK_VECTOR(v);
    int32_t ret = _vector_push(v, ptr);
    UNLOCK_VECTOR(v);
    return ret;
}

/* Returns and clears the last member of the vector */
void *_vector_pop(vector_t *v) {
    if(v->end_slot == 0) {
        return NULL;
    }

    void *last = v->data[v->end_slot - 1];
    size_t last_index = v->end_slot - 1;

    v->data[last_index] = 0x0;
    v->end_slot--;

    return last;
}

void *vector_pop(vector_t *v) {
    LOCK_VECTOR(v);
    void *last = _vector_pop(v);
    UNLOCK_VECTOR(v);
    return last;
}

/* Returns the last member of the vector */
void *_vector_get_end(vector_t *v) {
    return v->data[v->end_slot];
}

void *vector_get_end(vector_t *v) {
    LOCK_VECTOR(v);
    void *ret = _vector_get_end(v);
    UNLOCK_VECTOR(v);
    return ret;
}

/* Return the vector member at an index */
void *_vector_get_at(vector_t *v, size_t index) {
    if(index >= v->end_slot) {
        return NULL;
    }

    void *ret = v->data[index];
    return ret;
}

void *vector_get_at(vector_t *v, size_t index) {
    LOCK_VECTOR(v);
    void *ret = _vector_get_at(v, index);
    UNLOCK_VECTOR(v);
    return ret;
}

/* Sets the vector member at an index */
void *_vector_set_at(vector_t *v, int index, void *ptr) {
    if(index >= v->end_slot) {
        return NULL;
    }

    v->data[index] = ptr;
    v->end_slot++;

    return v->data[index];
}

void *vector_set_at(vector_t *v, int index, void *ptr) {
    LOCK_VECTOR(v);
    void *ret = _vector_set_at(v, index, ptr);
    UNLOCK_VECTOR(v);
    return ret;
}

/* Invokes a callback for each vector member.
 * Stops if the callback returns non-NULL */
void *vector_for_each(vector_t *v, vector_for_each_callback_t *fe, void *data) {
    LOCK_VECTOR(v);

    void *ret = NULL;

    if(fe == NULL) {
        UNLOCK_VECTOR(v);
        return NULL;
    }

    size_t used = vector_used(v);

    for(size_t idx = 0; idx < used; idx++) {
        void *p = _vector_get_at(v, idx);

        if(p == NULL) {
            continue;
        }

        ret = (fe) (p, data);

        if(ret != NULL || used > vector_used(v)) {
            UNLOCK_VECTOR(v);
            return ret;
        }
    }

    UNLOCK_VECTOR(v);
    return NULL;
}

/* Returns the number of entries in the vector */
size_t vector_used(vector_t *v) {
    return v->end_slot;
}

/* May create a hole in the vector. The free_slot
 * member tracks this. Its a cheap optimization */
void vector_delete_at(vector_t *v, size_t index) {
    LOCK_VECTOR(v);
    v->data[index] = 0x0;
    v->free_slot = index;
    UNLOCK_VECTOR(v);
}

/* Delete the vector by pop'ing each member from
 * the vector. Its callers responsibility to free
 * them before calling this. */
void vector_delete_all(vector_t *v, vector_delete_callback_t *dc) {
    LOCK_VECTOR(v);
    void *p = NULL;

    while((p = _vector_pop(v)) != NULL) {
        if(dc != NULL) {
            (dc)(p);
        }
    }

    UNLOCK_VECTOR(v);
}

/* Free the underlying buffer */
void vector_free(vector_t *v) {
    LOCK_VECTOR(v);
    memset(v->data, 0x0, v->size);
    free(v->data);
    v->size = 0;
    v->end_slot = 0;
    UNLOCK_VECTOR(v);
}

/* Initialize the vector members */
void vector_init(vector_t *v) {
#if VECTOR_LOCK
    pthread_mutex_init(&v->lock, 0);
#endif
    v->data = NULL;
    v->size = 0;
    v->end_slot = 0;
    v->free_slot = 0;
}

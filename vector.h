/* chris.rohlf@gmail.com - 2022 */
/* vector.h - A simple pointer vector */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#if VECTOR_LOCK
#include <assert.h>
#include <pthread.h>
#endif

#define VECTOR_MIN_SIZE 1024

typedef struct {
    /* This vector only holds pointers */
    void **data;
    /* Index to the end of the vector */
    size_t end_slot;
    /* Size of the vector */
    size_t size;
    /* Index of the last known hole in the vector */
    size_t free_slot;
#if VECTOR_LOCK
    pthread_mutex_t lock;
#endif
} vector_t;

#if VECTOR_LOCK
#define LOCK_VECTOR(v) pthread_mutex_lock(&v->lock);
#define UNLOCK_VECTOR(v) pthread_mutex_unlock(&v->lock);
#else
#define LOCK_VECTOR(v)
#define UNLOCK_VECTOR(v)
#endif

typedef void(vector_delete_callback_t)(void *);
typedef void *(vector_for_each_callback_t) (void *, void *);

void *_vector_pop(vector_t *v);
void *vector_pop(vector_t *v);

int32_t _vector_push(vector_t *v, void *ptr);
int32_t vector_push(vector_t *v, void *ptr);

void *_vector_get_end(vector_t *v);
void *vector_get_end(vector_t *v);

void *_vector_get_at(vector_t *v, size_t index);
void *vector_get_at(vector_t *v, size_t index);

void *_vector_set_at(vector_t *v, int index, void *ptr);
void *vector_set_at(vector_t *v, int index, void *ptr);

void *vector_for_each(vector_t *v, vector_for_each_callback_t *fe, void *data);
void vector_delete_all(vector_t *v, vector_delete_callback_t *dc);
void vector_delete_at(vector_t *v, size_t index);
void vector_free(vector_t *v);
void vector_init(vector_t *v);
size_t vector_used(vector_t *v);

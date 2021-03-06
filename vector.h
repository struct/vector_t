/* chris.rohlf@gmail.com - 2019 */
/* vector.h - A simple pointer vector */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#if DEBUG || WRITE_LOCK
#include <assert.h>
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
#if DEBUG || WRITE_LOCK
    /* Write lock. Not thread safe!
     * Only used with -DWRITE_LOCK or -DDEBUG */
    int32_t lock;
#endif
} vector_t;

#if DEBUG || WRITE_LOCK
#define LOCK_VECTOR(v) v->lock = 1;
#define UNLOCK_VECTOR(v) v->lock = 0;
#define IS_VECTOR_LOCKED(v) (v->lock == 1)
#define IS_VECTOR_UNLOCKED(v) (v->lock == 0)
#define ASSERT_IF_LOCKED(v) assert(!IS_VECTOR_LOCKED(v))
#else
#define LOCK_VECTOR(v)
#define UNLOCK_VECTOR(v)
#define IS_VECTOR_LOCKED(v)
#define IS_VECTOR_UNLOCKED(v)
#define ASSERT_IF_LOCKED(v)
#endif

typedef void(vector_delete_callback_t)(void *);
typedef void *(vector_for_each_callback_t)(void *, void *);

void *vector_pop(vector_t *v);
void *vector_get_end(vector_t *v);
void *vector_get_at(vector_t *v, size_t index);
void *vector_for_each(vector_t *v, vector_for_each_callback_t *fe, void *data);
void *vector_set_at(vector_t *v, int index, void *ptr);
void vector_delete_all(vector_t *v, vector_delete_callback_t *dc);
void vector_delete_at(vector_t *v, size_t index);
void vector_free(vector_t *v);
void vector_init(vector_t *v);
size_t vector_used(vector_t *v);
int32_t vector_push(vector_t *v, void *ptr);

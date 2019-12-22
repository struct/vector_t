/* chris.rohlf@gmail.com - 2019 */
/* vector_t unit tests */

#include "vector.h"

#define VECTOR_PUSHES 10
#define VECTOR_ENTRY "abcd1234"

vector_t v;

void *vector_fe_test(void *p, void *data) {
    printf("Writing to a locked vector. I will crash\n");
    vector_push(&v, "hello!");
    return NULL;
}

void vector_pointer_free(void *p) {
    free(p);
}

int main(int argc, char *argv[]) {
    vector_init(&v);

    for(size_t j=0; j < VECTOR_PUSHES; j++) {
        vector_push(&v, VECTOR_ENTRY);
    }

    printf("Vector @ %p has %zu slots in use\n", &v, vector_used(&v));

    if(vector_used(&v) != VECTOR_PUSHES) {
        printf("Vector size mismatch. Was %zu should be %d\n", vector_used(&v), VECTOR_PUSHES);
        exit(-1);
    }

    vector_for_each_callback_t *fe = &vector_fe_test;
    vector_for_each(&v, fe, NULL);

    for(size_t i=0; i < vector_used(&v); i++) {
        printf("Vector entry [%s]\n", vector_get_at(&v, i));
    }

    vector_delete_all(&v, NULL);
    vector_free(&v);

    return 0;
}

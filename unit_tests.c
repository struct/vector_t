/* chris.rohlf@gmail.com - 2019 */
/* vector_t unit tests */

#include "vector.h"

#define VECTOR_PUSHES 10
#define VECTOR_ENTRY "abcd1234"

void *vector_fe_test(void *p, void *data) {
    printf("Vector entry [%s]\n", p);
    if(strcmp(p, VECTOR_ENTRY) != 0) {
        printf("Unexpected vector entry [%s]\n", p);
        exit(-1);
    }
    return 0;
}

void vector_pointer_free(void *p) {
    free(p);
}

int main(int argc, char *argv[]) {
    vector_t v;
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

    vector_delete_callback_t *dc = &vector_pointer_free;
    vector_t vv;
    vector_init(&vv);
    vector_push(&vv, malloc(10));
    vector_push(&vv, malloc(20));
    vector_push(&vv, malloc(30));
    vector_delete_all(&vv, dc);
    vector_free(&vv);

    return 0;
}

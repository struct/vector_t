/* chris.rohlf@gmail.com - 2022 */
/* vector_t unit tests */

#include "vector.h"

#define VECTOR_PUSHES 100000000

int main(int argc, char *argv[]) {
    vector_t v;
    vector_init(&v);

    void *ptr = (void *) 0x12345678;

    for(size_t j = 0; j < VECTOR_PUSHES; j++) {
        vector_push(&v, ptr++);
    }

    if(vector_used(&v) != VECTOR_PUSHES) {
        printf("Vector size mismatch. Was %zu should be %d\n", vector_used(&v), VECTOR_PUSHES);
        exit(-1);
    }

    vector_free(&v);

    return 0;
}

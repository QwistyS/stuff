#define QWISTYS_ARRAY_IMPLEMENTATION
#include "qwistys_lib.h"
#include "stdint.h"


int main() {
  // Testing bitwise macros
    uint32_t bitfield = 0;

    QWISTYS_BIT_SET(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == true);

    QWISTYS_BIT_CLEAR(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == false);

    QWISTYS_BIT_TOGGLE(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == true);

    QWISTYS_BIT_TOGGLE(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == false);

    flexa_t* array = flexa_init(sizeof(int), 5);
    if (!array) {
        return -1;
    }

    int value = 10;
    if (flexa_add(array, &value) != 0) {
        fprintf(stderr, "Failed to add element to array\n");
        flexa_free(array);
        return -1;
    }

    QWISTYS_DEBUG_MSG("Size of the flexa = %d", flexa_size(array));
    int *data = (int*)flexa_get_raw_data(array);

    for (int i = 0; i < flexa_size(array); i++) {
      QWISTYS_DEBUG_MSG("Value = {%d} Index = {%d}", data[i], i);
    }

    int* fetched_value = (int*)flexa_get(array, 0);
    if (fetched_value) {
        QWISTYS_DEBUG_MSG("Fetched value %d", *fetched_value);
    } else {
        fprintf(stderr, "Failed to fetch element from array\n");
    }

    flexa_free(array);

    return 0;
}

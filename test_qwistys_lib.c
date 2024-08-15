#define QWISTYS_ALLOC_IMPLEMENTATION
#include "qwistys_alloc.h"
#define QWISTYS_ARRAY_IMPLEMENTATION
#include "qwistys_flexa.h"
#define QWISTYS_AVLT_IMPLEMENTATION
#include "qwistys_avltree.h"

int main() {
    QWISTYS_DEBUG_MSG("______________ ALLOC TEST ______________________");
    int* pointer = qwistys_malloc(sizeof(int), NULL);
    if (pointer == NULL) {
        QWISTYS_DEBUG_MSG("Something went wrong :/");
        return -1;
    }
    uint64_t pdata = 0xFFFFFAFAFAFAF;

    memcpy(pointer, &pdata, sizeof(int));
    // If wanna check the mem leack uncomment this line
    // Commented due the HALT on mem overide.
    // memcpy(pointer, &pdata, sizeof(uint64_t));

    qwistys_free(pointer);

    QWISTYS_DEBUG_MSG("______________ ALLOC END ______________________");
    QWISTYS_DEBUG_MSG("______________ BITWISE TEST ______________________");
    uint32_t bitfield = 0;

    QWISTYS_BIT_SET(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == true);

    QWISTYS_BIT_CLEAR(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == false);

    QWISTYS_BIT_TOGGLE(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == true);
    QWISTYS_BIT_TOGGLE(bitfield, 3);
    QWISTYS_ASSERT(QWISTYS_BIT_CHECK(bitfield, 3) == false);
    QWISTYS_DEBUG_MSG("______________ BITWISE END ______________________");

    QWISTYS_DEBUG_MSG("______________ FLEXA TEST ______________________");

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
    int* data = (int*) flexa_get_raw_data(array);

    for (int i = 0; i < flexa_size(array); i++) {
        QWISTYS_DEBUG_MSG("Value = {%d} Index = {%d}", data[i], i);
    }

    int* fetched_value = (int*) flexa_get(array, 0);
    if (fetched_value) {
        QWISTYS_DEBUG_MSG("Fetched value %d", *fetched_value);
    } else {
        fprintf(stderr, "Failed to fetch element from array\n");
    }

    flexa_free(array);

    QWISTYS_DEBUG_MSG("______________ FLEXA END ______________________");

    QWISTYS_DEBUG_MSG("______________  AVL TREE TEST ______________________");

    typedef struct {
        int id;
        char name[100];
    } user_data_t;

    void print_user_data(void* data) {
        user_data_t* pd = (user_data_t*) data;
        QWISTYS_DEBUG_MSG("Id = %d Name = %s", pd->id, pd->name);
    }

    int compare(void* a, void* b) {
        user_data_t* pa = (user_data_t*) a;
        user_data_t* pb = (user_data_t*) b;
        return pa->id - pb->id;
    }
    
    void delet_data(void* data) {
    user_data_t* pdata = (user_data_t*)data;
    QWISTYS_DEBUG_MSG("Prepering to delete data {id=%d, Name=%s}", pdata->id, pdata->name);
  }

    user_data_t user_array[] = {{1, "Daniel Mor"},
    {2, "Valentina Yagmorov"},
    {3, "Yuval Mor"},
    {4, "some other dude"},
    {12, "some other dude"},
    {11, "A cool person"},
    {7, "Shmok person"},
    {14, "Poor person"},
    {7, "Rich person"},
  };

    avlt_node_t* root = NULL;

    for (int i = 0; i < QWISTYS_ARRAY_LEN(user_array); i++) {
        root = avlt_insert(root, &user_array[i], sizeof(user_data_t), compare);
    }

    avlt_print(root, print_user_data);

    avlt_delete(root, &user_array[2], compare, delet_data);

    avlt_print(root, print_user_data);
    avlt_free_tree(root, delet_data);
    QWISTYS_DEBUG_MSG("______________  AVL TREE END ______________________");

    return 0;
}

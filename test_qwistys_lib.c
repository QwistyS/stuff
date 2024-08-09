#define QWISTYS_ARRAY_IMPLEMENTATION
#include "qwistys_flexa.h"
#define QWISTYS_ALVTREE_IMPLEMENTATION
#include "qwistys_avltree.h"
#define QWISTYS_ALLOC_IMPLEMENTATION
#include "qwistys_alloc.h"
#include "stdint.h"


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
    QWISTYS_DEBUG_MSG("______________ FLEXA END ______________________");
  
    QWISTYS_DEBUG_MSG("______________  AVL TREE TEST ______________________");
    typedef struct {
    int id;
    char name[100];
  } user_data_t;
    
    avlt_node_t* node = avlt_create_node(sizeof(user_data_t));
    if(node == NULL) {
    exit(-1);
  }
  QWISTYS_DEBUG_MSG("______________  AVL TREE TEST ______________________");

  return 0;
}

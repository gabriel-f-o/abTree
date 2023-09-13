#ifndef __ABTREE_H__
#define __ABTREE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __malloc
#define __malloc    malloc
#endif

#ifndef __realloc
#define __realloc   realloc
#endif

#ifndef __free
#define __free      free
#endif

#ifndef __fprintf
#define __fprintf   fprintf
#endif

#ifndef __static
#define __static    static
#endif

/**********************************
 * PUBLIC TYPES
 **********************************/

/*
-----------------------------------------*/
typedef struct abElement_s {
    int32_t key;
    void* data;
} abElement_t;

/*
-----------------------------------------*/
typedef struct abNode_s {
    struct abNode_s** child;
    struct abNode_s* parent;

    abElement_t* el;
    uint16_t keyNum;
    bool isLeaf;
} abNode_t;

/*
-----------------------------------------*/
typedef struct abTree_s {
    abNode_t* root;
    void (*conflict_cb) (abNode_t* node, int32_t key, void* data);
    uint16_t a;
    uint16_t b;
} abTree_t;


/**********************************
 * PUBLIC FUNCTIONS
 **********************************/

abTree_t* abTree_create(uint16_t a, uint16_t b, void (*abTree_conflictCB)(abNode_t* node, int32_t key, void* data));
void* abTree_search(abTree_t* tree, int32_t key);
/*
-----------------------------------------*/
void abTree_insert(abTree_t* tree, int32_t key, void* data);

void abTree_print(abTree_t* tree);
void abTree_destroy(abTree_t* tree);
void* abTree_remove(abTree_t* tree, int32_t key);


#ifdef __cplusplus
}
#endif

#endif //__ABTREE_H__
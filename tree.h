/*
 * tree.h
 *
 *  Created on: Sep 10, 2023
 *      Author: Gabriel
 */


#ifndef __ABTREE_H__
#define __ABTREE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************
 * DEFINES (will be overwritten by utests)
 *********************************************************/


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


/**********************************************************
 * PUBLIC TYPES
 *********************************************************/


/* Element of a node
----------------------------------------------------------------*/
typedef struct abElement_s {
    void* data;     //Data associated with the key
    int32_t key;    //Key of the element
} abElement_t;

/* Tree's node
----------------------------------------------------------------*/
typedef struct abNode_s {
    struct abNode_s** child;    //Array to all children (size = Number of elements + 1)
    struct abNode_s* parent;    //Pointer to the node's parent (NULL for the root)

    abElement_t* el;            //Array of elements
    uint16_t keyNum;            //Number of elements
    bool isLeaf;                //Flag to inform if the node is a leaf or not
} abNode_t;

/* Conflict callback function
 * 
 * @param abNode_t* node : [in] The node that has the key
 * @param int32_t key    : [in] The key that conflicted
 * @param void* data     : [in] Data associated with this key
----------------------------------------------------------------*/
typedef void (*abConflict_fn)(abNode_t* node, int32_t key, void* data);

/* Tree structure
----------------------------------------------------------------*/
typedef struct abTree_s {
    abNode_t* root;             //Pointer to the tree's root
    abConflict_fn conflict_cb;  //Conflict callback function to be called if the insert tries to insert two indentical keys
    uint16_t a;                 //Minimum number of children (at least 2)
    uint16_t b;                 //Maximum number of children (at least 2 * a - 1)
} abTree_t;


/**********************************************************
 * PUBLIC FUNCTIONS
 *********************************************************/


/* Creates a tree
 * 
 * @param uint16_t a                      : [in] Tree's minimum amount of children (at least 2)
 * @param uint16_t b                      : [in] Tree's maximum amount of children (at least 2 * a - 1)
 * @param abConflict_fn abTree_conflictCB : [in] Callback to be called in case of an insert conflict (inserting the same key twice). NULL to ignore
 * 
 * @return void* : The data associated to the key, or NULL if not found
----------------------------------------------------------------*/
abTree_t* abTree_create(uint16_t a, uint16_t b, abConflict_fn abTree_conflictCB);

/* Search a key in the tree
 * 
 * @param abTree_t* tree : [ in] The tree to search
 * @param int32_t key    : [ in] Key to search for
 * 
 * @return void* : The data associated to the key, or NULL if not found
----------------------------------------------------------------*/
void* abTree_search(abTree_t const * tree, int32_t key);

/* Insert a key into the tree
 * 
 * @param abTree_t* tree : [in] The tree to add
 * @param int32_t key    : [in] Key to be added
 * @param void* data     : [in] Data associated with the key
----------------------------------------------------------------*/
void abTree_insert(abTree_t* tree, int32_t key, void const * data);

/* Print the whole tree
 * 
 * @param abTree_t* tree : [in] The tree to print
----------------------------------------------------------------*/
void abTree_print(abTree_t const * tree);

/* Destroy the tree, de-allocating every node
 * 
 * @param abTree_t* tree : [in] The tree to de-allocate
----------------------------------------------------------------*/
void abTree_destroy(abTree_t* tree);


void* abTree_remove(abTree_t* tree, int32_t key);


#ifdef __cplusplus
}
#endif

#endif //__ABTREE_H__
/*
 * tree.c
 *
 *  Created on: Sep 10, 2023
 *      Author: Gabriel
 */


#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/**********************************************************
 * PRIVATE TYPES
 *********************************************************/

/* Enum used to choose which child to keep in the removeEl function
----------------------------------------------------------------*/
typedef enum {
    ABTREE_KEEP_LEFT_CHILD,
    ABTREE_KEEP_RIGHT_CHILD
}abTree_keepChild_e;

/**********************************************************
 * PRIVATE FUNCTIONS
 *********************************************************/

/* Print a node (this function uses unicode characters)
 *
 * @param abNode_t* node : [in] Node to print
 * @param uint32_t level : [in] Node's level
 * @param uint16_t b     : [in] Maximum number of childs
----------------------------------------------------------------*/
__static void abTree_printNode(abNode_t const * node, uint32_t level, uint16_t b)
{
    /* Argument check
    ----------------------------------------------------------------*/
    if(node == NULL) return;

    /* Calculate line length based on maximum number of keys
    ----------------------------------------------------------------*/
    const int line_len = b * 7 + 1;

    /* Print first line (upper frame line)
    ----------------------------------------------------------------*/
    for(int childPos = 0; childPos < line_len; childPos++) {
             if(childPos == 0)                 __fprintf(stdout, "\U0000250F");
        else if(childPos == 7)                 __fprintf(stdout, "\U0000252F");
        else if(childPos == line_len - 1)      __fprintf(stdout, "\U00002513\n");
        else                            __fprintf(stdout, "\U00002501");
    }

    /* Print second line (level + Leaf/Node)
    ----------------------------------------------------------------*/
    int pos = -1;
    for(int childPos = 0; childPos < line_len; childPos++) {
             if(childPos == 0)                 { __fprintf(stdout, "\U00002503 L%03u ", level); childPos += 6; }
        else if(childPos == 7)                   __fprintf(stdout, "\U00002502");
        else if(childPos == line_len - 1)        __fprintf(stdout, "\U00002503\n");
        else  { 
            if(pos == -1) pos = childPos;
            
            if(childPos < (line_len + pos) / 2 - 2)
                __fprintf(stdout, " ");
            
            else if(childPos == (line_len + pos) / 2 - 2){
                __fprintf(stdout, "%s", node->isLeaf ? "Leaf" : "Node");
                childPos += 3;
            }

            else{
                __fprintf(stdout, " ");
            }
        }
    }

    /* Print 3rd line (Line)
    ----------------------------------------------------------------*/
    for(int childPos = 0; childPos < line_len; childPos++) {
        if(childPos == 0)                      __fprintf(stdout, "\U00002520");
        else if(childPos == line_len - 1)      __fprintf(stdout, "\U00002528\n");
        else if(childPos == 7)                 __fprintf(stdout, "\U00002534");
        else                            __fprintf(stdout, "\U00002500");
    }

    /* Print 4th line (First parent key (or NULL))
    ----------------------------------------------------------------*/
    for(int childPos = 0; childPos < line_len; childPos++) {
             if(childPos == 0)                   __fprintf(stdout, "\U00002503");
        else if(childPos == line_len - 1)        __fprintf(stdout, "\U00002503\n");
        else  {             
            if(childPos < (line_len) / 2 - 2)
                __fprintf(stdout, " ");
            
            else if(childPos == (line_len) / 2 - 2){
                if(node->parent == NULL) 
                    __fprintf(stdout, "NULL");
                else
                    __fprintf(stdout, "%04d", node->parent->el[0].key);
                childPos += 3;
            }

            else{
                __fprintf(stdout, " ");
            }
        }
    }    
    
    /* Print 5th line (line)
    ----------------------------------------------------------------*/
    for(int childPos = 0; childPos < line_len; childPos++) {
        if(childPos == 0)                      __fprintf(stdout, "\U00002520");
        else if(childPos == line_len - 1)      __fprintf(stdout, "\U00002528\n");
        else if(childPos == 7)                 __fprintf(stdout, "\U0000252C");
        else if(childPos > 7 && childPos % 7 == 0)    __fprintf(stdout, "\U0000252C");
        else                            __fprintf(stdout, "\U00002500");
    }

    /* Print 6th line (all keys in order)
    ----------------------------------------------------------------*/
    printf ("\U00002503");
    for(uint16_t childPos = 0; childPos < b - 1; childPos++){
        if(node->el == NULL){
            for(int j = 1; j < line_len; j++) {
                if(j == line_len - 1)           __fprintf(stdout, "\U00002503\n");
                else if(j < line_len / 2 - 2)   __fprintf(stdout, " ");
                else if(j == line_len / 2 - 2)  { __fprintf(stdout, "NULL"); j += 3; }
                else                            __fprintf(stdout, " ");
            }

            break;
        } 

        if(childPos < node->keyNum)
            __fprintf(stdout, " %04d \U00002502", node->el[childPos].key);
        else
            __fprintf(stdout, " XXXX \U00002502");
    }

    /* Pad 6th line with "----" because the tree always have 1 child less than children
    ----------------------------------------------------------------*/
    if(node->el != NULL)
        __fprintf(stdout, " ---- \U00002503\n");

    /* Print 7th line (line)
    ----------------------------------------------------------------*/
    for(int childPos = 0; childPos < line_len; childPos++) {
        if(childPos == 0)                      __fprintf(stdout, "\U00002520");
        else if(childPos == line_len - 1)      __fprintf(stdout, "\U00002528\n");
        else if(childPos % 7 == 0)             __fprintf(stdout, "\U0000253C");
        else                            __fprintf(stdout, "\U00002500");
    }

    /* Print 8th line (first key for every child (or NULL if leaf))
    ----------------------------------------------------------------*/
    printf ("\U00002503");
    for(uint16_t childPos = 0; childPos < b; childPos++){
        if(node->isLeaf){
            for(int j = 1; j < line_len; j++) {
                if(j == line_len - 1)           __fprintf(stdout, "\U00002503\n");
                else if(j < line_len / 2 - 2)   __fprintf(stdout, " ");
                else if(j == line_len / 2 - 2)  { __fprintf(stdout, "NULL"); j += 3; }
                else                            __fprintf(stdout, " ");
            }

            break;
        } 

        if(childPos < node->keyNum + 1)
            if(node->child[childPos] == NULL)
                __fprintf(stdout, " NULL \U00002502");
            else if(childPos == b - 1)
                __fprintf(stdout, " %04d \U00002503\n", node->child[childPos]->el[0].key);
            else
                __fprintf(stdout, " %04d \U00002502", node->child[childPos]->el[0].key);
        
        else if(childPos < b - 1)
            __fprintf(stdout, " XXXX \U00002502");
        
        else
            __fprintf(stdout, " XXXX \U00002503\n");
    }

    /* Print 9th line (lower frame)
    ----------------------------------------------------------------*/
    for(int childPos = 0; childPos < line_len; childPos++) {
        if(childPos == line_len - 1)           __fprintf(stdout, "\U0000251B\n");
        else if(childPos == 0)                 __fprintf(stdout, "\U00002517");
        else if(childPos % 7 == 0)             __fprintf(stdout, "\U00002537");
        else                            __fprintf(stdout, "\U00002501");
    }
}

/* Print a node (this function uses unicode characters)
 *
 * @param abNode_t* node : [in] Node to calculate the height
 * 
 * @return uint32_t : node height (1 for leaf, then +1 for each level)
----------------------------------------------------------------*/
__static uint32_t abTree_getNodeHeight(abNode_t const * node)
{
    /* Arg check
    ----------------------------------------------------------------*/
    if(node == NULL)
        return 0;
    
    /* If node is a leafm then hight is 1
    ----------------------------------------------------------------*/
    else if(node->isLeaf)
        return 1;

    /* Otherwise calculate node's height
    ----------------------------------------------------------------*/
    else {
        uint32_t height = 0;
        for(uint16_t childPos = 0; childPos < node->keyNum + 1; childPos++){
            uint32_t h = abTree_getNodeHeight(node->child[childPos]);
            if(h > height)
                height = h;
        } 

        return height + 1;
    }
}

/* Print a whole level (this function uses unicode characters)
 *
 * @param abNode_t* node : [in] Node to print
 * @param uint32_t level : [in] Node's level
 * @param uint16_t b     : [in] Maximum number of childs
----------------------------------------------------------------*/
__static void abTree_printLevel(abNode_t const * node, uint32_t level, uint16_t b)
{
    /* Print node if we are in the correct level
    ----------------------------------------------------------------*/
    if(level == 1){ 
        abTree_printNode(node, abTree_getNodeHeight(node), b);
    }

    /* Otherwise continue to lower levels
    ----------------------------------------------------------------*/
    else {
        for(uint16_t childPos = 0; childPos < node->keyNum + 1; childPos++){
            abTree_printLevel(node->child[childPos], level - 1, b);
        }
    }
}


/* Creates a node with N elements inside
 * 
 * @param abNode_t* parent      : [in] parent of the new node (or NULL if root)
 * @param abElement_t new_el[]  : [in] Array containing all elements of the node
 * @param size_t el_len         : [in] Size of the element array
 * @param abNode_t* new_child[] : [in] Array containing all children (size = el_len  + 1)
 * @param bool isLeaf           : [in] Is the new node a leaf of not (if yes, the children array will be null)
 * 
 * @return abNode_t* : reference to the new node, or NULL if problem
----------------------------------------------------------------*/
__static abNode_t* abTree_createNodeNEl(abNode_t const * parent, abElement_t const new_el[], size_t el_len, abNode_t const * const new_child[], bool isLeaf)
{
    /* Allocate everything needed
    ----------------------------------------------------------------*/
    abNode_t* node = (abNode_t*)__malloc(sizeof(abNode_t));
    abElement_t* el = (abElement_t*)__malloc(el_len * sizeof(abElement_t));
    abNode_t** child = isLeaf ? NULL : (abNode_t**)__malloc((el_len + 1) * sizeof(abNode_t*));

    /* Check allocation and free if necessary
    ----------------------------------------------------------------*/
    if(node == NULL || el == NULL || (child == NULL && !isLeaf)){
        __free(node);
        __free(el);
        __free(child);
        return NULL;
    }

    /* Copy relevant information to the new node
    ----------------------------------------------------------------*/
    node->el = el;
    node->keyNum = el_len;
    node->child = child;
    node->parent = (abNode_t*) parent;
    node->isLeaf = isLeaf;

    memcpy(node->el, new_el, el_len * sizeof(abElement_t));
    if(!isLeaf && new_child != NULL){
        memcpy(node->child, new_child, (el_len + 1) * sizeof(abNode_t*));
    }

    return node;
}

/* Creates a node with 1 element inside
 *
 * @param abNode_t* parent          : [in] Parent of the nez node (NULL if root)
 * @param int32_t key               : [in] Key to add
 * @param void* data                : [in] Data to add
 * @param abNode_t* left_child      : [in] Left child of the new node (or NULL if leaf)
 * @param abNode_t* right_child     : [in] Right child of the new node (or NULL if leaf)
 * @param bool isLeaf               : [in] Flag indicating if is a leaf
 * 
 * @return abNode_t* : reference to the new node, or NULL if problem
----------------------------------------------------------------*/
__static abNode_t* abTree_createNode1El(abNode_t const * parent, int32_t key, void const * data, abNode_t const * left_child, abNode_t const * right_child, bool isLeaf)
{
    abElement_t el = { .key = key, .data = (void*)data };
    abNode_t const * child[] = {left_child, right_child};
    return abTree_createNodeNEl(parent, &el, 1, ( isLeaf ? NULL : child), isLeaf);
}


/* Frees a node, freeing its elements, children (just the array itself) and the node
 * 
 * @param abNode_t* node : [in] node to free
----------------------------------------------------------------*/
__static void abTree_freeNode(abNode_t* node)
{
    /* Arg check
    ----------------------------------------------------------------*/
    if(node == NULL) 
        return;

    /* Free arrays and then the node
    ----------------------------------------------------------------*/
    __free(node->el);
    __free(node->child);
    __free(node);
}

/* Frees a tree or sub-tree by freeing each node, starting from leaves and then its parents 
 * 
 * @param abNode_t* node : [in] root of the tree or sub-tree to free
----------------------------------------------------------------*/
__static void abTree_freeTree(abNode_t* node)
{
    /* Arg check
    ----------------------------------------------------------------*/
    if(node == NULL) 
        return;

    /* Recusively free children nodes
    ----------------------------------------------------------------*/
    for(int32_t childPos = 0; !node->isLeaf && childPos < node->keyNum + 1; childPos++){
        abTree_freeTree(node->child[childPos]);
    }

    /* Free the node itself
    ----------------------------------------------------------------*/
    abTree_freeNode(node);
}


/* Insert an element into a node. If the node is not a leaf, the child will be added in order to maintain the tree's structure 
 * (childPos.e. node->child[0].key[0] < node.key[0] < node->child[1].key[0] < node.key[1] ...)
 * 
 * @param abNode_t* node        : [in] Node to put the element
 * @param int32_t key           : [in] Key of the new element
 * @param void* data            : [in] Data of the new element
 * @param abNode_t* new_child   : [in] The new chil
 * 
 * @return int32_t : the position the element was inserted 
----------------------------------------------------------------*/
__static int32_t abTree_insertEl(abNode_t* node, int32_t key, void const * data, abNode_t const * new_child)
{
    /* Reallocate the arrays
    ----------------------------------------------------------------*/
    node->keyNum++;
    node->el = (abElement_t*)__realloc(node->el, node->keyNum * sizeof(abElement_t));

    if(!node->isLeaf){
        node->child = (abNode_t**)__realloc(node->child, (node->keyNum + 1) * sizeof(abNode_t*));
    }

    /* Move children and elements to maintain sorting. e.g. inserting 3 on [1, 2, 4, 5, X] will result in [1, 2, X, 3, 4]
    ----------------------------------------------------------------*/
    int32_t childPos;
    for(childPos = node->keyNum; childPos > 0; childPos--)
    {
        /* Move children depending on the current key
        ----------------------------------------------------------------*/ 
        if(!node->isLeaf){
            if(key < node->child[childPos - 1]->el[0].key){
                node->child[childPos] = (abNode_t*) node->child[childPos - 1]; 
            }
        }

        /* Move element depending on the current key
        ----------------------------------------------------------------*/
        if(childPos < node->keyNum){
            if(node->el[childPos - 1].key < key) break; //Stop if we are in the correct place

            memcpy(&node->el[childPos], &node->el[childPos - 1], sizeof(abElement_t));
        }
    }

    /* Copy contents into the correct position
    ----------------------------------------------------------------*/
    node->el[childPos].key = key;
    node->el[childPos].data = (void*)data;

    /* Position new child in the correct position
    ----------------------------------------------------------------*/
    if(!node->isLeaf && new_child != NULL){
        if(new_child->el[0].key > key)
            node->child[childPos + 1] = (abNode_t*) new_child;
        else
            node->child[childPos] = (abNode_t*) new_child;
    }

    /* Return inserted position
    ----------------------------------------------------------------*/
    return childPos;
}

/* Split a node in half - 1, creating 2 new children nodes. The parent node will receive a key to keep the tree's integrity.
 * Because of this, the parent might have too many keys, making necessary to call this function again, but for the parent. 
 * The children's children parent pointer must be updated to point to the correct parent, since a parent node can be split as well.
 * In case of the root, we need to do a different method of splitting to not break the tree->root reference
 * The split is done in a way that the 3 nodes (2 children + the splitted node) have between a and b - 1 keys.
 * 
 * @param abNode_t* node        : [in] Node to split
 * 
 * @return abNode_t* : the node's parent 
----------------------------------------------------------------*/
__static abNode_t* abTree_splitNode(abNode_t* node)
{
    /* Get the center element to make calculations easier
    ----------------------------------------------------------------*/
    uint16_t center_pos = (node->keyNum - 1)/ 2;
    abElement_t center_el = node->el[center_pos];

    /* The root pointer must be kept the same to not break the tree->root reference, 
     * so we convert the root node into the parent and attach the 2 children into it
    ----------------------------------------------------------------*/    
    abNode_t* parent = node->parent != NULL ? node->parent : node;
    
    /* Create children node and check for allocation
    ----------------------------------------------------------------*/
    abNode_t* new_node   = abTree_createNodeNEl(parent,     &node->el[0],              center_pos,                     ( (node->isLeaf) ? NULL : (abNode_t const * const *) &node->child[0]),                 node->isLeaf);
    abNode_t* new_sister = abTree_createNodeNEl(parent,     &node->el[center_pos + 1], node->keyNum - center_pos - 1,  ( (node->isLeaf) ? NULL : (abNode_t const * const *) &node->child[center_pos + 1]),    node->isLeaf);
    if(new_node == NULL || new_sister == NULL){
        abTree_freeNode(new_node);
        abTree_freeNode(new_sister);            
        return NULL;
    } 

    /* If the node to split is not a leaf, we must update the children's parents to the newly created nodes
    ----------------------------------------------------------------*/
    if(!node->isLeaf){
        for(int32_t childPos = 0; childPos < node->keyNum + 1; childPos++){
            if(childPos <= center_pos)
                node->child[childPos]->parent = new_node;
            else
                node->child[childPos]->parent = new_sister;
        }
    }

    /* In the case the node is not the root, just send a key to the parent, splitting the remaining keys into the 2 children
    ----------------------------------------------------------------*/
    if(node->parent != NULL){
        int32_t childPos = abTree_insertEl(node->parent, center_el.key, center_el.data, new_sister);
        node->parent->child[childPos] = new_node; //update child pointer because we need to free the current node
        abTree_freeNode(node);

        return parent;
    }

    /* If it's the root, we modify the node keeping its reference
    ----------------------------------------------------------------*/
    else{       
        node->el    = (abElement_t*)__realloc(node->el, sizeof(abElement_t));
        node->child = (abNode_t**)__realloc(node->child, 2 * sizeof(abNode_t*));

        node->el[0].key = center_el.key;
        node->el[0].data = center_el.data;

        node->child[0] = new_node;
        node->child[1] = new_sister;
        node->isLeaf = false;
        node->keyNum = 1;

        return node;
    }  
}


/* Search a node that contains a certain key
 * 
 * @param abNode_t* node            : [ in] The root of the tree or sub-tree
 * @param int32_t key   	        : [ in] Key to search for
 * @param uint16_t* pos             : [out] Position of the key in the element array (NULL to ignore)
 * @param abNode_t** insert_node    : [out] Pointer to the node where the key should be, if the key was not found (NULL to ignore)
 * 
 * @return abNode_t* : Node that contains the key, or NULL if not found
----------------------------------------------------------------*/
__static abNode_t* abTree_searchNode(abNode_t const * node, int32_t key, uint16_t* pos, abNode_t** insert_node)
{   
    /* While we are not at the end of the tree
    ----------------------------------------------------------------*/
    while(node != NULL)
    {
        /* Search for each key
        ----------------------------------------------------------------*/
        int32_t childPos = 0;
        while(1)
        {
            /* Key dos not exist where it should be, so we go to one of the children
            ----------------------------------------------------------------*/
            if(childPos == node->keyNum || key < node->el[childPos].key)
            {
                /* If the node is a leaf, there are no children so we just return
                ----------------------------------------------------------------*/
                if(node->isLeaf){
                    if(insert_node != NULL)
                        *insert_node = (abNode_t*)node;

                    node = NULL;
                }

                /* Otherwise we must check the child's keys
                ----------------------------------------------------------------*/
                else
                    node = node->child[childPos];

                break;
            }

            /* The key was found, return the node and the position
            ----------------------------------------------------------------*/
            else if(key == node->el[childPos].key){
                if(pos != NULL) 
                    *pos = childPos;
                return (abNode_t*) node;
            }

            childPos++;
        }
    }

    return NULL;
}


/* Removes an element from a node, de-allocation the children and element arrays if needed. This function assumes the key exists in the node
 * 
 * @param abNode_t* node            : [in] Node to have the key removed
 * @param int32_t key               : [in] Key to remove
 * @param abTree_keepChild_e keep   : [in] Whether to keep the left or right child of the removed key
 * 
 * @return int32_t : position of the removed key in the array of elements
----------------------------------------------------------------*/
__static int32_t abTree_removeEl(abNode_t* node, int32_t key, abTree_keepChild_e keep)
{
    /* First, search the key in the element array
    ----------------------------------------------------------------*/
    int32_t childPos = 0;
    for(childPos = 0; childPos < node->keyNum && key > node->el[childPos].key; childPos++) 
        continue;

    /* Then, move all keys and children in order to overwrite the removed key and element.
     * After this is done, we can get rid of the last elements of both arrays
    ----------------------------------------------------------------*/
    for(int32_t j = childPos; j < node->keyNum; j++){ 
        /* Move element
        ----------------------------------------------------------------*/
        if(j < node->keyNum - 1)      
            memcpy(&node->el[j], &node->el[j + 1], sizeof(abElement_t));

        /*  Move children if the node is not a leaf, and keeping the correct child
        ----------------------------------------------------------------*/
        if(!node->isLeaf){
            if(keep == ABTREE_KEEP_RIGHT_CHILD || childPos != j) 
                node->child[j] = node->child[j + 1];
        }
    }
    
    /* Re-allocate element array
    ----------------------------------------------------------------*/
    if(--node->keyNum == 0){
        __free(node->el);
        node->el = (void*) NULL;
    }
    else{
        node->el = (abElement_t*) __realloc(node->el, node->keyNum * sizeof(abElement_t));
    }

    /* Re-allocate children array. At least one children is kept in order to keep the child reference even if the node has 0 keys
     * This is useful when the root has 0 keys, and we need to update the root pointer. Without this, we would lose the child's reference
    ----------------------------------------------------------------*/
    if(!node->isLeaf){
        node->child = (abNode_t**) __realloc(node->child,   (node->keyNum + 1) * sizeof(abNode_t*));
    }

    /* Return position
    ----------------------------------------------------------------*/
    return childPos;
}

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
abTree_t* abTree_create(uint16_t a, uint16_t b, abConflict_fn abTree_conflictCB)
{
    /* Arg check
    ----------------------------------------------------------------*/
    if(a < 2) return NULL;
    if(b < 2 * a - 1) return NULL;

    /* Create tree
    ----------------------------------------------------------------*/
    abTree_t* tree = (abTree_t*)__malloc(sizeof(abTree_t));
    if(tree == NULL)
        return NULL;

    /* Copy data into the structure and return
    ----------------------------------------------------------------*/
    tree->a = a;
    tree->b = b;
    tree->conflict_cb = abTree_conflictCB;
    tree->root = NULL;

    return tree;
}

/* Search a key in the tree
 * 
 * @param abTree_t* tree : [ in] The tree to search
 * @param int32_t key    : [ in] Key to search for
 * 
 * @return void* : The data associated to the key, or NULL if not found
----------------------------------------------------------------*/
void* abTree_search(abTree_t const * tree, int32_t key)
{
    /* Search tree
    ----------------------------------------------------------------*/
    uint16_t pos = 0;
    abNode_t* node = abTree_searchNode(tree->root, key, &pos, NULL);

    /* Return NULL if not found, or the data
    ----------------------------------------------------------------*/
    return node == NULL ? NULL : node->el[pos].data;
}

/* Insert a key into the tree
 * 
 * @param abTree_t* tree : [in] The tree to add
 * @param int32_t key    : [in] Key to be added
 * @param void* data     : [in] Data associated with the key
----------------------------------------------------------------*/
void abTree_insert(abTree_t* tree, int32_t key, void const * data)
{
    /* If there already is a root, insert it into the tree
    ----------------------------------------------------------------*/
     if(tree->root != NULL){

        /* Search for the key and return if it already exists
        ----------------------------------------------------------------*/
        uint16_t pos = 0;
        abNode_t* insert_node;
        abNode_t* node = abTree_searchNode(tree->root, key, &pos, &insert_node);
        if(node != NULL){
            if(tree->conflict_cb != NULL) tree->conflict_cb(node, key, (void*) node->el[pos].data);
            return;
        }

        /* Insert key into the node. If the node has too many keys, split it.
         * To split a node, the central key is removed, and the other keys are assigned to 2 nodes (one for lower keys and the other for greater keys)
         * If the node is the root, the central key is assigned to a node, that becomes the new root
         * If the node has a paret, the central key is added to the parent. In this case, we might have to split it as well
        ----------------------------------------------------------------*/
        abTree_insertEl(insert_node, key, data, NULL);
        while(insert_node->keyNum >= tree->b){
            insert_node = abTree_splitNode(insert_node);
        }
    }

    /* Otherwise just create the root node
    ----------------------------------------------------------------*/
    else{
        tree->root = abTree_createNode1El(NULL, key, data, NULL, NULL, true);
    }
}


/* Remove a key from the tree
 * 
 * @param abTree_t* tree : [in] The tree to remove from
 * @param int32_t key    : [in] Key to be removed
 * 
 * @return void* : Data associated with the key (or null if not found)
----------------------------------------------------------------*/
void* abTree_remove(abTree_t* tree, int32_t key)
{
    /* Search for key, if it does not exist just exit
    ----------------------------------------------------------------*/
    uint16_t key_pos;
    abNode_t* node = abTree_searchNode(tree->root, key, &key_pos, NULL);
    if(node == NULL)
        return NULL;

    /* Save the data to return to be easier
    ----------------------------------------------------------------*/
    void* data = node->el[key_pos].data;

    /* If the key to remove is not a leaf, then we have to find the highest key of the left sub tree, and swap them
     * By doing this, the tree's structure is kept, and we just have to remove leaves 
    ----------------------------------------------------------------*/
    if(!node->isLeaf){

        /* Search for the right-most node in the left subtree
        ----------------------------------------------------------------*/
        abNode_t* substitute = node->child[key_pos];
        while(!substitute->isLeaf){
            substitute = substitute->child[substitute->keyNum];
        }

        /* Swap the two keys. Now the key to be removed is in a leaf
        ----------------------------------------------------------------*/
        abElement_t swap = substitute->el[substitute->keyNum - 1];
        memcpy(&substitute->el[substitute->keyNum - 1],     &node->el[key_pos],     sizeof(abElement_t));
        memcpy(&node->el[key_pos],                          &swap,                  sizeof(abElement_t));

        /* Update locals to point to the leaf instead
        ----------------------------------------------------------------*/
        node = substitute;
        key_pos = substitute->keyNum - 1;
    }

    /* Remove key. Since it is a leaf, the child to keep is irrelevant
    ----------------------------------------------------------------*/
    abTree_removeEl(node, key, ABTREE_KEEP_LEFT_CHILD);

    /* Loop here while the current node has less children then the allowed
    ----------------------------------------------------------------*/
    while( node != NULL && ( (node->parent == NULL && node->keyNum == 0) || (node->parent != NULL && node->keyNum < tree->a - 1) ) )
    {
    
        /* If the node is the root, this means the root has no more keys. 
         * If the root is also a leaf, we just free it. Otherwise we update the root pointer to the root's only child
        ----------------------------------------------------------------*/
        if(node->parent == NULL){
            if(node->isLeaf){
                tree->root = NULL;
            }
            else{
                tree->root = node->child[0];
                tree->root->parent = NULL;
            }
            
            /* Free the old root
            ----------------------------------------------------------------*/
            abTree_freeNode(node);
            node = tree->root;
        }

        /* If the node is not the root, we have to either merge or steal a key
        ----------------------------------------------------------------*/
        else {

            /* First, search the parent for the position that points to the current node
            ----------------------------------------------------------------*/  
            int32_t childPos = 0;
            while(1){
                if(node->parent->child[childPos] == node) 
                    break;
                childPos++;
            }

            /* Get the reference to its neighbors (or null if they don't exist)
            ----------------------------------------------------------------*/
            abNode_t* neighbor_left =                     (childPos == 0) ? NULL : node->parent->child[childPos - 1];
            abNode_t* neighbor_right = (childPos == node->parent->keyNum) ? NULL : node->parent->child[childPos + 1];
            
            /* If the left neighbor exists and has enough keys, we can just steal it.
             * Stealing is easier than merging, as it does put other nodes in a low key situation
            ----------------------------------------------------------------*/
            if(neighbor_left != NULL && neighbor_left->keyNum > tree->a - 1) {

                /* Update child's parent
                ----------------------------------------------------------------*/
                if(!neighbor_left->isLeaf)
                    neighbor_left->child[neighbor_left->keyNum]->parent = node;
                
                /* Stealing a key is adding the parent's key into the node, then move the neighbor's biggest key to the parent
                 * Don't forget that stealing a key also means stealing a child
                ----------------------------------------------------------------*/
                abTree_insertEl(node, node->parent->el[childPos - 1].key, node->parent->el[childPos - 1].data, neighbor_left->isLeaf ? NULL : neighbor_left->child[neighbor_left->keyNum]);
                memcpy(&node->parent->el[childPos - 1],    &neighbor_left->el[neighbor_left->keyNum - 1],     sizeof(abElement_t));
                abTree_removeEl(neighbor_left,              neighbor_left->el[neighbor_left->keyNum - 1].key, ABTREE_KEEP_LEFT_CHILD); //Since we always remove the biggest key, we should keep left child
            }

            /* If the right neighbor exists and has enough keys, we can just steal it.
            ----------------------------------------------------------------*/
            else if(neighbor_right != NULL && neighbor_right->keyNum > tree->a - 1) {
                
                /* Update child's parent
                ----------------------------------------------------------------*/
                if(!neighbor_right->isLeaf)
                    neighbor_right->child[0]->parent = node;

                /* Stealing a key is adding the parent's key into the node, then move the neighbor's smallest key to the parent
                 * Don't forget that stealing a key also means stealing a child
                ----------------------------------------------------------------*/
                abTree_insertEl(node, node->parent->el[childPos].key, node->parent->el[childPos].data, neighbor_right->isLeaf ? NULL : neighbor_right->child[0]);
                memcpy(&node->parent->el[childPos],     &neighbor_right->el[0],     sizeof(abElement_t));
                abTree_removeEl(neighbor_right,          neighbor_right->el[0].key, ABTREE_KEEP_RIGHT_CHILD); //Since we always remove the smallest key, we should keep right child
            }

            /* If we can't steal, then we must merge nodes.
             * Merging nodes mean take a key from the parent, then merging the current node with one of its neighbors
             * After a merge, the parent can have too few keys, and the merge / steal logic repeats
            ----------------------------------------------------------------*/
            else {

                /* Decide if we should merge with left or right neghbor
                ----------------------------------------------------------------*/
                abNode_t* ins_node = neighbor_left != NULL ? neighbor_left : neighbor_right;
                int32_t elPos = ins_node == neighbor_left ? childPos - 1 : childPos;

                /* To merge nodes, we pick one key from the parent, and we merge all keys in the current node + the neighbor
                ----------------------------------------------------------------*/
                abTree_insertEl(ins_node, node->parent->el[elPos].key, node->parent->el[elPos].data, node->isLeaf ? NULL : node->child[0]);
                for(int32_t j = 0; j < node->keyNum; j++){
                    /* Don't forget that merging the keys also mean merging the children
                    ----------------------------------------------------------------*/
                    abTree_insertEl(ins_node, node->el[j].key, node->el[j].data, node->isLeaf ? NULL : node->child[j + 1]);

                    /* Also update the children's parent
                    ----------------------------------------------------------------*/
                    if(!node->isLeaf){
                        if(j == 0) 
                            node->child[j]->parent = ins_node;

                        node->child[j + 1]->parent = ins_node;
                    }
                }

                /* Now that the neighbor has all keys and children, delete the picked key from the parent
                 * Since we have to update the children's reference anyway, we can keep either right or left child
                ----------------------------------------------------------------*/
                abTree_removeEl(node->parent, node->parent->el[elPos].key, ABTREE_KEEP_LEFT_CHILD);
                node->parent->child[elPos] = ins_node;

                /* Free current node as it is not needed anymore
                ----------------------------------------------------------------*/
                abTree_freeNode(node);

                /* Point to the neighbor's parent. This is needed because now the parent may have insufficient keys
                ----------------------------------------------------------------*/
                node = ins_node->parent;
            }
        }
    }

    return data;
}

/* Print the whole tree
 * 
 * @param abTree_t* tree : [in] The tree to print
----------------------------------------------------------------*/
void abTree_print(abTree_t const * tree)
{
    uint32_t h = abTree_getNodeHeight(tree->root);
    for(uint16_t childPos = 1; childPos <= h; childPos++){
        abTree_printLevel(tree->root, childPos, tree->b);
        __fprintf(stdout, "\n");
    }

    __fprintf(stdout, "____________________________________________________________________________\n");
}

/* Destroy the tree, de-allocating every node
 * 
 * @param abTree_t* tree : [in] The tree to de-allocate
----------------------------------------------------------------*/
void abTree_destroy(abTree_t* tree)
{
    if(tree->root != NULL)
        abTree_freeTree(tree->root);
        
    __free(tree);
}
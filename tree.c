#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

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
    for(int i = 0; i < line_len; i++) {
             if(i == 0)                 __fprintf(stdout, "\U0000250F");
        else if(i == 7)                 __fprintf(stdout, "\U0000252F");
        else if(i == line_len - 1)      __fprintf(stdout, "\U00002513\n");
        else                            __fprintf(stdout, "\U00002501");
    }

    /* Print second line (level + Leaf/Node)
    ----------------------------------------------------------------*/
    int pos = -1;
    for(int i = 0; i < line_len; i++) {
             if(i == 0)                 { __fprintf(stdout, "\U00002503 L%03d ", level); i += 6; }
        else if(i == 7)                   __fprintf(stdout, "\U00002502");
        else if(i == line_len - 1)        __fprintf(stdout, "\U00002503\n");
        else  { 
            if(pos == -1) pos = i;
            
            if(i < (line_len + pos) / 2 - 2)
                __fprintf(stdout, " ");
            
            else if(i == (line_len + pos) / 2 - 2){
                __fprintf(stdout, "%s", node->isLeaf ? "Leaf" : "Node");
                i += 3;
            }

            else{
                __fprintf(stdout, " ");
            }
        }
    }

    /* Print 3rd line (Line)
    ----------------------------------------------------------------*/
    for(int i = 0; i < line_len; i++) {
        if(i == 0)                      __fprintf(stdout, "\U00002520");
        else if(i == line_len - 1)      __fprintf(stdout, "\U00002528\n");
        else if(i == 7)                 __fprintf(stdout, "\U00002534");
        else                            __fprintf(stdout, "\U00002500");
    }

    /* Print 4th line (First parent key (or NULL))
    ----------------------------------------------------------------*/
    for(int i = 0; i < line_len; i++) {
             if(i == 0)                   __fprintf(stdout, "\U00002503");
        else if(i == line_len - 1)        __fprintf(stdout, "\U00002503\n");
        else  {             
            if(i < (line_len) / 2 - 2)
                __fprintf(stdout, " ");
            
            else if(i == (line_len) / 2 - 2){
                if(node->parent == NULL) 
                    __fprintf(stdout, "NULL");
                else
                    __fprintf(stdout, "%04d", node->parent->el[0].key);
                i += 3;
            }

            else{
                __fprintf(stdout, " ");
            }
        }
    }    
    
    /* Print 5th line (line)
    ----------------------------------------------------------------*/
    for(int i = 0; i < line_len; i++) {
        if(i == 0)                      __fprintf(stdout, "\U00002520");
        else if(i == line_len - 1)      __fprintf(stdout, "\U00002528\n");
        else if(i == 7)                 __fprintf(stdout, "\U0000252C");
        else if(i > 7 && i % 7 == 0)    __fprintf(stdout, "\U0000252C");
        else                            __fprintf(stdout, "\U00002500");
    }

    /* Print 6th line (all keys in order)
    ----------------------------------------------------------------*/
    printf ("\U00002503");
    for(uint16_t i = 0; i < b - 1; i++){
        if(node->el == NULL){
            for(int i = 1; i < line_len; i++) {
                if(i == line_len - 1)           __fprintf(stdout, "\U00002503\n");
                else if(i < line_len / 2 - 2)   __fprintf(stdout, " ");
                else if(i == line_len / 2 - 2)  { __fprintf(stdout, "NULL"); i += 3; }
                else                            __fprintf(stdout, " ");
            }

            break;
        } 

        if(i < node->keyNum)
            __fprintf(stdout, " %04d \U00002502", node->el[i].key);
        else
            __fprintf(stdout, " XXXX \U00002502");
    }

    /* Pad 6th line with "----" because the tree always have 1 child less than children
    ----------------------------------------------------------------*/
    if(node->el != NULL)
        __fprintf(stdout, " ---- \U00002503\n");

    /* Print 7th line (line)
    ----------------------------------------------------------------*/
    for(int i = 0; i < line_len; i++) {
        if(i == 0)                      __fprintf(stdout, "\U00002520");
        else if(i == line_len - 1)      __fprintf(stdout, "\U00002528\n");
        else if(i % 7 == 0)             __fprintf(stdout, "\U0000253C");
        else                            __fprintf(stdout, "\U00002500");
    }

    /* Print 8th line (first key for every child (or NULL if leaf))
    ----------------------------------------------------------------*/
    printf ("\U00002503");
    for(uint16_t i = 0; i < b; i++){
        if(node->isLeaf){
            for(int i = 1; i < line_len; i++) {
                if(i == line_len - 1)           __fprintf(stdout, "\U00002503\n");
                else if(i < line_len / 2 - 2)   __fprintf(stdout, " ");
                else if(i == line_len / 2 - 2)  { __fprintf(stdout, "NULL"); i += 3; }
                else                            __fprintf(stdout, " ");
            }

            break;
        } 

        if(i < node->keyNum + 1 && i < b)
            if(node->child[i] == NULL)
                __fprintf(stdout, " NULL \U00002502");
            else if(i == b - 1)
                __fprintf(stdout, " %04d \U00002503\n", node->child[i]->el[0].key);
            else
                __fprintf(stdout, " %04d \U00002502", node->child[i]->el[0].key);
        
        else if(i < b - 1)
            __fprintf(stdout, " XXXX \U00002502");
        
        else
            __fprintf(stdout, " XXXX \U00002503\n");
    }

    /* Print 9th line (lower frame)
    ----------------------------------------------------------------*/
    for(int i = 0; i < line_len; i++) {
        if(i == line_len - 1)           __fprintf(stdout, "\U0000251B\n");
        else if(i == 0)                 __fprintf(stdout, "\U00002517");
        else if(i % 7 == 0)             __fprintf(stdout, "\U00002537");
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
        for(uint16_t i = 0; i < node->keyNum + 1; i++){
            uint32_t h = abTree_getNodeHeight(node->child[i]);
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
    else if(level > 1){
        for(uint16_t i = 0; i < node->keyNum + 1; i++){
            if(!node->isLeaf)
                abTree_printLevel(node->child[i], level - 1, b);
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

/* 
----------------------------------------------------------------*/
__static abNode_t* abTree_createNode1El(abNode_t const * parent, int32_t key, void const * data, abNode_t const * left_child, abNode_t const * right_child, bool isLeaf)
{
    abElement_t el = { key };
    abNode_t const * child[] = {left_child, right_child};
    return abTree_createNodeNEl(parent, &el, 1, ( (left_child == NULL && right_child == NULL) ? NULL : child), isLeaf);
}



void abTree_freeNode(abNode_t* node){
    __free(node->el);
    __free(node->child);
    __free(node);
}

int32_t abTree_insertEl(abNode_t* node, int32_t key, void* data, abNode_t* new_child){

    node->keyNum++;
    node->el = (abElement_t*)__realloc(node->el, node->keyNum * sizeof(abElement_t));

    if(!node->isLeaf){
        node->child = (abNode_t**)__realloc(node->child, (node->keyNum + 1) * sizeof(abNode_t*));
    }

    int32_t i;
    for(i = node->keyNum; i > 0; i--){

        if(!node->isLeaf){
            if(key < node->child[i - 1]->el[0].key){
                node->child[i] = node->child[i - 1]; 
            }
        }

        if(i < node->keyNum){
            if(node->el[i - 1].key < key) break;

            memcpy(&node->el[i], &node->el[i - 1], sizeof(abElement_t));
        }
    }

    node->el[i].key = key;
    node->el[i].data = data;

    if(!node->isLeaf){
        if(new_child->el[0].key > key)
            node->child[i + 1] = new_child;
        else
            node->child[i] = new_child;
    }

    return i;
}

abNode_t* abTree_splitNode(abNode_t* node){

    uint16_t center_pos = (node->keyNum - 1)/ 2;

    abElement_t el = node->el[center_pos];

    abNode_t* parent = node->parent != NULL ? node->parent : node;
    abNode_t* new_node   = abTree_createNodeNEl(parent,     &node->el[0],              center_pos,                     ( (node->isLeaf) ? NULL : (abNode_t const * const *) &node->child[0]),                 node->isLeaf);
    abNode_t* new_sister = abTree_createNodeNEl(parent,     &node->el[center_pos + 1], node->keyNum - center_pos - 1,  ( (node->isLeaf) ? NULL : (abNode_t const * const *) &node->child[center_pos + 1]),    node->isLeaf);
    if(new_node == NULL || new_sister == NULL){
        abTree_freeNode(new_node);
        abTree_freeNode(new_sister);            
        return NULL;
    } 

    if(!node->isLeaf){
        for(int32_t i = 0; i < node->keyNum + 1; i++){
            if(i <= center_pos)
                node->child[i]->parent = new_node;
            else if(i > center_pos)
                node->child[i]->parent = new_sister;
        }
    }

    if(node->parent != NULL){
        int32_t i = abTree_insertEl(node->parent, el.key, el.data, new_sister);
        node->parent->child[i] = new_node;
        abTree_freeNode(node);

        return parent;
    }
    else{       
        node->el    = (abElement_t*)__realloc(node->el, sizeof(abElement_t));
        node->child = (abNode_t**)__realloc(node->child, 2 * sizeof(abNode_t*));

        node->el[0].key = el.key;
        node->el[0].data = el.data;

        node->child[0] = new_node;
        node->child[1] = new_sister;
        node->isLeaf = false;
        node->keyNum = 1;

        return node;
    }  
}

void abTree_freeTree(abNode_t* node){
    if(node == NULL) return;

    for(int32_t i = 0; !node->isLeaf && i < node->keyNum + 1; i++){
        abTree_freeTree(node->child[i]);
    }

    abTree_freeNode(node);
}

abNode_t* abTree_searchNode(abNode_t* node, int32_t key, uint16_t* pos, abNode_t** insert_node){
   
    while(node != NULL){
        for(int32_t i = 0; i < node->keyNum + 1; i++){
            if(i == node->keyNum || key < node->el[i].key){
                if(node->isLeaf){
                    if(insert_node != NULL)
                        *insert_node = node;

                    node = NULL;
                }
                else
                    node = node->child[i];
                break;
            }

            else if(key == node->el[i].key){
                if(pos != NULL) 
                    *pos = i;
                return node;
            }
        }
    }

    return NULL;
}


int32_t abTree_removeEl(abNode_t* node, int32_t key, abNode_t** remchild){

    int32_t i = 0;
    for(i = 0; i < node->keyNum && key > node->el[i].key; i++) 
        continue;

    if(!node->isLeaf && remchild != NULL)
        *remchild = node->child[i];

    for(int32_t j = i; j < node->keyNum; j++){ 

        if(j < node->keyNum - 1)      
            memcpy(&node->el[j], &node->el[j + 1], sizeof(abElement_t));

        if(!node->isLeaf){
            node->child[j] = node->child[j + 1]; 
        }
    }
    
    node->keyNum--;
    node->el = (abElement_t*) ( (node->keyNum == 0) ? NULL : __realloc(node->el,          node->keyNum * sizeof(abElement_t)) );

    if(!node->isLeaf){
        node->child = (abNode_t**) ( (node->keyNum == 0) ? NULL : __realloc(node->child,   (node->keyNum + 1) * sizeof(abNode_t*)) );
    }

    return i;
}



/**********************************
 * PUBLIC FUNCTIONS
 **********************************/

void* abTree_search(abTree_t* tree, int32_t key){
    uint16_t pos = 0;
    abNode_t* node = abTree_searchNode(tree->root, key, &pos, NULL);

    return node != NULL ? NULL : NULL;
}

/*
-----------------------------------------*/
abTree_t* abTree_create(uint16_t a, uint16_t b, void (*abTree_conflictCB)(abNode_t* node, int32_t key, void* data))
{
    abTree_t* tree = (abTree_t*)__malloc(sizeof(abTree_t));
    if(tree == NULL)
        return NULL;

    tree->a = a;
    tree->b = b;
    tree->conflict_cb = abTree_conflictCB;
    tree->root = NULL;

    return tree;
}

/*
-------------------------------------------------------------*/
void abTree_insert(abTree_t* tree, int32_t key, void* data){
    if(tree->root != NULL){
        abNode_t* insert_node;
        abNode_t* node = abTree_searchNode(tree->root, key, NULL, &insert_node);
        if(node != NULL){
            if(tree->conflict_cb != NULL) tree->conflict_cb(node, key, data);
            return;
        }

        abTree_insertEl(insert_node, key, data, NULL);
        while(insert_node != NULL && insert_node->keyNum >= tree->b){
            insert_node = abTree_splitNode(insert_node);
        }
    }
    else{
        tree->root = abTree_createNode1El(NULL, key, data, NULL, NULL, true);
    }
}


void* abTree_remove(abTree_t* tree, int32_t key){
    uint16_t pos;
    abNode_t* node = abTree_searchNode(tree->root, key, &pos, NULL);
    if(node == NULL)
        return NULL;

    void* data = node->el[pos].data;

    if(!node->isLeaf){
        abNode_t* substitute = node->child[pos];
        while(!substitute->isLeaf){
            substitute = substitute->child[substitute->keyNum];
        }

        abElement_t swap = substitute->el[substitute->keyNum - 1];
        memcpy(&substitute->el[substitute->keyNum - 1],     &node->el[pos],     sizeof(abElement_t));
        memcpy(&node->el[pos],                              &swap,              sizeof(abElement_t));

        node = substitute;
        pos = substitute->keyNum - 1;
    }

    abTree_removeEl(node, key, NULL);

    while( (node->parent == NULL && node->keyNum <= 0) || (node->parent != NULL && node->keyNum < tree->a - 1)){

        if(node->parent == NULL){
            if(node->keyNum == 0){
                tree->root = node->child == NULL ? NULL : node->child[0];
                tree->root->parent = NULL;
                abTree_freeNode(node);
            }
        }

        else if(node->keyNum < tree->a - 1){
            int32_t i;
            for(i = 0; i < node->parent->keyNum + 1; i++){
                if(node->parent->child[i] == node) 
                    break;
            }

            abNode_t* neighbor_left =                     (i == 0) ? NULL : node->parent->child[i - 1];
            abNode_t* neighbor_right = (i == node->parent->keyNum) ? NULL : node->parent->child[i + 1];
            
            if(neighbor_left != NULL && neighbor_left->keyNum > tree->a - 1) {
                neighbor_left->child[neighbor_left->keyNum - 1] = node;
                abTree_insertEl(node, node->parent->el[i - 1].key, node->parent->el[i - 1].data, neighbor_left->isLeaf ? NULL : neighbor_left->child[neighbor_left->keyNum - 1]);
                memcpy(&node->parent->el[i - 1],    &neighbor_left->el[neighbor_left->keyNum - 1],  sizeof(abElement_t));
                abTree_removeEl(neighbor_left,       neighbor_left->el[neighbor_left->keyNum - 1].key,  NULL);
            }

            else if(neighbor_right != NULL && neighbor_right->keyNum > tree->a - 1) {
                neighbor_right->child[0]->parent = node;
                abTree_insertEl(node, node->parent->el[i].key, node->parent->el[i].data, neighbor_right->isLeaf ? NULL : neighbor_right->child[0]);
                memcpy(&node->parent->el[i],            &neighbor_right->el[0],  sizeof(abElement_t));
                abTree_removeEl(neighbor_right,          neighbor_right->el[0].key,  NULL);
            }

            else {
                abNode_t* ins_node = neighbor_left != NULL ? neighbor_left : neighbor_right;
                int32_t elPos = ins_node == neighbor_left ? i - 1 : i;

                abTree_insertEl(ins_node, node->parent->el[elPos].key, node->parent->el[i].data, node->isLeaf ? NULL : node->child[0]);
                for(int32_t j = 0; j < node->keyNum; j++){
                    abTree_insertEl(ins_node, node->el[j].key, node->el[j].data, node->isLeaf ? NULL : node->child[j + 1]);

                    if(!node->isLeaf){
                        if(j == 0) 
                            node->child[j]->parent = ins_node;

                        node->child[j + 1]->parent = ins_node;
                    }
                }

                abTree_removeEl(node->parent, node->parent->el[elPos].key, node->parent->el[elPos].data);
                node->parent->child[elPos] = ins_node;
                abTree_freeNode(node);
                node = ins_node->parent;
            }
        }
    }

    return data;
}

/*
-------------------------------------------------------------*/
void abTree_print(abTree_t* tree){
    uint32_t h = abTree_getNodeHeight(tree->root);
    for(uint16_t i = 1; i <= h; i++){
        abTree_printLevel(tree->root, i, tree->b);
        __fprintf(stdout, "\n");
    }

    __fprintf(stdout, "____________________________________________________________________________\n");
}

void abTree_destroy(abTree_t* tree){
    if(tree->root != NULL)
        abTree_freeTree(tree->root);
        
    __free(tree);
}
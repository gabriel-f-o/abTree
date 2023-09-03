#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

/**********************************************************
 * 
 *********************************************************/

abTree_t* cur_tree;

/**********************************************************
 * 
 *********************************************************/

void abTree_printNode(abNode_t* node, uint32_t level){
    if(node == NULL) return;

    const int line_len = cur_tree->b * 7 + 1;

    for(int i = 0; i < line_len; i++) {
             if(i == 0)                 printf("\U0000250F");
        else if(i == 7)                 printf("\U0000252F");
        else if(i == line_len - 1)      printf("\U00002513\n");
        else                            printf("\U00002501");
    }

    int pos = -1;
    for(int i = 0; i < line_len; i++) {
             if(i == 0)                 { printf("\U00002503 L%03d ", level); i += 6; }
        else if(i == 7)                   printf("\U00002502");
        else if(i == line_len - 1)        printf("\U00002503\n");
        else  { 
            if(pos == -1) pos = i;
            
            if(i < (line_len + pos) / 2 - 2)
                printf(" ");
            
            else if(i == (line_len + pos) / 2 - 2){
                printf("%s", node->isLeaf ? "Leaf" : "Node");
                i += 3;
            }

            else{
                printf(" ");
            }
        }
    }

    for(int i = 0; i < line_len; i++) {
        if(i == 0)                      printf("\U00002520");
        else if(i == line_len - 1)      printf("\U00002528\n");
        else if(i == 7)                 printf("\U00002534");
        else                            printf("\U00002500");
    }

    for(int i = 0; i < line_len; i++) {
             if(i == 0)                   printf("\U00002503");
        else if(i == line_len - 1)        printf("\U00002503\n");
        else  {             
            if(i < (line_len) / 2 - 2)
                printf(" ");
            
            else if(i == (line_len) / 2 - 2){
                if(node->parent == NULL) 
                    printf("NULL");
                else
                    printf("%04d", node->parent->el[0].key);
                i += 3;
            }

            else{
                printf(" ");
            }
        }
    }

    for(int i = 0; i < line_len; i++) {
        if(i == 0)                      printf("\U00002520");
        else if(i == line_len - 1)      printf("\U00002528\n");
        else if(i == 7)                 printf("\U0000252C");
        else if(i > 7 && i % 7 == 0)    printf("\U0000252C");
        else                            printf("\U00002500");
    }

    printf ("\U00002503");
    for(uint16_t i = 0; i < cur_tree->b - 1; i++){
        if(node->el == NULL){
            for(int i = 1; i < line_len; i++) {
                if(i == line_len - 1)           printf("\U00002503\n");
                else if(i < line_len / 2 - 2)   printf(" ");
                else if(i == line_len / 2 - 2)  { printf("NULL"); i += 3; }
                else                            printf(" ");
            }

            break;
        } 

        if(i < node->keyNum)
            printf(" %04d \U00002502", node->el[i].key);
        else
            printf(" XXXX \U00002502");
    }

    if(node->el != NULL)
        printf(" ---- \U00002503\n");

    for(int i = 0; i < line_len; i++) {
        if(i == 0)                      printf("\U00002520");
        else if(i == line_len - 1)      printf("\U00002528\n");
        else if(i % 7 == 0)             printf("\U0000253C");
        else                            printf("\U00002500");
    }

    printf ("\U00002503");
    for(uint16_t i = 0; i < cur_tree->b; i++){
        if(node->child == NULL){
            for(int i = 1; i < line_len; i++) {
                if(i == line_len - 1)           printf("\U00002503\n");
                else if(i < line_len / 2 - 2)   printf(" ");
                else if(i == line_len / 2 - 2)  { printf("NULL"); i += 3; }
                else                            printf(" ");
            }

            break;
        } 

        if(i < node->keyNum + 1 && i < cur_tree->b)
            if(node->child[i] == NULL)
                printf(" NULL \U00002502");
            else if(i == cur_tree->b - 1)
                printf(" %04d \U00002503\n", node->child[i]->el[0].key);
            else
                printf(" %04d \U00002502", node->child[i]->el[0].key);
        
        else if(i < cur_tree->b - 1)
            printf(" XXXX \U00002502");
        
        else
            printf(" XXXX \U00002503\n");
    }

    for(int i = 0; i < line_len; i++) {
        if(i == line_len - 1)           printf("\U0000251B\n");
        else if(i == 0)                 printf("\U00002517");
        else if(i % 7 == 0)             printf("\U00002537");
        else                            printf("\U00002501");
    }
}

uint32_t abTree_getNodeHeight(abNode_t* node){
    if(node == NULL)
        return 0;
    else if(node->isLeaf)
        return 1;
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

void abTree_printLevel(abNode_t* node, uint32_t level){
    if(node == NULL)
        return;
    if(level == 1){ 
        abTree_printNode(node, abTree_getNodeHeight(node));
    }
    else if(level > 1){
        for(uint16_t i = 0; i < node->keyNum + 1; i++){
            if(node->child != NULL)
                abTree_printLevel(node->child[i], level - 1);
        }
    }
}



abNode_t* abTree_createNodeNEl(abNode_t* parent, abElement_t new_el[], size_t el_len, abNode_t** new_child, bool isLeaf){
    abNode_t* node = (abNode_t*)malloc(sizeof(abNode_t));
    abElement_t* el = (abElement_t*)malloc(el_len * sizeof(abElement_t));
    abNode_t** child = isLeaf ? NULL : (abNode_t**)malloc((el_len + 1) * sizeof(abNode_t*));

    if(node == NULL || el == NULL || (child == NULL && !isLeaf)){
        free(node);
        free(el);
        free(child);
        return NULL;
    }

    memcpy(el, new_el, el_len * sizeof(abElement_t));

    node->el = el;
    node->keyNum = el_len;
    node->child = child;
    node->parent = parent;
    node->isLeaf = isLeaf;

    if(!isLeaf && new_child != NULL){
        memcpy(child, new_child, (el_len + 1) * sizeof(abNode_t*));
    }

    return node;
}

abNode_t* abTree_createNode1El(abNode_t* parent, int32_t key, void* data, abNode_t* left_child, abNode_t* right_child, bool isLeaf){
    abElement_t el = { .data = data, .key = key };
    abNode_t* child[] = {left_child, right_child};
    return abTree_createNodeNEl(parent, &el, 1, ( (left_child == NULL && right_child == NULL) ? NULL : child), isLeaf);
}



void abTree_freeNode(abNode_t* node){
    free(node->el);
    free(node->child);
    free(node);
}

int32_t abTree_insertEl(abNode_t* node, int32_t key, void* data, abNode_t* left_neighbor, abNode_t* right_neighbor){

    node->keyNum++;
    node->el = (abElement_t*)realloc(node->el, node->keyNum * sizeof(abElement_t));

    if(node->child != NULL){
        node->child = (abNode_t**)realloc(node->child, (node->keyNum + 1) * sizeof(abNode_t*));
    }

    int32_t i;
    for(i = node->keyNum - 1; i > 0 && node->el[i - 1].key > key; --i){
        if(node->child != NULL){
            if(i == node->keyNum - 1)
                node->child[i + 1] = node->child[i];

            node->child[i] = node->child[i - 1]; 
        }

        memcpy(&node->el[i], &node->el[i - 1], sizeof(abElement_t));
    }

    node->el[i].key = key;
    node->el[i].data = data;

    if(node->child != NULL){
        node->child[i] = left_neighbor;
        node->child[i + 1] = right_neighbor;
    }

    return i;
}

abNode_t* abTree_splitNode(abNode_t* node){

    uint16_t center_pos = (node->keyNum - 1)/ 2;

    abElement_t el = node->el[center_pos];

    abNode_t* parent = node->parent != NULL ? node->parent : node;
    abNode_t* new_node   = abTree_createNodeNEl(parent,     &node->el[0],              center_pos,                     ( (node->child == NULL) ? NULL : &node->child[0]),                 node->isLeaf);
    abNode_t* new_sister = abTree_createNodeNEl(parent,     &node->el[center_pos + 1], node->keyNum - center_pos - 1,  ( (node->child == NULL) ? NULL : &node->child[center_pos + 1]),    node->isLeaf);
    if(new_node == NULL || new_sister == NULL){
        abTree_freeNode(new_node);
        abTree_freeNode(new_sister);            
        return NULL;
    } 

    if(node->child != NULL){
        for(int32_t i = 0; i < node->keyNum + 1; i++){
            if(i <= center_pos)
                node->child[i]->parent = new_node;
            else if(i > center_pos)
                node->child[i]->parent = new_sister;
        }
    }

    if(node->parent != NULL){
        int32_t i = abTree_insertEl(node->parent, el.key, el.data, new_node, new_sister);
        node->parent->child[i] = new_node;
        node->parent->child[i+1] = new_sister;
        abTree_freeNode(node);

        return parent;
    }
    else{       
        node->el    = (abElement_t*)realloc(node->el, sizeof(abElement_t));
        node->child = (abNode_t**)realloc(node->child, 2 * sizeof(abNode_t*));

        node->el->data = el.data;
        node->el->key = el.key;

        node->child[0] = new_node;
        node->child[1] = new_sister;
        node->isLeaf = false;
        node->keyNum = 1;

        return node;
    }  
}

void abTree_freeTree(abNode_t* node){
    for(int32_t i = 0; node->child != NULL && i < node->keyNum + 1; i++){
        abTree_freeTree(node->child[i]);
    }

    abTree_freeNode(node);
}

abNode_t* abTree_searchNode(abNode_t* node, int32_t key, uint16_t* pos, abNode_t** insert_node){
   
    while(node != NULL){
        for(int32_t i = 0; i < node->keyNum + 1; i++){
            if(i == node->keyNum || key < node->el[i].key){
                if(node->child == NULL){
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

int32_t abTree_removeElement(abNode_t* node, int32_t key, abNode_t* left_child, abNode_t* right_child){

    int32_t i = 0;
    for(i = 0; i < node->keyNum - 2 && key > node->el[i + 1].key; i++) continue;


    for(int32_t j = i; j < node->keyNum - 2; j++){       
        memcpy(&node->el[j], &node->el[j + 1], sizeof(abElement_t));

        if(node->child!= NULL)
            node->child[j] = node->child[j + 1]; 
    }

    node->keyNum--;
    node->el = (abElement_t*) ( (node->keyNum == 0) ? NULL : realloc(node->el,          node->keyNum * sizeof(abElement_t)) );

    if(node->child != NULL){
        node->child = (abNode_t**) ( (node->keyNum == 0) ? NULL : realloc(node->child,   (node->keyNum + 1) * sizeof(abNode_t*)) );
        node->child[i] = left_child;
        node->child[i + 1] = right_child;
    }

    return i;
}



/**********************************
 * PUBLIC FUNCTIONS
 **********************************/

void* abTree_search(abTree_t* tree, int32_t key){
    uint16_t pos = 0;
    abNode_t* node = abTree_searchNode(tree->root, key, &pos, NULL);

    return node != NULL ? node->el[pos].data : NULL;
}

/*
-----------------------------------------*/
abTree_t* abTree_create(uint16_t a, uint16_t b, void (*abTree_conflictCB)(abNode_t* node, int32_t key, void* data))
{
    abTree_t* tree = (abTree_t*)malloc(sizeof(abTree_t));
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

        abTree_insertEl(insert_node, key, data, NULL, NULL);
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


    while(1){

        if(node->parent == NULL){
            abTree_removeElement(node, key, NULL, NULL);
            if(node->keyNum == 0){
                abTree_freeNode(node);
                tree->root = NULL;
            }
        }

        else if(node->keyNum <= tree->a - 1){
            int32_t i;
            for(i = 0; i < node->parent->keyNum + 1; i++){
                if(node->parent->child[i] == node) 
                    break;
            }

            abNode_t* neighbor_left =                     (i == 0) ? NULL : node->parent->child[i - 1];
            abNode_t* neighbor_right = (i == node->parent->keyNum) ? NULL : node->parent->child[i + 1];
            
            if(neighbor_left != NULL && neighbor_left->keyNum > tree->a - 1) {
                memcpy(&node->el[pos],                  &node->parent->el[i - 1],                       sizeof(abElement_t));
                memcpy(&node->parent->el[i - 1],        &neighbor_left->el[neighbor_left->keyNum - 1],  sizeof(abElement_t));
                abTree_removeElement(neighbor_left,     neighbor_left->el[neighbor_left->keyNum - 1].key, NULL, NULL);
            }

            else if(neighbor_right != NULL && neighbor_right->keyNum > tree->a - 1) {
                memcpy(&node->el[pos],                  &node->parent->el[i],   sizeof(abElement_t));
                memcpy(&node->parent->el[i],            &neighbor_right->el[0], sizeof(abElement_t));
                abTree_removeElement(neighbor_right,    neighbor_right->el[0].key, NULL, NULL);
            }

            else {
                //memcpy(&node->el[pos], &node->parent->el[i == node->parent->keyNum + 1 ? i - 1 : i], sizeof(abElement_t));
                abNode_t* ins_node = neighbor_left != NULL ? neighbor_left : neighbor_right;
                int32_t elPos = ins_node == neighbor_left ? i - 1 : i;
                abElement_t el = node->parent->el[elPos];

                abTree_insertEl(ins_node, el.key, el.data, NULL, NULL);
                abTree_removeElement(node->parent, el.key,node->parent->child[elPos], node->parent->child[elPos + 1]);
                abTree_removeElement(node, key, NULL, NULL);
                abTree_freeNode(node);
                
                /*int32_t elPos = i == node->parent->keyNum + 1 ? i - 1 : i;
                abElement_t el = node->parent->el[i == node->parent->keyNum + 1 ? i - 1 : i];
                abTree_insertEl(neighbor_left != NULL ? neighbor_left : neighbor_right, el.key, el.data, NULL, NULL);
                abTree_removeElement(node, key, NULL, NULL);
                abTree_removeElement(node->parent, el.key,node->parent->child[elPos - 1], node->parent->child[elPos + 1]);
                abTree_freeNode(node);*/
            }

        }

        else{
            abTree_removeElement(node, key, NULL, NULL);
        }

        break;
    }

    return data;
}

/*
-------------------------------------------------------------*/
void abTree_print(abTree_t* tree){
    cur_tree = tree;
    uint32_t h = abTree_getNodeHeight(tree->root);
    for(uint16_t i = 1; i <= h; i++){
        abTree_printLevel(tree->root, i);
        printf("\n");
    }

    printf("------------------------------------------------\n\n");
}

void abTree_destroy(abTree_t* tree){
    if(tree->root != NULL)
        abTree_freeTree(tree->root);
        
    free(tree);
}
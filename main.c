#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void abTree_conflictCB(abNode_t* node, int32_t key, void* data){
    printf("Conflict key %d\n", key);
}

int main(){
   
    /*abTree_t* tree = abTree_create(2, 4, &abTree_conflictCB);

    abTree_insert(tree, 10, NULL);
    abTree_insert(tree, 1000, NULL);
    abTree_insert(tree, 1, NULL);

    abTree_insert(tree, 100, NULL);

    abTree_insert(tree, 2, NULL);
    abTree_insert(tree, 50, NULL);    

    abTree_insert(tree, 20, (void*) 69);
    abTree_insert(tree, 30, NULL);
    abTree_insert(tree, 40, NULL);

    abTree_insert(tree, 15, NULL);
    abTree_insert(tree, 3, NULL);
  
    abTree_insert(tree, 4, NULL);

    abTree_insert(tree, 1500, NULL);
    abTree_insert(tree, 2000, (void*) 69);
    abTree_insert(tree, 3000, (void*) 71);
    abTree_insert(tree, 4000, (void*) 73);

    abTree_print(tree);
    abTree_destroy(tree);*/

    abTree_t* tree = abTree_create(2, 4, abTree_conflictCB);
    abTree_insert(tree, 100, NULL);
    abTree_insert(tree, 200, NULL);
    abTree_insert(tree, 300, NULL);
    abTree_insert(tree, 400, NULL);
    abTree_insert(tree, 500, NULL);
    abTree_insert(tree, 600, NULL);

    abTree_print(tree);

    //abTree_remove(tree, 400);
    abTree_remove(tree, 600);
    abTree_remove(tree, 300);
    //abTree_remove(tree, 400);
    //abTree_remove(tree, 600);
    abTree_print(tree);

    return 0;
}

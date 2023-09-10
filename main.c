#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void abTree_conflictCB(abNode_t* node, int32_t key, void* data){
    printf("Conflict key %d\n", key);
}

int main(){
   
#if 0
    abTree_t* tree = abTree_create(2, 4, &abTree_conflictCB);

    abTree_insert(tree, 10, NULL);
    abTree_insert(tree, 1000, NULL);
    abTree_insert(tree, 1, NULL);

    abTree_insert(tree, 100, NULL);

    abTree_insert(tree, 2, NULL);
    abTree_insert(tree, 50, NULL);    

    abTree_insert(tree, 20, NULL);
    
    abTree_insert(tree, 30, NULL);
    abTree_insert(tree, 40, NULL);

    abTree_insert(tree, 15, NULL);
    abTree_insert(tree, 3, NULL);

    abTree_insert(tree, 4, NULL);

    abTree_insert(tree, 1500, NULL);
    abTree_insert(tree, 2000, NULL);
    abTree_insert(tree, 3000, NULL);
    abTree_insert(tree, 4000, NULL);

    abTree_print(tree);
    abTree_destroy(tree);
    
#else
    abTree_t* tree = abTree_create(3, 5, abTree_conflictCB);
    /*abTree_insert(tree, 100, NULL);
    abTree_insert(tree, 200, NULL);
    abTree_insert(tree, 300, NULL);
    abTree_insert(tree, 400, NULL);
    abTree_insert(tree, 500, NULL);
    abTree_insert(tree, 600, NULL);
    abTree_insert(tree, 700, NULL);
    abTree_insert(tree, 800, NULL);*/
    //abTree_print(tree);

    for(int i = 100; i <= 2000; i += 100)
        abTree_insert(tree, i, NULL);

    abTree_insert(tree, 801, NULL);
    abTree_insert(tree, 802, NULL);
    abTree_insert(tree, 803, NULL);

    abTree_remove(tree, 1400);
    abTree_remove(tree, 900);
    abTree_remove(tree, 803);
    abTree_remove(tree, 802);
    abTree_remove(tree, 801);
    abTree_print(tree);
#endif
    return 0;
}


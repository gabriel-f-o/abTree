#include "tree.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tests_mocks.h"

/**********************************************************
 * EXTERNAL VARIABLES
 *********************************************************/

extern bool malloc_enable;
extern bool print_enable;

typedef enum {
    ABTREE_KEEP_LEFT_CHILD,
    ABTREE_KEEP_RIGHT_CHILD
}abTree_keepChild_e;

/**********************************************************
 * EXTERNAL C FUNCTIONS
 *********************************************************/

extern "C"{
    /*abNode_t* abTree_createNode1El(abNode_t const * parent, int32_t key, void const * data, abNode_t const * left_child, abNode_t const * right_child, bool isLeaf);
    abNode_t* abTree_createNodeNEl(abNode_t const * parent, abElement_t const new_el[], size_t el_len, abNode_t const * const new_child[], bool isLeaf);
    void abTree_printNode(abNode_t const * node, uint32_t level, uint16_t b);
    void abTree_freeTree(abNode_t* node);
    int32_t abTree_insertEl(abNode_t* node, int32_t key, void const * data, abNode_t const * new_child);
    abNode_t* abTree_splitNode(abNode_t* node);
    abNode_t* abTree_searchNode(abNode_t* node, int32_t key, uint16_t* pos, abNode_t** insert_node);*/

    void abTree_printNode(abNode_t const * node, uint32_t level, uint16_t b);
    int32_t abTree_insertEl(abNode_t* node, int32_t key, void const * data, abNode_t const * new_child);
    int32_t abTree_removeEl(abNode_t* node, int32_t key, abTree_keepChild_e keep);
    abNode_t* abTree_createNode1El(abNode_t const * parent, int32_t key, void const * data, abNode_t const * left_child, abNode_t const * right_child, bool isLeaf);
    abNode_t* abTree_createNodeNEl(abNode_t const * parent, abElement_t const new_el[], size_t el_len, abNode_t const * const new_child[], bool isLeaf);
    void abTree_freeTree(abNode_t* node);
    uint32_t abTree_getNodeHeight(abNode_t const * node);
    void abTree_printLevel(abNode_t const * node, uint32_t level, uint16_t b);
}

void print_subTree(abNode_t* n, int b){
    uint32_t h = abTree_getNodeHeight(n);
    for(uint16_t i = 1; i <= h; i++){
        abTree_printLevel(n, i, b);
        __fprintf(stdout, "\n");
    }

    __fprintf(stdout, "____________________________________________________________________________\n");
}

/**********************************************************
 * PUBLIC FUNCTIONS
 *********************************************************/

TEST_GROUP(removeTest)
{
    void setup()
    {
        mock().clear();
        mock().disable();
    }

    void teardown()
    {
        mock().clear();
    }
};

TEST(removeTest, remove_4_7)
{
    abTree_t* t = abTree_create(4, 7, NULL);
    CHECK(t != NULL);

    for(int i = 100; i <= 3900; i += 200)
        abTree_insert(t, i, (void*)((intptr_t)i));

    for(int i = 200; i <= 4000; i += 200)
        abTree_insert(t, i, (void*)((intptr_t)i));


    CHECK_EQUAL((void*)NULL, abTree_remove(t, 69));
    CHECK_EQUAL(1900ULL, (intptr_t) abTree_remove(t, 1900));
    CHECK_EQUAL(3300ULL, (intptr_t) abTree_remove(t, 3300));
    CHECK_EQUAL(2900ULL, (intptr_t) abTree_remove(t, 2900));

    abTree_insert(t, 1900, (void*) 1900);
    abTree_insert(t, 3300, (void*) 3300);
    abTree_insert(t, 2900, (void*) 2900);

    CHECK_EQUAL(900ULL, (intptr_t) abTree_remove(t, 900));

    abTree_insert(t, 4100, (void*) 4100);
    abTree_insert(t, 4200, (void*) 4200);
    abTree_insert(t, 4300, (void*) 4300);

    CHECK_EQUAL(800ULL, (intptr_t) abTree_remove(t, 800));
    CHECK_EQUAL(600ULL, (intptr_t) abTree_remove(t, 600));
    CHECK_EQUAL(700ULL, (intptr_t) abTree_remove(t, 700));
    CHECK_EQUAL(1000ULL, (intptr_t) abTree_remove(t, 1000));
    CHECK_EQUAL(1100ULL, (intptr_t) abTree_remove(t, 1100));
    CHECK_EQUAL(1200ULL, (intptr_t) abTree_remove(t, 1200));

    abTree_insert(t, 600, (void*)600);
    
    CHECK_EQUAL(2800ULL, (intptr_t) abTree_remove(t, 2800));    
    CHECK_EQUAL(3000ULL, (intptr_t) abTree_remove(t, 3000));
    CHECK_EQUAL(1400ULL, (intptr_t) abTree_remove(t, 1400));
    CHECK_EQUAL( 600ULL, (intptr_t) abTree_remove(t, 600));
    CHECK_EQUAL( 500ULL, (intptr_t) abTree_remove(t, 500));
    CHECK_EQUAL( 400ULL, (intptr_t) abTree_remove(t, 400));
    CHECK_EQUAL( 300ULL, (intptr_t) abTree_remove(t, 300));


    CHECK_EQUAL((void*)NULL, t->root->parent);
    CHECK_EQUAL(5, t->root->keyNum);
    CHECK_EQUAL(1800, t->root->el[0].key);
    CHECK_EQUAL(2300, t->root->el[1].key);
    CHECK_EQUAL(2700, t->root->el[2].key);
    CHECK_EQUAL(3600, t->root->el[3].key);
    CHECK_EQUAL(4000, t->root->el[4].key);
    CHECK_EQUAL(1800LL, (intptr_t) t->root->el[0].data);
    CHECK_EQUAL(2300LL, (intptr_t) t->root->el[1].data);
    CHECK_EQUAL(2700LL, (intptr_t) t->root->el[2].data);
    CHECK_EQUAL(3600LL, (intptr_t) t->root->el[3].data);
    CHECK_EQUAL(4000LL, (intptr_t) t->root->el[4].data);


    CHECK_EQUAL(t->root, t->root->child[0]->parent);
    CHECK_EQUAL(   6, t->root->child[0]->keyNum);
    CHECK_EQUAL( 100, t->root->child[0]->el[0].key);
    CHECK_EQUAL( 200, t->root->child[0]->el[1].key);
    CHECK_EQUAL(1300, t->root->child[0]->el[2].key);
    CHECK_EQUAL(1500, t->root->child[0]->el[3].key);
    CHECK_EQUAL(1600, t->root->child[0]->el[4].key);
    CHECK_EQUAL(1700, t->root->child[0]->el[5].key);
    CHECK_EQUAL( 100, (intptr_t) t->root->child[0]->el[0].data);
    CHECK_EQUAL( 200, (intptr_t) t->root->child[0]->el[1].data);
    CHECK_EQUAL(1300, (intptr_t) t->root->child[0]->el[2].data);
    CHECK_EQUAL(1500, (intptr_t) t->root->child[0]->el[3].data);
    CHECK_EQUAL(1600, (intptr_t) t->root->child[0]->el[4].data);
    CHECK_EQUAL(1700, (intptr_t) t->root->child[0]->el[5].data);

    CHECK_EQUAL(t->root, t->root->child[1]->parent);
    CHECK_EQUAL(   4, t->root->child[1]->keyNum);
    CHECK_EQUAL(1900, t->root->child[1]->el[0].key);
    CHECK_EQUAL(2000, t->root->child[1]->el[1].key);
    CHECK_EQUAL(2100, t->root->child[1]->el[2].key);
    CHECK_EQUAL(2200, t->root->child[1]->el[3].key);
    CHECK_EQUAL(1900, (intptr_t) t->root->child[1]->el[0].data);
    CHECK_EQUAL(2000, (intptr_t) t->root->child[1]->el[1].data);
    CHECK_EQUAL(2100, (intptr_t) t->root->child[1]->el[2].data);
    CHECK_EQUAL(2200, (intptr_t) t->root->child[1]->el[3].data);

    CHECK_EQUAL(t->root, t->root->child[2]->parent);
    CHECK_EQUAL(   3, t->root->child[2]->keyNum);
    CHECK_EQUAL(2400, t->root->child[2]->el[0].key);
    CHECK_EQUAL(2500, t->root->child[2]->el[1].key);
    CHECK_EQUAL(2600, t->root->child[2]->el[2].key);
    CHECK_EQUAL(2400, (intptr_t) t->root->child[2]->el[0].data);
    CHECK_EQUAL(2500, (intptr_t) t->root->child[2]->el[1].data);
    CHECK_EQUAL(2600, (intptr_t) t->root->child[2]->el[2].data);

    CHECK_EQUAL(t->root, t->root->child[3]->parent);
    CHECK_EQUAL(   6, t->root->child[3]->keyNum);
    CHECK_EQUAL(2900, t->root->child[3]->el[0].key);
    CHECK_EQUAL(3100, t->root->child[3]->el[1].key);
    CHECK_EQUAL(3200, t->root->child[3]->el[2].key);
    CHECK_EQUAL(3300, t->root->child[3]->el[3].key);
    CHECK_EQUAL(3400, t->root->child[3]->el[4].key);
    CHECK_EQUAL(3500, t->root->child[3]->el[5].key);
    CHECK_EQUAL(2900, (intptr_t) t->root->child[3]->el[0].data);
    CHECK_EQUAL(3100, (intptr_t) t->root->child[3]->el[1].data);
    CHECK_EQUAL(3200, (intptr_t) t->root->child[3]->el[2].data);
    CHECK_EQUAL(3300, (intptr_t) t->root->child[3]->el[3].data);
    CHECK_EQUAL(3400, (intptr_t) t->root->child[3]->el[4].data);
    CHECK_EQUAL(3500, (intptr_t) t->root->child[3]->el[5].data);

    CHECK_EQUAL(t->root, t->root->child[4]->parent);
    CHECK_EQUAL(   3, t->root->child[4]->keyNum);
    CHECK_EQUAL(3700, t->root->child[4]->el[0].key);
    CHECK_EQUAL(3800, t->root->child[4]->el[1].key);
    CHECK_EQUAL(3900, t->root->child[4]->el[2].key);
    CHECK_EQUAL(3700, (intptr_t) t->root->child[4]->el[0].data);
    CHECK_EQUAL(3800, (intptr_t) t->root->child[4]->el[1].data);
    CHECK_EQUAL(3900, (intptr_t) t->root->child[4]->el[2].data);

    CHECK_EQUAL(t->root, t->root->child[5]->parent);
    CHECK_EQUAL(   3, t->root->child[5]->keyNum);
    CHECK_EQUAL(4100, t->root->child[5]->el[0].key);
    CHECK_EQUAL(4200, t->root->child[5]->el[1].key);
    CHECK_EQUAL(4300, t->root->child[5]->el[2].key);
    CHECK_EQUAL(4100, (intptr_t) t->root->child[5]->el[0].data);
    CHECK_EQUAL(4200, (intptr_t) t->root->child[5]->el[1].data);
    CHECK_EQUAL(4300, (intptr_t) t->root->child[5]->el[2].data);


    abTree_destroy(t);
}

TEST(removeTest, remove_root)
{
    abTree_t* t = abTree_create(2, 3, NULL);
    abTree_insert(t, 1, (void*) 1ULL);
    abTree_insert(t, 2, (void*) 2ULL);

    CHECK_EQUAL(1LL, (intptr_t) abTree_remove(t, 1));
    CHECK_EQUAL(2LL, (intptr_t) abTree_remove(t, 2));

    CHECK_EQUAL((void*) NULL, t->root);

    abTree_destroy(t);
}

TEST(removeTest, removeEl)
{
    abElement_t el[3] = { { (void*)10, 10 }, { (void*)20, 20 }, { (void*)30, 30 } };
    abElement_t c1[3] = { { (void*) 1,  1 }, { (void*) 2,  2 }, { (void*) 3,  3 } };
    abElement_t c2[3] = { { (void*)11, 11 }, { (void*)12, 12 }, { (void*)13, 13 } };
    abElement_t c3[3] = { { (void*)21, 21 }, { (void*)22, 22 }, { (void*)23, 23 } };
    abElement_t c4[3] = { { (void*)31, 31 }, { (void*)32, 32 }, { (void*)33, 33 } };

    abNode_t* n_c1 = abTree_createNodeNEl(NULL, c1, 3, NULL, true);
    abNode_t* n_c2 = abTree_createNodeNEl(NULL, c2, 3, NULL, true);
    abNode_t* n_c3 = abTree_createNodeNEl(NULL, c3, 3, NULL, true);
    abNode_t* n_c4 = abTree_createNodeNEl(NULL, c4, 3, NULL, true);

    abNode_t* children[] = { n_c1, n_c2, n_c3, n_c4 };

    abNode_t* node = abTree_createNodeNEl(NULL, el, 3, children, false);
    n_c1->parent = n_c2->parent = n_c3->parent = n_c4->parent = node;  

    CHECK_EQUAL(0, abTree_removeEl(node, 10, ABTREE_KEEP_LEFT_CHILD));
    abTree_insertEl(node, 10, (void*)10, n_c2);

    CHECK_EQUAL(0, abTree_removeEl(node, 10, ABTREE_KEEP_RIGHT_CHILD));
    abTree_insertEl(node, 10, (void*)10, n_c1);

    CHECK_EQUAL(1, abTree_removeEl(node, 20, ABTREE_KEEP_RIGHT_CHILD));
    abTree_insertEl(node, 20, (void*)20, n_c2);

    CHECK_EQUAL(1, abTree_removeEl(node, 20, ABTREE_KEEP_LEFT_CHILD));
    abTree_insertEl(node, 20, (void*)20, n_c3);

    CHECK_EQUAL(2, abTree_removeEl(node, 30, ABTREE_KEEP_RIGHT_CHILD));
    abTree_insertEl(node, 30, (void*)30, n_c3);

    CHECK_EQUAL(2, abTree_removeEl(node, 30, ABTREE_KEEP_LEFT_CHILD));
    abTree_insertEl(node, 30, (void*)30, n_c4);

    CHECK_EQUAL(1, abTree_removeEl(n_c2, 12, ABTREE_KEEP_LEFT_CHILD));

    CHECK_EQUAL(2, abTree_removeEl(n_c2, 59, ABTREE_KEEP_LEFT_CHILD));


    CHECK_EQUAL((void*)NULL, node->parent);
    CHECK_EQUAL(3, node->keyNum);
    CHECK_EQUAL(10, node->el[0].key);
    CHECK_EQUAL(20, node->el[1].key);
    CHECK_EQUAL(30, node->el[2].key);
    CHECK_EQUAL(10, (intptr_t) node->el[0].data);
    CHECK_EQUAL(20, (intptr_t) node->el[1].data);
    CHECK_EQUAL(30, (intptr_t) node->el[2].data);
    CHECK_EQUAL(false, node->isLeaf);

    CHECK_EQUAL((void*)node, node->child[0]->parent);
    CHECK_EQUAL(3, node->child[0]->keyNum);
    CHECK_EQUAL(1, node->child[0]->el[0].key);
    CHECK_EQUAL(2, node->child[0]->el[1].key);
    CHECK_EQUAL(3, node->child[0]->el[2].key);
    CHECK_EQUAL(1, (intptr_t) node->child[0]->el[0].data);
    CHECK_EQUAL(2, (intptr_t) node->child[0]->el[1].data);
    CHECK_EQUAL(3, (intptr_t) node->child[0]->el[2].data);

    CHECK_EQUAL((void*)node, node->child[1]->parent);
    CHECK_EQUAL(1, node->child[1]->keyNum);
    CHECK_EQUAL(11, node->child[1]->el[0].key);

    CHECK_EQUAL((void*)node, node->child[2]->parent);
    CHECK_EQUAL(3, node->child[2]->keyNum);
    CHECK_EQUAL(21, node->child[2]->el[0].key);
    CHECK_EQUAL(22, node->child[2]->el[1].key);
    CHECK_EQUAL(23, node->child[2]->el[2].key);

    CHECK_EQUAL((void*)node, node->child[3]->parent);
    CHECK_EQUAL(3, node->child[3]->keyNum);
    CHECK_EQUAL(31, node->child[3]->el[0].key);
    CHECK_EQUAL(32, node->child[3]->el[1].key);
    CHECK_EQUAL(33, node->child[3]->el[2].key);

    abTree_freeTree(node);
}
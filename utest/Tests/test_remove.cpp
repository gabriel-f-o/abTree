#include "tree.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tests_mocks.h"

/**********************************************************
 * EXTERNAL VARIABLES
 *********************************************************/

extern bool malloc_enable;
extern bool print_enable;

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
extern int minhajeba;
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

    abTree_insert(t, 1900, NULL);
    abTree_insert(t, 3300, NULL);
    abTree_insert(t, 2900, NULL);

    CHECK_EQUAL(900ULL, (intptr_t) abTree_remove(t, 900));

    abTree_insert(t, 4100, NULL);
    abTree_insert(t, 4200, NULL);
    abTree_insert(t, 4300, NULL);

    CHECK_EQUAL(800ULL, (intptr_t) abTree_remove(t, 800));
    CHECK_EQUAL(600ULL, (intptr_t) abTree_remove(t, 600));
    CHECK_EQUAL(700ULL, (intptr_t) abTree_remove(t, 700));
    CHECK_EQUAL(1000ULL, (intptr_t) abTree_remove(t, 1000));
    CHECK_EQUAL(1100ULL, (intptr_t) abTree_remove(t, 1100));
    CHECK_EQUAL(1200ULL, (intptr_t) abTree_remove(t, 1200));

    abTree_insert(t, 600, NULL);
    
    CHECK_EQUAL(2800ULL, (intptr_t) abTree_remove(t, 2800));    
    CHECK_EQUAL(3000ULL, (intptr_t) abTree_remove(t, 3000));

    print_enable = true;
    abTree_print(t);
    print_enable = false;

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
#include "tree.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tests_mocks.h"

extern "C"{
    abNode_t* abTree_createNodeNEl(abNode_t const * parent, abElement_t const new_el[], size_t el_len, abNode_t const * const new_child[], bool isLeaf);
}

TEST_GROUP(insertTest)
{
    void setup()
    {
        mock().clear();
        mock().disable();
    }
};

TEST(insertTest, insert_2_4)
{
    abTree_t* t = abTree_create(2, 4, NULL);
    for(int i = 100; i <= 2500; i += 200){
        abTree_insert(t, i, (void*)((intptr_t)i));
    }

    for(int i = 200; i <= 2600; i += 200){
        abTree_insert(t, i, (void*)((intptr_t)i));
    }
    
    abTree_print(t);
    abTree_destroy(t);
}

TEST(insertTest, insert_4_8)
{
    abTree_t* t = abTree_create(4, 8, NULL);
    for(int i = 100; i <= 4000; i += 100)
        abTree_insert(t, i, (void*)((intptr_t)i));

    abTree_print(t);
    abTree_destroy(t);
}

TEST(insertTest, insert_null)
{
    abTree_t* t = abTree_create(2, 4, NULL);
    abTree_insert(t, 5, NULL);
    abTree_insert(t, 6, NULL);
    abTree_insert(t, 7, NULL);
    abTree_insert(t, 8, NULL);
    abTree_insert(t, 9, NULL);

    abElement_t* el  = t->root->el;
    abNode_t* child0 = t->root->child[0];
    abNode_t* child1 = t->root->child[1];

    t->root->el = NULL;
    t->root->child[0] = NULL;
    t->root->child[1] = NULL;
    
    abTree_print(t);

    t->root->el = el;
    t->root->child[0] = child0;
    t->root->child[1] = child1;

    abTree_destroy(t);
}


TEST(insertTest, insert_allocErr)
{
   // abTree_createNodeNEl(NULL, )
}

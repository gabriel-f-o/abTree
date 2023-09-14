#include "tree.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tests_mocks.h"

extern bool malloc_enable;

extern "C"{
    abNode_t* abTree_createNode1El(abNode_t const * parent, int32_t key, void const * data, abNode_t const * left_child, abNode_t const * right_child, bool isLeaf);
    abNode_t* abTree_createNodeNEl(abNode_t const * parent, abElement_t const new_el[], size_t el_len, abNode_t const * const new_child[], bool isLeaf);
    void abTree_printNode(abNode_t const * node, uint32_t level, uint16_t b);
}

TEST_GROUP(insertTest)
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

TEST(insertTest, prints){
    abElement_t el[2] = { { 5, NULL }, { 9, NULL } }; 

    abNode_t n = { NULL, NULL, el, 2, true };
    abNode_t* child[3] = { &n, &n, &n };

    abNode_t node = { 
        child,
        NULL,
        el,
        2,
        false,
    };

    abTree_printNode(NULL, 0, 0);
    abTree_printNode(&node, 1, 1);
    abTree_printNode(&node, 1, 1);
    abTree_printNode(&node, 1, 10);
}

TEST(insertTest, insert_alloc)
{
    abElement_t el[2] = { { 5, NULL }, { 9, NULL } }; 
    abNode_t n = { NULL, NULL, el, 2, true };
    abNode_t* child[3] = { &n, &n, &n };

    malloc_enable = false;
    mock().enable();

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue((void*)0x0ULL);
    mock().expectOneCall("mock_malloc").withParameter("size", 2 * sizeof(abElement_t)).andReturnValue((void*)0x0ULL);
    mock().expectOneCall("mock_malloc").withParameter("size", 3 * sizeof(abNode_t*)).andReturnValue((void*)0x0ULL);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);

    CHECK_EQUAL(0ULL, abTree_createNodeNEl(NULL, el, 2, &child[0], false));

    mock().checkExpectations();

    abNode_t* nd = (abNode_t*) malloc(sizeof(abNode_t));
    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 2 * sizeof(abElement_t)).andReturnValue((void*)0x0ULL);
    mock().expectOneCall("mock_malloc").withParameter("size", 3 * sizeof(abNode_t*)).andReturnValue((void*)0x0ULL);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", nd);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);

    CHECK_EQUAL(0ULL, abTree_createNodeNEl(NULL, el, 2, &child[0], false));

    mock().checkExpectations();

    abElement_t* e = (abElement_t*) malloc(2 * sizeof(abElement_t));
    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 2 * sizeof(abElement_t)).andReturnValue(e);
    mock().expectOneCall("mock_malloc").withParameter("size", 3 * sizeof(abNode_t*)).andReturnValue((void*)0x0ULL);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", nd);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", e);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);

    CHECK_EQUAL(0ULL, abTree_createNodeNEl(NULL, el, 2, &child[0], false));

    mock().checkExpectations();

    abNode_t** c = (abNode_t**) malloc(3 * sizeof(abNode_t*));
    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 2 * sizeof(abElement_t)).andReturnValue(e);
    mock().expectOneCall("mock_malloc").withParameter("size", 3 * sizeof(abNode_t*)).andReturnValue(c);

    CHECK_TRUE(NULL != abTree_createNodeNEl(NULL, el, 2, &child[0], false));
    CHECK_EQUAL(0ULL, nd->parent);
    CHECK_EQUAL(2, nd->keyNum);
    CHECK_EQUAL(false, nd->isLeaf);
    CHECK_EQUAL(e, nd->el);
    CHECK_EQUAL(c, nd->child);
    CHECK_EQUAL(child[0], nd->child[0]);
    CHECK_EQUAL(child[1], nd->child[1]);
    CHECK_EQUAL(child[2], nd->child[2]);

    mock().checkExpectations();

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 2 * sizeof(abElement_t)).andReturnValue(e);
    mock().expectOneCall("mock_malloc").withParameter("size", 3 * sizeof(abNode_t*)).andReturnValue(c);

    memset(c, 0, 3 * sizeof(abNode_t*));
    CHECK_TRUE(NULL != abTree_createNodeNEl(NULL, el, 2, NULL, false));
    CHECK_EQUAL(0ULL, nd->parent);
    CHECK_EQUAL(2, nd->keyNum);
    CHECK_EQUAL(false, nd->isLeaf);
    CHECK_EQUAL(e, nd->el);
    CHECK_EQUAL(c, nd->child);
    CHECK_EQUAL(0ULL, nd->child[0]);
    CHECK_EQUAL(0ULL, nd->child[1]);
    CHECK_EQUAL(0ULL, nd->child[2]);
    mock().checkExpectations();
    mock().clear();

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 1 * sizeof(abElement_t)).andReturnValue(e);
    mock().expectOneCall("mock_malloc").withParameter("size", 2 * sizeof(abNode_t*)).andReturnValue(c);

    CHECK_TRUE(NULL != abTree_createNode1El(NULL, 77, (void*)5ULL, child[0], child[1], false));
    CHECK_EQUAL(0ULL, nd->parent);
    CHECK_EQUAL(1, nd->keyNum);
    CHECK_EQUAL(false, nd->isLeaf);
    CHECK_EQUAL(77, nd->el->key);
    CHECK_EQUAL(5ULL, (uintptr_t)nd->el->data);
    CHECK_EQUAL(c, nd->child);
    CHECK_EQUAL(child[0], nd->child[0]);
    CHECK_EQUAL(child[1], nd->child[1]);
    mock().checkExpectations();

    free(e);
    free(c);
    free(nd);
    malloc_enable = true;
}

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
    abNode_t* abTree_createNode1El(abNode_t const * parent, int32_t key, void const * data, abNode_t const * left_child, abNode_t const * right_child, bool isLeaf);
    abNode_t* abTree_createNodeNEl(abNode_t const * parent, abElement_t const new_el[], size_t el_len, abNode_t const * const new_child[], bool isLeaf);
    void abTree_printNode(abNode_t const * node, uint32_t level, uint16_t b);
    void abTree_freeTree(abNode_t* node);
    int32_t abTree_insertEl(abNode_t* node, int32_t key, void const * data, abNode_t const * new_child);
    abNode_t* abTree_splitNode(abNode_t* node);
    abNode_t* abTree_searchNode(abNode_t* node, int32_t key, uint16_t* pos, abNode_t** insert_node);
}

/**********************************************************
 * PRIVATE FUNCTION
 *********************************************************/

static void abConflict_cb(abNode_t* node, int32_t key, void* data){
    CHECK_EQUAL(100, key);
    CHECK_EQUAL(100ULL, (intptr_t)data);
}

/**********************************************************
 * PUBLIC FUNCTIONS
 *********************************************************/

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

TEST(insertTest, insert_2_3)
{
    abTree_t* t = abTree_create(2, 3, NULL);
    CHECK(t != NULL);

    for(int i = 100; i <= 1500; i += 200){
        abTree_insert(t, i, (void*)((intptr_t)i));
    }

    for(int i = 1600; i >= 200; i -= 200){
        abTree_insert(t, i, (void*)((intptr_t)i));
    }

    abTree_insert(t, 401, NULL);
    abTree_insert(t, 401, NULL);
    abTree_insert(t, 402, NULL);

    CHECK_EQUAL(1, t->root->keyNum);
    CHECK_EQUAL(700, t->root->el[0].key);

    
    CHECK_EQUAL(t->root, t->root->child[0]->parent);
    CHECK_EQUAL(1, t->root->child[0]->keyNum);
    CHECK_EQUAL(401, t->root->child[0]->el[0].key);

    CHECK_EQUAL(t->root, t->root->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[1]->keyNum);
    CHECK_EQUAL(1300, t->root->child[1]->el[0].key);


    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[0]->parent);
    CHECK_EQUAL(1, t->root->child[0]->child[0]->keyNum);
    CHECK_EQUAL(300, t->root->child[0]->child[0]->el[0].key);

    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[0]->child[1]->keyNum);
    CHECK_EQUAL(500, t->root->child[0]->child[1]->el[0].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[0]->parent);
    CHECK_EQUAL(2, t->root->child[1]->child[0]->keyNum);
    CHECK_EQUAL(900, t->root->child[1]->child[0]->el[0].key);
    CHECK_EQUAL(1100, t->root->child[1]->child[0]->el[1].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[1]->child[1]->keyNum);
    CHECK_EQUAL(1500, t->root->child[1]->child[1]->el[0].key);


    CHECK_EQUAL(t->root->child[0]->child[0], t->root->child[0]->child[0]->child[0]->parent);
    CHECK_EQUAL(2, t->root->child[0]->child[0]->child[0]->keyNum);
    CHECK_EQUAL(100, t->root->child[0]->child[0]->child[0]->el[0].key);
    CHECK_EQUAL(200, t->root->child[0]->child[0]->child[0]->el[1].key);

    CHECK_EQUAL(t->root->child[0]->child[0], t->root->child[0]->child[0]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[0]->child[0]->child[1]->keyNum);
    CHECK_EQUAL(400, t->root->child[0]->child[0]->child[1]->el[0].key);

    CHECK_EQUAL(t->root->child[0]->child[1], t->root->child[0]->child[1]->child[0]->parent);
    CHECK_EQUAL(1, t->root->child[0]->child[1]->child[0]->keyNum);
    CHECK_EQUAL(402, t->root->child[0]->child[1]->child[0]->el[0].key);

    CHECK_EQUAL(t->root->child[0]->child[1], t->root->child[0]->child[1]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[0]->child[1]->child[1]->keyNum);
    CHECK_EQUAL(600, t->root->child[0]->child[1]->child[1]->el[0].key);

    CHECK_EQUAL(t->root->child[1]->child[0], t->root->child[1]->child[0]->child[0]->parent);
    CHECK_EQUAL(1, t->root->child[1]->child[0]->child[0]->keyNum);
    CHECK_EQUAL(800, t->root->child[1]->child[0]->child[0]->el[0].key);

    CHECK_EQUAL(t->root->child[1]->child[0], t->root->child[1]->child[0]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[1]->child[0]->child[1]->keyNum);
    CHECK_EQUAL(1000, t->root->child[1]->child[0]->child[1]->el[0].key);

    CHECK_EQUAL(t->root->child[1]->child[0], t->root->child[1]->child[0]->child[2]->parent);
    CHECK_EQUAL(1, t->root->child[1]->child[0]->child[2]->keyNum);
    CHECK_EQUAL(1200, t->root->child[1]->child[0]->child[2]->el[0].key);
    
    CHECK_EQUAL(t->root->child[1]->child[1], t->root->child[1]->child[1]->child[0]->parent);
    CHECK_EQUAL(1, t->root->child[1]->child[1]->child[0]->keyNum);
    CHECK_EQUAL(1400, t->root->child[1]->child[1]->child[0]->el[0].key);

    CHECK_EQUAL(t->root->child[1]->child[1], t->root->child[1]->child[1]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[1]->child[1]->child[1]->keyNum);
    CHECK_EQUAL(1600, t->root->child[1]->child[1]->child[1]->el[0].key);

    abTree_destroy(t);
}

TEST(insertTest, insert_2_4)
{
    CHECK_EQUAL(0ULL, (intptr_t)abTree_create(1, 4, NULL));
    CHECK_EQUAL(0ULL, (intptr_t)abTree_create(4, 6, NULL));
    CHECK_EQUAL(0ULL, (intptr_t)abTree_create(2, 2, NULL));

    abTree_destroy(abTree_create(2, 5, NULL));
    
    abTree_t* t = abTree_create(2, 4, abConflict_cb);
    for(int i = 100; i <= 2500; i += 200){
        abTree_insert(t, i, (void*)((intptr_t)i));
    }

    for(int i = 200; i <= 2600; i += 200){
        abTree_insert(t, i, (void*)((intptr_t)i));
    }

    abTree_insert(t, 100, (void*)((intptr_t)50ULL));

    CHECK_EQUAL(500LL, (intptr_t)abTree_search(t, 500));
    CHECK_EQUAL(0ULL, (intptr_t)abTree_search(t, 69));
    CHECK_EQUAL(0, (intptr_t)abTree_searchNode(t->root, 50, NULL, NULL));
    CHECK(0 != (intptr_t)abTree_searchNode(t->root, 500, NULL, NULL));
    CHECK(0 != (intptr_t)abTree_searchNode(t->root, 100, NULL, NULL));
    

    CHECK_EQUAL(2, t->root->keyNum);
    CHECK_EQUAL(700, t->root->el[0].key);
    CHECK_EQUAL(1500, t->root->el[1].key);

    CHECK_EQUAL(t->root, t->root->child[0]->parent);
    CHECK_EQUAL(1, t->root->child[0]->keyNum);
    CHECK_EQUAL(300, t->root->child[0]->el[0].key);

    CHECK_EQUAL(t->root, t->root->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[1]->keyNum);
    CHECK_EQUAL(1100, t->root->child[1]->el[0].key);
    
    CHECK_EQUAL(t->root, t->root->child[2]->parent);
    CHECK_EQUAL(3, t->root->child[2]->keyNum);
    CHECK_EQUAL(1900, t->root->child[2]->el[0].key);
    CHECK_EQUAL(2100, t->root->child[2]->el[1].key);
    CHECK_EQUAL(2300, t->root->child[2]->el[2].key);

    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[0]->parent);
    CHECK_EQUAL(2, t->root->child[0]->child[0]->keyNum);
    CHECK_EQUAL(100, t->root->child[0]->child[0]->el[0].key);
    CHECK_EQUAL(200, t->root->child[0]->child[0]->el[1].key);

    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[1]->parent);
    CHECK_EQUAL(3, t->root->child[0]->child[1]->keyNum);
    CHECK_EQUAL(400, t->root->child[0]->child[1]->el[0].key);
    CHECK_EQUAL(500, t->root->child[0]->child[1]->el[1].key);
    CHECK_EQUAL(600, t->root->child[0]->child[1]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[0]->parent);
    CHECK_EQUAL(3, t->root->child[1]->child[0]->keyNum);
    CHECK_EQUAL(800, t->root->child[1]->child[0]->el[0].key);
    CHECK_EQUAL(900, t->root->child[1]->child[0]->el[1].key);
    CHECK_EQUAL(1000, t->root->child[1]->child[0]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[0]->parent);
    CHECK_EQUAL(3, t->root->child[1]->child[0]->keyNum);
    CHECK_EQUAL(800, t->root->child[1]->child[0]->el[0].key);
    CHECK_EQUAL(900, t->root->child[1]->child[0]->el[1].key);
    CHECK_EQUAL(1000, t->root->child[1]->child[0]->el[2].key);

    CHECK_EQUAL(t->root->child[2], t->root->child[2]->child[0]->parent);
    CHECK_EQUAL(3, t->root->child[2]->child[0]->keyNum);
    CHECK_EQUAL(1600, t->root->child[2]->child[0]->el[0].key);
    CHECK_EQUAL(1700, t->root->child[2]->child[0]->el[1].key);
    CHECK_EQUAL(1800, t->root->child[2]->child[0]->el[2].key);

    CHECK_EQUAL(t->root->child[2], t->root->child[2]->child[1]->parent);
    CHECK_EQUAL(1, t->root->child[2]->child[1]->keyNum);
    CHECK_EQUAL(2000, t->root->child[2]->child[1]->el[0].key);

    CHECK_EQUAL(t->root->child[2], t->root->child[2]->child[2]->parent);
    CHECK_EQUAL(1, t->root->child[2]->child[2]->keyNum);
    CHECK_EQUAL(2200, t->root->child[2]->child[2]->el[0].key);

    CHECK_EQUAL(t->root->child[2], t->root->child[2]->child[3]->parent);
    CHECK_EQUAL(3, t->root->child[2]->child[3]->keyNum);
    CHECK_EQUAL(2400, t->root->child[2]->child[3]->el[0].key);
    CHECK_EQUAL(2500, t->root->child[2]->child[3]->el[1].key);
    CHECK_EQUAL(2600, t->root->child[2]->child[3]->el[2].key);


    abTree_print(t);
    abTree_destroy(t);
}

TEST(insertTest, insert_4_7)
{
    abTree_t* t = abTree_create(4, 7, NULL);
    for(int i = 100; i <= 3900; i += 200)
        abTree_insert(t, i, (void*)((intptr_t)i));

    for(int i = 200; i <= 4000; i += 200)
        abTree_insert(t, i, (void*)((intptr_t)i));
    

    CHECK_EQUAL(1, t->root->keyNum);
    CHECK_EQUAL(1900, t->root->el[0].key);

    CHECK_EQUAL(t->root, t->root->child[0]->parent);
    CHECK_EQUAL(3, t->root->child[0]->keyNum);
    CHECK_EQUAL(700, t->root->child[0]->el[0].key);
    CHECK_EQUAL(1100, t->root->child[0]->el[1].key);
    CHECK_EQUAL(1500, t->root->child[0]->el[2].key);

    CHECK_EQUAL(t->root, t->root->child[1]->parent);
    CHECK_EQUAL(4, t->root->child[1]->keyNum);
    CHECK_EQUAL(2300, t->root->child[1]->el[0].key);
    CHECK_EQUAL(2700, t->root->child[1]->el[1].key);
    CHECK_EQUAL(3100, t->root->child[1]->el[2].key);
    CHECK_EQUAL(3500, t->root->child[1]->el[3].key);


    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[0]->parent);
    CHECK_EQUAL(6, t->root->child[0]->child[0]->keyNum);
    CHECK_EQUAL(100, t->root->child[0]->child[0]->el[0].key);
    CHECK_EQUAL(200, t->root->child[0]->child[0]->el[1].key);
    CHECK_EQUAL(300, t->root->child[0]->child[0]->el[2].key);
    CHECK_EQUAL(400, t->root->child[0]->child[0]->el[3].key);
    CHECK_EQUAL(500, t->root->child[0]->child[0]->el[4].key);
    CHECK_EQUAL(600, t->root->child[0]->child[0]->el[5].key);
    
    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[1]->parent);
    CHECK_EQUAL(3, t->root->child[0]->child[1]->keyNum);
    CHECK_EQUAL( 800, t->root->child[0]->child[1]->el[0].key);
    CHECK_EQUAL( 900, t->root->child[0]->child[1]->el[1].key);
    CHECK_EQUAL(1000, t->root->child[0]->child[1]->el[2].key);

    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[2]->parent);
    CHECK_EQUAL(3, t->root->child[0]->child[2]->keyNum);
    CHECK_EQUAL(1200, t->root->child[0]->child[2]->el[0].key);
    CHECK_EQUAL(1300, t->root->child[0]->child[2]->el[1].key);
    CHECK_EQUAL(1400, t->root->child[0]->child[2]->el[2].key);

    CHECK_EQUAL(t->root->child[0], t->root->child[0]->child[3]->parent);
    CHECK_EQUAL(3, t->root->child[0]->child[3]->keyNum);
    CHECK_EQUAL(1600, t->root->child[0]->child[3]->el[0].key);
    CHECK_EQUAL(1700, t->root->child[0]->child[3]->el[1].key);
    CHECK_EQUAL(1800, t->root->child[0]->child[3]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[0]->parent);
    CHECK_EQUAL(3, t->root->child[1]->child[0]->keyNum);
    CHECK_EQUAL(2000, t->root->child[1]->child[0]->el[0].key);
    CHECK_EQUAL(2100, t->root->child[1]->child[0]->el[1].key);
    CHECK_EQUAL(2200, t->root->child[1]->child[0]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[1]->parent);
    CHECK_EQUAL(3, t->root->child[1]->child[1]->keyNum);
    CHECK_EQUAL(2400, t->root->child[1]->child[1]->el[0].key);
    CHECK_EQUAL(2500, t->root->child[1]->child[1]->el[1].key);
    CHECK_EQUAL(2600, t->root->child[1]->child[1]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[2]->parent);
    CHECK_EQUAL(3, t->root->child[1]->child[2]->keyNum);
    CHECK_EQUAL(2800, t->root->child[1]->child[2]->el[0].key);
    CHECK_EQUAL(2900, t->root->child[1]->child[2]->el[1].key);
    CHECK_EQUAL(3000, t->root->child[1]->child[2]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[3]->parent);
    CHECK_EQUAL(3, t->root->child[1]->child[3]->keyNum);
    CHECK_EQUAL(3200, t->root->child[1]->child[3]->el[0].key);
    CHECK_EQUAL(3300, t->root->child[1]->child[3]->el[1].key);
    CHECK_EQUAL(3400, t->root->child[1]->child[3]->el[2].key);

    CHECK_EQUAL(t->root->child[1], t->root->child[1]->child[4]->parent);
    CHECK_EQUAL(5, t->root->child[1]->child[4]->keyNum);
    CHECK_EQUAL(3600, t->root->child[1]->child[4]->el[0].key);
    CHECK_EQUAL(3700, t->root->child[1]->child[4]->el[1].key);
    CHECK_EQUAL(3800, t->root->child[1]->child[4]->el[2].key);
    CHECK_EQUAL(3900, t->root->child[1]->child[4]->el[3].key);
    CHECK_EQUAL(4000, t->root->child[1]->child[4]->el[4].key);

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
    abElement_t el[2] = { { NULL, 5 }, { NULL, 9 } }; 

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
    abElement_t el[2] = { { NULL, 5 }, { NULL, 9 } }; 
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

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abTree_t)).andReturnValue((void*)NULL);
    CHECK(NULL == abTree_create(2, 5, NULL));
    mock().checkExpectations();
    mock().clear();

    abElement_t sEl[3] = { { NULL, 2 }, { NULL, 5 }, { NULL, 10 } };
    abNode_t split_node = { NULL, NULL, sEl, 3, true };

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 1 * sizeof(abElement_t)).andReturnValue(e);

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue((void*)NULL);
    mock().expectOneCall("mock_malloc").withParameter("size", 1 * sizeof(abElement_t)).andReturnValue(e);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", e);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", e);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", nd);

    CHECK(NULL == abTree_splitNode(&split_node));
    mock().checkExpectations();

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue((void*)NULL);
    mock().expectOneCall("mock_malloc").withParameter("size", 1 * sizeof(abElement_t)).andReturnValue(e);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", e);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);

    mock().expectOneCall("mock_malloc").withParameter("size", sizeof(abNode_t)).andReturnValue(nd);
    mock().expectOneCall("mock_malloc").withParameter("size", 1 * sizeof(abElement_t)).andReturnValue(e);

    mock().expectOneCall("mock_free").withPointerParameter("ptr", NULL);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", e);
    mock().expectOneCall("mock_free").withPointerParameter("ptr", nd);

    CHECK(NULL == abTree_splitNode(&split_node));
    mock().checkExpectations();

    free(e);
    free(c);
    free(nd);

    malloc_enable = true;
    mock().disable();

    abTree_freeTree(NULL);
}

TEST(insertTest, insertEl)
{
    abNode_t* c1   = abTree_createNode1El(NULL,  5, (void*) 10ULL, NULL, NULL, true);
    abNode_t* c2   = abTree_createNode1El(NULL, 55, (void*)110ULL, NULL, NULL, true);
    abNode_t* nc   = abTree_createNode1El(NULL, 15, (void*)30ULL, NULL, NULL, true);
    abNode_t* nc2  = abTree_createNode1El(NULL, 20, (void*)40ULL, NULL, NULL, true);
    abNode_t* node = abTree_createNode1El(NULL, 10, (void*)20ULL, c1, c2, false);

    CHECK_EQUAL(1, abTree_insertEl(node, 20, (void*)40ULL, nc));
    CHECK_EQUAL(1, abTree_insertEl(node, 18, (void*)36ULL, nc2));

    CHECK_EQUAL(3, node->keyNum);
    CHECK_EQUAL(10, node->el[0].key);
    CHECK_EQUAL(18, node->el[1].key);
    CHECK_EQUAL(20, node->el[2].key);

    CHECK_EQUAL(20LL, (intptr_t)node->el[0].data);
    CHECK_EQUAL(36LL, (intptr_t)node->el[1].data);
    CHECK_EQUAL(40LL, (intptr_t)node->el[2].data);

    CHECK_EQUAL(1, node->child[0]->keyNum);
    CHECK_EQUAL(5, node->child[0]->el[0].key);
    CHECK_EQUAL(10LL, (intptr_t)node->child[0]->el[0].data);

    CHECK_EQUAL(1, node->child[1]->keyNum);
    CHECK_EQUAL(15, node->child[1]->el[0].key);
    CHECK_EQUAL(30LL, (intptr_t)node->child[1]->el[0].data);

    CHECK_EQUAL(1, node->child[2]->keyNum);
    CHECK_EQUAL(20, node->child[2]->el[0].key);
    CHECK_EQUAL(40LL, (intptr_t)node->child[2]->el[0].data);

    CHECK_EQUAL(1, node->child[3]->keyNum);
    CHECK_EQUAL(55, node->child[3]->el[0].key);
    CHECK_EQUAL(110LL, (intptr_t)node->child[3]->el[0].data);

    abTree_insertEl(node, 60, NULL, NULL);
    node->keyNum--;

    abTree_freeTree(node);
}
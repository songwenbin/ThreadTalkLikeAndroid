#include "CuTest.h"
#include "Message.h"
#include "MessageQueue.h"
#include "MessagePool.h"

void TestSimple1(CuTest *tc) {
	printf("sssssssssssssssssssss\n");
	/*
	CuAssertIntEqual(tc, 2, count);
	CuAssertPtrEquals(tc, NULL, result);
	CuAssertStrEquals(tc, expected, actual);
	 */
}

void TestMessagePoolCreate(CuTest *tc) {
	struct MessagePool *pool = CreateMessagePool();
	int i = 0;
	while (pool->head != NULL) {
		pool->head = pool->head->next;
		i ++;
	}	

	CuAssertIntEquals(tc, pool->count, i);
}

void TestMessagePoolGetTwoMessage(CuTest *tc) {
	struct MessagePool *pool = CreateMessagePool();

	struct Message *msg = GetMessage(pool);
	struct Message *msg1 = GetMessage(pool);
	int i = 0;
	while (pool->head != NULL) {
		pool->head = pool->head->next;
		i ++;
	}	

	CuAssertIntEquals(tc, pool->count, i);
}

void TestMessagePoolGetAllMessage(CuTest *tc) {
	struct MessagePool *pool = CreateMessagePool();

	int i = 0;
	for (i = 0; i < 1024; i ++) {
		struct Message *msg = GetMessage(pool);
	}
	CuAssertIntEquals(tc, 0, pool->count);
	CuAssertPtrEquals(tc, NULL, pool->head);

	struct Message *msg = GetMessage(pool);
	CuAssertPtrEquals(tc, NULL, msg);
}

void TestMessagePoolPutMessage(CuTest *tc) {
	struct MessagePool *pool = CreateMessagePool();
	struct Message *m = NewMessage();
	PutMessage(pool, m);
	CuAssertIntEquals(tc, 1025, pool->count);

	int i = 0;
	for (i = 0; i < 1024; i ++) {
		struct Message *msg = GetMessage(pool);
	}
	CuAssertIntEquals(tc, 1, pool->count);
	CuAssertPtrNotNull(tc, pool->head);

	struct Message *msg = GetMessage(pool);
	CuAssertIntEquals(tc, 0, pool->count);
	CuAssertPtrEquals(tc, NULL, pool->head);

	struct Message *m1 = NewMessage();
	PutMessage(pool, m1);
	CuAssertIntEquals(tc, 1, pool->count);
	CuAssertPtrNotNull(tc, pool->head);

	struct Message *m2 = NewMessage();
	PutMessage(pool, m2);
	CuAssertIntEquals(tc, 2, pool->count);
	CuAssertPtrNotNull(tc, pool->head);
}

CuSuite* MessagePoolSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSimple1);
	SUITE_ADD_TEST(suite, TestMessagePoolCreate);
	SUITE_ADD_TEST(suite, TestMessagePoolGetTwoMessage);
	SUITE_ADD_TEST(suite, TestMessagePoolPutMessage);
	return suite;
}

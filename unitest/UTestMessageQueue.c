#include "CuTest.h"
#include "Message.h"
#include "MessageQueue.h"

void TestSimple(CuTest *tc) {
	printf("sssssssssssssssssssss\n");
	/*
	CuAssertIntEqual(tc, 2, count);
	CuAssertPtrEquals(tc, NULL, result);
	CuAssertStrEquals(tc, expected, actual);
	 */
}

void TestQueueGenerate(CuTest *tc) {
	struct MessageQueue *queue = NewMessageQueue();
	CuAssertPtrNotNull(tc, queue);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueHead);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueTail);
        DeleteMessageQueue(queue);	
}

void TestDeNullQueue(CuTest *tc) {
	struct MessageQueue *queue = NewMessageQueue();
	struct Message *m  = DequeueMessage(queue);
	CuAssertPtrEquals(tc, NULL, m);
        DeleteMessageQueue(queue);
}


void TestMessageEnDeBound(CuTest *tc) {
	struct MessageQueue *queue = NewMessageQueue();
	CuAssertPtrNotNull(tc, queue);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueHead);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueTail);

	struct Message *enm = NewMessage(); 
	EnqueueMessage(queue, enm);
	CuAssertPtrNotNull(tc, queue->msgQueueHead);
	CuAssertPtrNotNull(tc, queue->msgQueueTail);

	struct Message *dem = DequeueMessage(queue);	
	CuAssertPtrEquals(tc, NULL, queue->msgQueueHead);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueTail);

        DeleteMessageQueue(queue);
	DeleteMessage(dem);
}

void TestMessageEnDe(CuTest *tc) {
	struct MessageQueue *queue = NewMessageQueue();
	CuAssertPtrNotNull(tc, queue);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueHead);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueTail);

	struct Message *enm1 = NewMessage(); 
	enm1->what = 0x01;
	EnqueueMessage(queue, enm1);
	CuAssertPtrNotNull(tc, queue->msgQueueHead);
	CuAssertPtrNotNull(tc, queue->msgQueueTail);

	struct Message *enm2 = NewMessage(); 
	enm2->what = 0x02;
	EnqueueMessage(queue, enm2);
	CuAssertPtrNotNull(tc, queue->msgQueueHead);
	CuAssertPtrNotNull(tc, queue->msgQueueTail);

	struct Message *dem1 = DequeueMessage(queue);	
	CuAssertIntEquals(tc, 1, dem1->what);
	CuAssertPtrNotNull(tc, queue->msgQueueHead);
	CuAssertPtrNotNull(tc, queue->msgQueueTail);

	struct Message *dem2 = DequeueMessage(queue);	
	CuAssertIntEquals(tc, 2, dem2->what);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueHead);
	CuAssertPtrEquals(tc, NULL, queue->msgQueueTail);

        DeleteMessageQueue(queue);
	DeleteMessage(dem1);
	DeleteMessage(enm2);
}

CuSuite* MessageQueueSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestMessageEnDeBound);
	SUITE_ADD_TEST(suite, TestQueueGenerate);
	SUITE_ADD_TEST(suite, TestDeNullQueue);
	SUITE_ADD_TEST(suite, TestMessageEnDe);
	return suite;
}

#include "CuTest.h"
#include "Handler.h"

extern __thread struct Looper *gLoop;

void TestLooperThreadStorage(CuTest *tc) {
	CuAssertPtrEquals(tc, NULL, gLoop);

	struct Handler *handler1 = NewHandler();
	struct Looper *loop1 = handler1->loop;
	CuAssertPtrNotNull(tc, gLoop);

	struct Handler *handler2 = NewHandler();
	struct Looper *loop2 = handler2->loop;
	CuAssertPtrEquals(tc, loop1, loop2);

	DelHandler(handler1);
	DelHandler(handler2);
}

void TestHandler(CuTest *tc) {
	CuAssertPtrNotNull(tc, gLoop);
	struct Handler *handler = NewHandler();
	struct Message *m = ObtainMessage(handler);
	SendMessage(handler, m);
	uint64_t s;
	read(handler->eventFd, &s, sizeof(uint64_t));
	CuAssertIntEquals(tc, 1, s);
}

/*
int HandleMessageImpl(struct Message *m) {
	switch(m->what) {
		case 0x01:
			return 1;
			break;
		case 0x02:
			return 2;
			break;
	}
}

void TestHandleMessage(CuTest *tc) {
	CuAssertPtrNotNull(tc, gLoop);
	// main
	struct Handler *handler = NewHandler();
	handler->HandleMessage = HandleMessageImpl;

	// sub
	struct Message *m = ObtainMessage(handler);
	m->what = 0x01;
	CuAssertPtrEquals(tc, handler, m->target);

	int ret = ((struct Handler *)m->target)->HandleMessage(m);
	CuAssertIntEquals(tc, 1, ret);
}
*/


CuSuite* HandlerSuite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestLooperThreadStorage);
	//SUITE_ADD_TEST(suite, TestHandler);
	//SUITE_ADD_TEST(suite, TestHandleMessage);
	return suite;
}

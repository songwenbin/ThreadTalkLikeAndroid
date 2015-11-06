#include "CuTest.h"
#include "Handler.h"
#include "Looper.h"
#include "CalTime.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <pthread.h>

#define THREAD_NUM 50
#define MESSAGE_NUM 0xFFFF

static int result = 0;
static int HandleMessageImpl(struct Message *m) {
	switch(m->what) {
		case 0x01:
			result = 2;
	}
}

void TestLooper(CuTest *tc) {
	struct Handler *handler = NewHandler();

	struct Message *m1 = ObtainMessage(handler);
	m1->what = 0x01;
	SendMessage(handler, m1);

	struct Message *m2 = ObtainMessage(handler);
	m2->what = 0x02;
	SendMessage(handler, m2);

	struct Message *m3 = ObtainMessage(handler);
	m3->what = 0x03;
	SendMessage(handler, m3);

	int ret = DispatchMessage(handler->loop);
	CuAssertIntEquals(tc, 0, ret);
}

void *RegisterHandlerSub(void *args) {
	int64_t u = 2;
	int fd = *((int *)args);
	write(fd, &u, sizeof(int64_t));
}

void TestRegisterHandler(CuTest *tc) {
	struct Handler *handler = NewHandler();
	CuAssertPtrNotNull(tc, handler);
	int fd = handler->eventFd;

	pthread_t t;
	pthread_create(&t, NULL, RegisterHandlerSub, &(handler->eventFd));
	
	int num = Looper(handler->loop);
	CuAssertIntEquals(tc, 1, num);
	CuAssertIntEquals(tc, 1, handler->loop->activeNum);
	CuAssertIntEquals(tc, fd, handler->loop->activeHandlers[0]);
	DelHandler(handler);
}

void *SendMessageThread(void *args) {
	struct Handler *handler = (struct Handler *)args;
	struct Message *m = ObtainMessage(handler);
	m->what = 0x01;
	SendMessage(handler, m);
	LoopStop(handler->loop);
}

void TestUserHandler(CuTest *tc) {
	struct Handler *handler = NewHandler();
	CuAssertPtrNotNull(tc, handler);
        handler->HandleMessage = HandleMessageImpl;

	pthread_t t;
	pthread_create(&t, NULL, SendMessageThread, handler);
	
	LoopStart(handler->loop);
	CuAssertIntEquals(tc, 2, result);
	DelHandler(handler);
}

struct TestObj {
	struct Handler *handler;
	int i;
	CuTest *context;
};

// MULTI START
static int impl[THREAD_NUM] = {0};

static int MultiHandleMessageImpl(struct Message *m) {
	int i = m->args1;
	CuAssertIntEquals((CuTest *)m->obj, impl[i]++, m->what);
	//printf("Thread [%d] : %d\n", i, m->what);
	if (m->what == (THREAD_NUM - 1)) {
		printf("Thread [%d] is finished!\n", i); 
	}
}

void *MultiHandlerSub(void *args) {
	struct TestObj *obj = (struct TestObj *)args;
	struct Handler *handler = obj->handler;
	int i = 0;
	for (i = 0; i < MESSAGE_NUM; i ++) {
		struct Message *m = ObtainMessage(handler);
		m->what = i;
		m->args1 = obj->i;
		m->obj = (void *)obj->context;
		SendMessage(handler, m);
	}
	sleep(1);
}

pthread_t t[THREAD_NUM];
void *CollectThread(void *args) {
	struct Handler *handler = (struct Handler *)args;
	int i = 0;
	for (i = 0; i < THREAD_NUM; i ++) {
		pthread_join(t[i], NULL);
	}

	LoopStop(handler->loop);
}

void TestMultiHandler(CuTest *tc) {
	struct Handler *handlers[THREAD_NUM];
	struct TestObj obj[THREAD_NUM];
	int i = 0;
	for (i = 0; i < THREAD_NUM; i ++) {
		handlers[i] = NewHandler();
		CuAssertPtrNotNull(tc, handlers[i]);
		handlers[i]->HandleMessage = MultiHandleMessageImpl;
		obj[i].context = tc;
		obj[i].handler = handlers[i];
		obj[i].i = i;
	}

	for (i = 0; i < THREAD_NUM; i ++) {
		pthread_create(&t[i], NULL, MultiHandlerSub, &obj[i]);
	}
	
	pthread_t ct;
	pthread_create(&ct, NULL, CollectThread, handlers[0]);
	
	LoopStart(handlers[0]->loop);

}
// MULTI END

// OLD TEST START
struct msgbuf {
	long mtype;
	int mtext;
};

void *OldThreadMessageSub(void *args) {
	int id = *((int *)args);
		int i;
	printf("old\n");
	StartTime();
	for (i = 0; i < 0xFFFFFF; i ++) {
		struct msgbuf *msg = (struct msgbuf *)malloc(sizeof(long) +sizeof(int));
		msg->mtype = 0x100;
		msg->mtext = i;
		msgsnd(id, msg, sizeof(int), 0);
	}


}

void TestOldThreadMessage(CuTest *tc) {
	int id = msgget(IPC_PRIVATE, 0660|IPC_CREAT);
	static int ret1 = 0;
	
	pthread_t t1;
	pthread_create(&t1, NULL, OldThreadMessageSub, &id);
	struct msgbuf buf;
	while (1) {
		msgrcv(id, &buf, sizeof(int), 0x100, 0);
		//CuAssertIntEquals(tc, ret1++, buf.mtext);
		//printf("%d\n", buf.mtext);
		if (buf.mtext == 0xFFFFFE) {
			break;
		}
	}
	EndTime();
	PassTime();
}
// OLD TEST END

int SingleHandleMessageImpl(struct Message *m) {
	static int k = 0;
	//printf("%d %d\n", i, m->what);
	CuAssertIntEquals((CuTest *)m->obj, k++, m->what);
}

void *SingleHandlerSub(void *args) {
	struct TestObj *obj = (struct TestObj *)args;
	struct Handler *handler = obj->handler;
	int i = 0;
	StartTime();
	for (i = 0; i < 0xFFFFFF; i ++) {
		struct Message *m = ObtainMessage(handler);
		m->what = i;
		m->obj = (void *)obj->context;
		SendMessage(handler, m);
	}
	LoopStop(handler->loop);
}

void TestSingleHandler(CuTest *tc) {
	struct Handler *handler = NewHandler();
	CuAssertPtrNotNull(tc, handler);
        handler->HandleMessage = SingleHandleMessageImpl;
	struct TestObj obj;
	obj.context = tc;
	obj.handler = handler;

	pthread_t t;
	pthread_create(&t, NULL, SingleHandlerSub, &obj);
	
	LoopStart(handler->loop);
	EndTime();
	PassTime();
}


CuSuite* LooperSuite()
{
	CuSuite* suite = CuSuiteNew();
	//SUITE_ADD_TEST(suite, TestLooper);
	//SUITE_ADD_TEST(suite, TestRegisterHandler);
	//SUITE_ADD_TEST(suite, TestUserHandler);
	//SUITE_ADD_TEST(suite, TestUserHandler);
	//SUITE_ADD_TEST(suite, TestOldThreadMessage);
	SUITE_ADD_TEST(suite, TestMultiHandler);
	SUITE_ADD_TEST(suite, TestSingleHandler);
	return suite;
}

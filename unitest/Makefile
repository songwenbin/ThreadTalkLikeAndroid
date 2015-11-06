OBJS=../Message.c
OBJS+=../MessageQueue.c
OBJS+=../MessagePool.c
OBJS+=../Handler.c
OBJS+=../Looper.c

TESTOBJS=AllTests.c
TESTOBJS+=CuTest.c
TESTOBJS+=UTestMessageQueue.c 
TESTOBJS+=UTestMessagePool.c 
TESTOBJS+=UTestHandler.c 
TESTOBJS+=UTestLooper.c 

UniTest: $(OBJS)
	gcc -g -o $@ $(TESTOBJS) $(OBJS) -I../ -lpthread

clean:
	rm -rf UniTest

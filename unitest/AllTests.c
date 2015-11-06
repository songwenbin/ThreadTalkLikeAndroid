#include <stdio.h>

#include "CuTest.h"

CuSuite* MessageQueueSuite();
CuSuite* MessagePoolSuite();
CuSuite* HandlerSuite();
CuSuite* LooperSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	/*
	CuSuiteAddSuite(suite, MessageQueueSuite());
	CuSuiteAddSuite(suite, HandlerSuite());
	*/
	CuSuiteAddSuite(suite, LooperSuite());
	//CuSuiteAddSuite(suite, MessagePoolSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
}

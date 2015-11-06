#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct timeval start_time;
static struct timeval end_time;

void StartTime() {
	gettimeofday(&start_time, NULL);
}

void EndTime() {
	gettimeofday(&end_time, NULL);
}

void PassTime() {
	int s, ms, us;

	s = end_time.tv_sec - start_time.tv_sec;

	if (end_time.tv_usec/1000 >= start_time.tv_usec/1000) {
		ms = (end_time.tv_usec/1000 - start_time.tv_usec/1000);
	} else {
		ms = (end_time.tv_usec/1000 + 1000 - start_time.tv_usec/1000);
		s -= 1;
	}

	if (end_time.tv_usec%1000 >= start_time.tv_usec%1000) {
		us = end_time.tv_usec%1000 - start_time.tv_usec%1000;
	} else {
		us = end_time.tv_usec%1000 + 1000 - start_time.tv_usec%1000;
		ms -= 1;
	}

	printf("%d s : %d ms : %d us\n", s, ms, us);

}

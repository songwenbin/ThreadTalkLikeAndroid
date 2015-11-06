#include "Looper.h"

#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Looper *CreateLooper() {
	struct Looper *loop = (struct Looper *)malloc(sizeof(struct Looper));
	if (loop == NULL) {
		fprintf(stderr, "loop allocate error\n");
		return NULL;
	}

	loop->handleNum = 0;
	loop->activeNum = 0;
	loop->efd = epoll_create(HANDLER_NUM);
	loop->stopFd = eventfd(0, EFD_NONBLOCK);

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = loop->stopFd;
	int ret = epoll_ctl(loop->efd, EPOLL_CTL_ADD, loop->stopFd, &ev);
	if (ret == -1) {
		fprintf(stderr, "register handler error\n");
	}

	memset(loop->activeHandlers, 0, HANDLER_NUM*HANDLER_NUM);

	return loop;
}

void DeleteLooper(struct Looper *loop) {
	close(loop->efd);
	close(loop->stopFd);
	free(loop);
}

void RegisterHandler(struct Looper *loop, struct Handler *handler) {
	int ret = 0;

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = handler->eventFd;
	ret = epoll_ctl(loop->efd, EPOLL_CTL_ADD, handler->eventFd, &ev);

	if (ret == -1) {
		fprintf(stderr, "register handler error\n");
		return;
	}

	loop->handlers[loop->handleNum++] = handler;
}

void PushActiveHandler(struct Looper *loop, int fd) {
	int n = 0;
	for (n = 0; n < loop->handleNum; ++n) {
		if (fd == loop->handlers[n]->eventFd) {
			loop->activeHandlers[loop->activeNum++] = fd;
		}
	}	
}

int Looper(struct Looper *loop) {
	int nfds = 0;
	int n = 0;

	struct epoll_event events[HANDLER_NUM*256];
	nfds = epoll_wait(loop->efd, events, HANDLER_NUM*256, -1);
	if (nfds == -1) {
		return nfds;
	}

	for (n = 0; n < nfds; ++n) {
		int64_t u;
		read(events[n].data.fd, &u, sizeof(int64_t));

		if (u == STOP_LOOP && loop->stopFd == events[n].data.fd) {
			printf("exit\n");
			return 0;
		}
		PushActiveHandler(loop, events[n].data.fd);
	}

	return nfds;
}

int DispatchMessage(struct Looper *loop) {
	int n = 0;
	int k = 0;

	for (n = 0; n < loop->activeNum; ++n) {
		int fd = loop->activeHandlers[n];	
		for (k = 0; k < loop->handleNum; ++k) {
			if (fd == loop->handlers[k]->eventFd) {
				HandleMessageQueue(loop->handlers[k]);
			}
		}
	}

	return 0;
}

void LoopStart(struct Looper *loop) {
	int ret = 0;
	while (1) {
		ret = Looper(loop);
		if (ret < 0) {
			continue;
		} else if (ret == 0) {
			break;
		} else {
			DispatchMessage(loop);
		}
	}
}

void LoopStop(struct Looper *loop) {
	uint64_t stop = STOP_LOOP;
	printf("send exit\n");
	write(loop->stopFd, &stop, sizeof(uint64_t));
}

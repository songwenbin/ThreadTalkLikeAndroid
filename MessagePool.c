#include "MessagePool.h"

struct MessagePool *CreateMessagePool() {
	struct MessagePool *pool = (struct MessagePool *)malloc(sizeof(struct MessagePool));
	if (pool == NULL) {
		printf("allocated pool error\n");
		return NULL;
	}

	pool->count = 0;

	int i = 0;
	for (i = 0; i < MESSAGE_INIT_NUM; i ++) {
		struct Message *m = NewMessage();
		PutMessage(pool, m);
	}

	pthread_mutex_init(&pool->poolMutex, NULL);

	return pool;
}


struct Message *GetMessage(struct MessagePool *pool) {
	/*
	pthread_mutex_lock(&pool->poolMutex);
	if (pool->count == 0) {
		pthread_mutex_unlock(&pool->poolMutex);
		return NULL;
	}

	struct Message *m = pool->head;
	pool->head = pool->head->next;

	pool->count--;
	pthread_mutex_unlock(&pool->poolMutex);
	*/

	pthread_mutex_lock(&pool->poolMutex);
	if (pool->count == 0) {
	//if (pool->tail == NULL) {
		return NULL;
	}
	struct Message *m = pool->head;
	pool->head = pool->head->next;
	if (m == pool->tail) {
		pool->head = pool->tail = NULL;
	}
	pool->count--;
	pthread_mutex_unlock(&pool->poolMutex);

	return m;
}

void PutMessage(struct MessagePool *pool, struct Message *m) {
	/*
	pthread_mutex_lock(&pool->poolMutex);
	if (pool->count == 0) {
		pool->head = m;
	} else {
		m->next = pool->head;
		pool->head = m;
	}
	pool->count ++;
	pthread_mutex_unlock(&pool->poolMutex);
	*/

	pthread_mutex_lock(&pool->poolMutex);
	if (pool->count == 0) {
	//if (pool->head == NULL) {
		pool->head = m;
		pool->tail = m;
	} else {
		pool->tail->next = m;
		pool->tail = m;
	}
	pool->count++;
	pthread_mutex_unlock(&pool->poolMutex);
}

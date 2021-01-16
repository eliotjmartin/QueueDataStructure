/*
 * implementation for linked-list-based generic FIFO queue
 *
 * Author: Eliot Martin
 */

#include "ADTs/llistqueue.h"
#include <stdlib.h>
/* any other includes needed for the implementation */

typedef struct qnode {
	struct qnode *next;
	void *value;
} QNode;

typedef struct q_data {
    /* flesh out the instance specific data structure */
	long size;
	QNode *head;
	QNode *tail;
	void (*freeValue)(void *v);
} QData;
/* any other data structures needed */

static void purge(QData *qd, void (*freeValue)(void *element)) {
	if (freeValue != NULL) {
		QNode *q;

		for (q = qd->head; q != NULL; q = q->next)
			(freeValue)(q->value);
	}
}
static void freeList(QData *qd) {
	QNode *p, *q = NULL;
	for (p = qd->head; p != NULL; p = q) {
		q = p->next;
		free(p);
	}
}

static void q_destroy(const Queue *q) {
    /* implementation of the destroy() method */
	QData *qd = (QData *)q->self;
	purge(qd, qd->freeValue);
	freeList(qd);
	free(qd);
	free((void *)q);
}

static void q_clear(const Queue *q) {
    /* implementation of the clear() method */
	QData *qd = (QData *)q->self;
	purge(qd, qd->freeValue);
	freeList(qd);
	qd->head = qd->tail = NULL;
	qd->size = 0L;
}

static int q_enqueue(const Queue *q, void *element) {
    /* implementation of the enqueue() method */
	QData *qd = (QData *)q->self;
	int status = 0;
	QNode *n = (QNode *)malloc(sizeof(QNode));
	if (n != NULL) {
		n->value = element;
		n->next = NULL;
		status = 1;
		if ((qd->size) == 0L){
			qd->head = n;
		}
		else{
			qd->tail->next = n;
		}
		qd->tail = n;
		qd->size++;
		
	}
	return status;
}

static int q_front(const Queue *q, void **element) {
    /* implementation of the front() method */
	QData *qd = (QData *)q->self;
	int status = (qd->size > 0L);
	if (status)
		*element = qd->head->value;
	return status;
}

static int q_dequeue(const Queue *q, void **element) {
    /* implementation of the dequeue() method */
	QData *qd = (QData *)q->self;
	int status = 0;
	if ((qd->size) != 0L) {
		status = 1;
		QNode *n = qd->head;
		*element = n->value;
		if((qd->head = n->next) == NULL)
			qd->tail = NULL;
		free(n);
		qd->size--;
	}
	return status;
}

static long q_size(const Queue *q) {
    /* implementation of the size() method */
	QData *qd = (QData *)q->self;
	return qd->size;
}

static int q_isEmpty(const Queue *q) {
    /* implementation of the isEmpty() method */
	QData *qd = (QData *)q->self;
	return (qd->size == 0l);
}

static void **genArray(QData *qd) {
	void **theArray = NULL;
	if (qd->size > 0L) {
		theArray = (void **)malloc(qd->size*sizeof(void *));
		if (theArray != NULL) {
			long i = 0L;
			QNode *n;
			for (n = qd->head; n != NULL; n = n->next)
				theArray[i++] = n->value;
		}
	}
	return theArray;
}

static void **q_toArray(const Queue *q, long *len) {
    /* implementation of the toArray() method */
	QData *qd = (QData *)q->self;
	void **temp = genArray(qd);
	if (temp != NULL)
		*len = qd->size;
	return temp;
}

static const Iterator *q_itCreate(const Queue *q) {
    /* implementation of the itCreate() method */
	QData *qd = (QData *)q->self;
	const Iterator *it = NULL;
	void **temp = genArray(qd);
	if (temp != NULL) {
		it = Iterator_create(qd->size, temp);
		if (it == NULL)
			free(temp);
	}
	return it;
}

static const Queue *q_create(const Queue *q);
/* this is just declaring the signature for the create() method; it's
   implementation is provided below */

static Queue template = {
    NULL, q_create, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size,
    q_isEmpty, q_toArray, q_itCreate
};
/*
 * Helper Function
 */

static const Queue *newQueue(void (*freeValue)(void *e)) {
	Queue *q = (Queue *)malloc(sizeof(Queue));
	if (q != NULL) {
		QData *qd = (QData *)malloc(sizeof(QData));
		if (qd != NULL) {
			qd->size = 0L;
			qd->head = NULL;
			qd->tail = NULL;
			qd->freeValue = freeValue;
			*q = template;
			q->self = qd;
		}
		else{
			free(qd);
			qd = NULL;
		}
	}
	else {
		free(q);
		q = NULL;
	}
	return q;
}

static const Queue *q_create(const Queue *q) {
    /* implementation of the create() method */
	QData *qd = (QData *)q->self;
	return newQueue(qd->freeValue);
}

const Queue *LListQueue(void (*freeValue)(void *e)) {
    /* implementation of the structure-specific constructor */
	return newQueue(freeValue);
}

const Queue *Queue_create(void (*freeValue)(void *e)) {
    /* implementation of the generic constructor */
	return newQueue(freeValue);
}

/*exerciseQueue.c - Reads commands enqueue, dequeue, and print
 * from input file and performs commands on long
 * integers using a Queue structure (First in first out)
 *
 *usage: ./exerciseQueue FILE
 *
 * author: Eliot Martin
 */
#include "ADTs/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void printQueue(const Queue *q, FILE *fp) {
	const Iterator *it;
	it = q->itCreate(q);
	bool first = true;
	if(q->isEmpty(q)) {
		fprintf(fp, "Empty\n");
		return;
	}
	if (it != NULL){
		while(it->hasNext(it)) {
			long n;
			(void)it->next(it, (void **)&n);
			(first) ? fprintf(fp, "%ld", n) : fprintf(fp, " %ld", n);
			first = false;
		}
	}
	fprintf(fp, "\n");
	it->destroy(it);
}

void exerciseQueue(const Queue *q, FILE *fp) {
	char buf[BUFSIZ];
	int N, i;

	(void)fgets(buf, BUFSIZ, fp);
	(void)sscanf(buf, "%d", &N);
	for(i = 0; i < N; i++) {
		(void)fgets(buf, BUFSIZ, fp);
		if(strcmp(buf, "dequeue\n") == 0) {
			if(q->size(q) == 0L) {
				printf("QueueError\n");
			}
			else {
				long v;
				q->dequeue(q, (void **)&v);
				printf("%ld\n", v);
			}
		}
		else if(strcmp(buf, "print\n") == 0) {
			printQueue(q, stdout);
		}
		else {
			long v;
			sscanf(buf+8, "%ld", &v);
			(void) q->enqueue(q, (void *)v);
		}
	}
}

int main(int argc, char *argv[]) {
	FILE *fp;
	int exitStatus = EXIT_SUCCESS;
	const Queue *q = NULL;

	if(argc != 2) {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		goto cleanup;
	}
	if((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "cannot open file %s\n", argv[1]);
		exitStatus = EXIT_FAILURE;
		goto cleanup;
	}
	if((q = Queue_create(NULL)) == NULL) {
		fprintf(stderr, "%s: cannot create Queue ADT instance\n", argv[0]);
		exitStatus = EXIT_FAILURE;
		goto cleanup;
	}
	exerciseQueue(q, fp);
	goto cleanup;
cleanup:
	if(fp != NULL)
		fclose(fp);
	if(q != NULL)
		q->destroy(q);
	return exitStatus;
}

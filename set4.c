#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>

int sharedData;

void* total() {
	int i;
	for(i = 0; i < 100; i++)
		sharedData = sharedData + 1;
}

int main() {
	sharedData = 0;
	pthread_t thread[2];
	int i;

	for(i = 0; i < 2; i++)
		pthread_create(&thread[i], NULL, total, NULL);
	for(i = 0; i < 2; i++)
		pthread_join(thread[i], NULL);

	printf("%d/n", sharedData);

	return 0;
}

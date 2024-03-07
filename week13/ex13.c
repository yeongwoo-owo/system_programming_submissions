#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int nPointInCircle, nThread, nPoint;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

void* monteCarlo(void* arg){
	for(int i=0;i<nPoint;i++){
		double x=rand()/(double)RAND_MAX;
		double y=rand()/(double)RAND_MAX;
		
		if(x*x+y*y<=1){
			pthread_mutex_lock(&m);
			nPointInCircle++;
			pthread_mutex_unlock(&m);
		}
	}
}
	
int main(int argc, char** argv){
	nThread=atoi(argv[1]);
	nPoint=atoi(argv[2]);
	srand(time(NULL));
	
	for(int i=0;i<nThread;i++){
		pthread_t thread;
		pthread_create(&thread, NULL, &monteCarlo, NULL);
		pthread_join(thread, NULL);
	}

	printf("%f\n", 4*(float)nPointInCircle/((float)nThread*(float)nPoint));

	return 0;
}

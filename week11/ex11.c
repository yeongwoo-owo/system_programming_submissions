#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct thread_data {
	int thread_id;
	int result;
};

int row_size;
int col_size;

int** matrix;
int* vector;

/*** Insert your own matrix & vector data, functions ***/
void setMatrix(){
	printf("*** Matrix ***\n");
	for(int i=0;i<row_size;i++){
		for(int j=0;j<col_size;j++){
			matrix[i][j]=rand()%10;
			printf("[ %d ] ", matrix[i][j]);
		}
		printf("\n");
	}
}

void setVector(){
	printf("*** Vector ***\n");
	for(int i=0;i<col_size;i++){
		vector[i]=rand()%10;
		printf("[ %d ]\n", vector[i]);
	}
}

void printResult(struct thread_data* t_data){
	printf("*** Result ***\n");
	for(int i=0;i<row_size;i++){
		printf("[ %d ]\n", t_data[i].result);
	}
}

void *thread_mvm(void *arg) {
	/*** Insert your code ***/
	long ret=0;
	for(int i=0;i<col_size;i++){
		ret+=matrix[(long)arg][i]*vector[i];
	}

	return (void*)ret;
}

int main(int argc, char *argv[]){

	if (argc != 3) {
		printf("Usage: %s <row> <column>\n", argv[0]);
		exit(1);
	}

	row_size = atoi(argv[1]);
	col_size = atoi(argv[2]);
	pthread_t tid[row_size];	
	struct thread_data t_data[row_size];

	/*** Insert your code ***/

	srand(time(NULL));

	matrix=(int**)malloc(sizeof(int)*row_size*col_size);
	for(int i=0;i<col_size;i++){
		matrix[i]=(int*)malloc(sizeof(int)*row_size);
	}
	vector=(int*)malloc(sizeof(int)*col_size);

	setMatrix();
	setVector();

	long t;
	for(t=0;t<row_size;t++){
		if(pthread_create(&tid[t], NULL, thread_mvm, (void*)t)){
			printf("ERROR: pthread creation failed.\n");
			exit(1);
		}
	}
	
	for(t=0;t<row_size;t++){
		pthread_join(tid[t], (void**)&(t_data[t].result));
	}
	
	printResult(t_data);	
	
	for(int i=0;i<col_size;i++){
		free(matrix[i]);
	}
	free(matrix);
	free(vector);

	return 0;
}


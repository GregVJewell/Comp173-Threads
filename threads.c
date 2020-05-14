/*
	How to compile:
	unix >> gcc -o threads threads.c -pthread

	How to run:
	unix >> ./threads [vector size]

	Example:
	unix >> ./threads 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//Prototypes
void *malloc(size_t size);

//Child Thread
static void* child(void *argv[]) {
	int* A = argv[0]; //Load first passed array from package
	int* B = argv[1]; //Load second passed array from package
	
	//Unload Dynamic Size Value	
	int* size = argv[2];
	int n = size[0];

	int* C = malloc(sizeof(int) * n); //Create Dynamic Array (Prevent local variable overwrite on exit)

	for (int i = 0; i < n; i++){ //Add the array positions
		C[i] = A[i] + B[i];
	}

	printf("Child is done!\n"); //Graphic to show child thread ending

	pthread_exit(C); //Kill this thread, but return the sum-vector pointer
}

//Child Creation/Main
void main(int argc, char *argv[]) {

	pthread_t child_thread; //Address for Child Thread to be
	int n = atoi(argv[1]);
	int size[1] = { n };

	//Make sure size of arrays are greater than 0
	if (n <= 0) {
		fprintf(stderr, "Array sizes is incorrect! Size must be greater than 0!\n");
		exit(1);
	}

	//Allows for dynamic vector size
	int* A = malloc(sizeof(int) * n);
	int* B = malloc(sizeof(int) * n);

	//Take user input
	for (int i = 0; i < n; i++) {
		printf("Enter value of A[%d]:\n", i);
		scanf("%d", &A[i]);

		printf("Enter value of B[%d]:\n", i);
		scanf("%d", &B[i]);
	}

	printf("Packaging!\n");
	int* Pass[3] = { A, B, size }; //A package array for passing to child thread

	//Create/Start/Check that child thread starts correctly, also passes package array
	printf("Passing to child thread\n");
	if(pthread_create(&child_thread, NULL, (void *)child, Pass)) {
		fprintf(stderr, "pthread_create failed!\n");
	}

	int* C; //Variable to recieve child thread information (avoid a leak)
	pthread_join(child_thread, (void *)&C); //Pauses this thread, until child thread returns dynamic address

	for (int i = 0; i < n; i++) //Show the sum-vector values to show proof of computation and passing
		printf("Sum of A[%d] and B[%d] is: %d\n", i, i, C[i]);

	printf("Parent is done!\n"); //Shows parent/main thread ending
}

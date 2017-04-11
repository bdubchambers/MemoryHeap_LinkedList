/*
 * main.c
 *
 *  Created on: May 9, 2016
 *      Author: Brandon Chambers
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "mallok.h"

/* Function Prototypes */
void test0();
void test1();
void test2();
void test3();
void test4();
void customTest0();

int main(void) {
	//for eclipse console debug
	setvbuf(stdout, NULL, _IONBF, 0);

	//Tests here
	printf("\n------------------Test0\n");
	test0();
	printf("\n------------------Test1\n");
	test1();
	printf("\n-------------------Test2\n");
	test2();
	printf("\n-------------------Test3\n");
	test3();
	printf("\n-------------------Test4\n");
	test4();
	printf("\n----------------customTest0\n");
	customTest0();
	printf("\n----------------customTest1\n");
}

/**
 * - create a pool of 1000 bytes.
 * 		Count how times you can successfully request a block of size 10.
 */
void test0() {
	int count = 0, poolSize = 1000, blockSize = 10;
	create_pool(poolSize);
	int *ptr = my_malloc(blockSize);
	printf("create_pool(%d)\n", poolSize);
	printf("my_malloc(10) = %p, count = %d ...\n...\n", ptr,
			ptr == NULL ? count : ++count);
	while (my_malloc(blockSize) != NULL) {
		++count;
	}
	if ((poolSize % blockSize < blockSize) && (poolSize > blockSize))
		printf("Final Count: %d, Test0: Success\n", count);
	else
		printf("Final Count: %d, Test0: Failure\n", count);

}

/**
 * - create a pool of 1000 bytes.
 * 		Make 5 requests for blocks of 200 bytes.
 * 		Free all 5 blocks.
 * 		Repeat this request/freepattern several times to make sure
 * 			you can reuse blocks after they are returned.
 */
void test1() {
	int i, k, poolSize = 1000;
	void *ptr[5];
	create_pool(poolSize);
	printf("create_pool(%d)\n", poolSize);

	for (k = 1; k <= 5; k++) {
		printf("repeat = %d\n", k);
		for (i = 0; i < 5; i++) {
			printf("my_malloc(200)~assert:%p", ptr[i] = my_malloc(200));
			if (ptr[i] == 0x0)
				printf(" <-NULL, Failure!\n");
			else
				printf(" <- Valid address, Success!\n");
		}
		for (i = 0; i < 5; i++) {
			my_free(ptr[i]);
			ptr[i] = NULL;
		}
	}
}

/**
 * - create a pool of 1000 bytes.
 * 		Make 5 requests for blocks of 200 bytes.
 * 		Free the middle block.
 * 		Request a block of 210 bytes (it should fail)
 * 		Request a block of 150 bytes (it should succeed)
 * 		Request a block of 60 bytes (it should fail)
 * 		Request a block of 50 bytes (it should succeed)
 */
void test2() {
	int i, poolSize = 1000, numRequest = 5;
	void *ptr[numRequest], *ptr2 = NULL;
	create_pool(poolSize);
	printf("create_pool(%d)\n", poolSize);

	for (i = 0; i < numRequest; i++) {
		printf("my_malloc(200)~assert:%p", ptr[i] = my_malloc(200));
		if (ptr[i] == 0x0)
			printf(" <-NULL, Failure!\n");
		else
			printf(" <- Valid address, Success!\n");
	}
	my_free(ptr[numRequest / 2]);
	ptr[numRequest / 2] = NULL;

	printf("my_malloc(210): %p <- %s\n", ptr2 = my_malloc(210),
			ptr2 != NULL ? "Success" : "Failure");
	printf("my_malloc(150): %p <-", ptr2 = my_malloc(150));
	if (ptr2 == NULL)
		printf("Failure\n");
	else if (ptr2 != NULL)
		printf("Success\n");
	printf("my_malloc(60): %p <-", ptr2 = my_malloc(60));
	if (ptr2 == NULL)
		printf("Failure\n");
	else if (ptr2 != NULL)
		printf("Success\n");
	printf("my_malloc(50): %p <-", ptr2 = my_malloc(50));
	if (ptr2 == NULL)
		printf("Failure\n");
	else if (ptr2 != NULL)
		printf("Success\n");
}

/**
 * - create a pool of 1000 bytes.
 * 		Request 5 blocks of 200 bytes.
 * 		Fill the first block with the letter 'A', second block with 'B', etc.
 * 		Verify that the values stored in each block are still there.
 * 		(Is the first block full of A's, second block full of B's, etc.)
 */
void test3() {
	int i, k, poolSize = 1000, numRequest = 5, blockSize = 200;
	char c, *chPtr;
	void *ptr[numRequest];
	create_pool(poolSize);
	printf("create_pool(%d)\n", poolSize);
	//fill up each of the five 200byte blocks with A, B, C, D, and E.
	for (i = 0; i < numRequest; i++) {
		printf("my_malloc(200)~assert:%p\n", ptr[i] = my_malloc(blockSize));
		c = i + 65; //turn i into 'A', 'B', ... , 'E'
		memset(ptr[i], c, blockSize);
	}
	//ensure our blocks contain chars we put in ^^
	printf("\n*****Contents of the five blocks of 200-->\n");
	printf("\t\t\t\t                25\t\t\t\t                  50\n");
	for (i = 0; i < numRequest; i++) {
		printf("memory block #%d-->\n", i + 1);
		for (chPtr = (char*) ptr[i], k = 0; chPtr < (char*) ptr[i] + blockSize;
				chPtr++, k++) {
			if (k < 50) //print chars by width of 50
				printf("%c ", *chPtr); //indirection of chPtr to grab the char
			else {
				printf("\n%c ", *chPtr);
				k = 0;
			}
		}
		printf("\n");
	}
}

/**
 * - create a pool of 1000 bytes.
 * 		Request and return a block of 1000 bytes
 * 		Request and return four blocks of 250 bytes
 * 		Request and return ten blocks of 100 bytes
 */
void test4() {
	int i, poolSize = 1000, req250 = 250, req100 = 100;
	void *ptr = NULL, *ptrArr1[4], *ptrArr2[10];
	create_pool(poolSize);
	printf("create_pool(%d)\n", poolSize);

	printf("\nAttempt my_malloc(1000):\n");
	ptr = my_malloc(1000);
	if (ptr == 0x0)
		printf("Failure, unable to allocated 1000 bytes...");
	else
		printf("Success, able to allocate 1000 bytes. Assert:ptr=%p\n\n", ptr);
	my_free(ptr);

	printf("Attempt allocating four blocks of 250 bytes:\n");
	for (i = 0; i < 4; i++) {
		ptrArr1[i] = my_malloc(req250);
		if (ptrArr1[i] == 0x0)
			exit(1);
		printf("assert ptrArr1[%d]:%p\n",i,ptrArr1[i]);
	}
	printf("Successfully allocated four blocks of 250 bytes.\n");
	for (i = 0; i < 4; i++)
		my_free(ptrArr1[i]);

	printf("\nAttempt allocating ten blocks of 100 bytes:\n");
	for(i = 0; i < 10; i++) {
		ptrArr2[i] = my_malloc(req100);
		if(ptrArr2[i]==0x0)
			exit(1);
		printf("assert ptrArr2[%d]:%p\n",i,ptrArr2[i]);
	}
	printf("Successfully allocated ten blocks of 100 bytes.\n");
	for (i = 0; i < 10; i++)
			my_free(ptrArr2[i]);


}

/**
 * Create a pool of  199715099999 (close to as large as possible)
 * 		attempt to allocate all 100,000,000 bytes, then free
 * 		allocate 49,000,000 bytes
 * 		allocate another 49M bytes
 */
void customTest0() {
	unsigned long poolSize = 199715099999;
	void *ptr = NULL;
	create_pool(poolSize);
	printf("create_pool(%lu)\n", poolSize);

	printf("my_malloc(%lu), assert:%p\n",poolSize, ptr = my_malloc(poolSize));
	my_free(ptr);
	printf("my_free(ptr)\nRepeatedly allocate 49M bytes, then poolSize:\n");
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(49M), assert:%p\n", ptr = my_malloc(49000000));
	printf("my_malloc(%lu), assert:%p\n",poolSize, ptr = my_malloc(poolSize));
}

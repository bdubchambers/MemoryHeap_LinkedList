/*
 * mallok.c
 *
 *  Created on: May 9, 2016
 *      Author: Brandon Chambers
 */

#include <stdlib.h>

#include "mallok.h"

struct mem_node{
	void *addr;
	int blockSize;
	int allocated;//boolean 1 or 0
	struct mem_node *next;
};

typedef struct mem_node MemNode;

/* Global Static Variables */
static MemNode *front = NULL;


/*****************************************************************************
 *                 Contract Functions Defined in mallok.h
 *////////////////////////////////////////////////////////////////////////////

/*
 * The argument for create_pool indicates how many bytes should be included
 * in the heap. Your implementation of create_pool must use malloc to acquire
 *  a single block of exactly this size. This single block is your
 *  "private heap" (of memory).
 */
void create_pool(int bytes_requested) {
	//Use standard malloc to initialize our private heap/mem block
	void *memAddress = malloc(bytes_requested);

	if(front) {
		free(front->addr);
		free(front);
		front = NULL;
	}
	//assert(front == NULL);
	//Check that malloc allocated valid memory block
	if(memAddress != NULL) {
		front = (MemNode *) malloc( sizeof(MemNode) );
		front->next = NULL;
		front->allocated = 0;
		front->blockSize = bytes_requested;
		front->addr = memAddress;
	}
}

/**
 * All calls to my_malloc must be satisfied by returning a piece of this
 * private heap if possible. my_malloc should not simply use malloc to
 * allocate the block the caller has asked for.  my_malloc must search the
 * block list for the FIRST free block that is big enough to satisfy the
 * requested size. If the first free block is more than big enough, then
 * my_malloc should use the first part of the block to satisfy the request
 * and keep the latter part in the private heap.
 */
void *my_malloc(int bytes_requested) {
	MemNode *tempNode, *currNode;
	currNode = front;

	//Stop on a section of our memory pool with enough unallocated memory
	while(currNode != NULL &&
			( (currNode->allocated) ||
				(currNode->blockSize < bytes_requested) )) {
		currNode = currNode->next;
	}//If found, currNode is now pointing at the candidate block of memory.

	//Ensure we have a sufficient sized block and other checks, then if
	//the block size is bigger than 'bytes_requested', split into two
	//separate, addressable blocks (MemNodes in linked list).
	if(currNode != NULL && currNode->blockSize >= bytes_requested) {
		if(currNode->blockSize > bytes_requested) {
			//swap & split block into two MemNodes
			tempNode = (MemNode *)malloc(sizeof(MemNode));
			tempNode->next = currNode->next;
			tempNode->allocated = 0;
			tempNode->blockSize = currNode->blockSize - bytes_requested;
			tempNode->addr = (void *)(bytes_requested + (char *)currNode->addr);
			currNode->next = tempNode;//remaining memblock is now after current
		}
		currNode->allocated = 1;
		currNode->blockSize = bytes_requested;

		return currNode->addr;
	} else {
		return NULL;//Requested memory block unavailable
	}
}

/**
 * my_free should return a block to the private heap, not to the built-in
 * heap by calling free, making that storage once again available. In the
 * process you must recombine the block with any neighboring blocks that
 * are also free.
 */
void my_free(void *block){
	MemNode*prevNode,*currNode,*nextNode;
	prevNode = NULL;
	currNode = front;

	//locate address of memblock to free
	while(currNode != NULL && currNode->addr != block){
		prevNode = currNode;
		currNode = currNode->next;
	}
	//the block either doesn't exist or hasn't been allocated
	if(currNode == NULL)
		return;

	currNode->allocated = 0;

	/************************************************************************
	 *			Merge Adjacent Blocks of Memory Into One Block
	 *///////////////////////////////////////////////////////////////////////

	//If unallocated, combine the previous MemNode with this MemNode
	if(prevNode != NULL && !prevNode->allocated){
		prevNode->blockSize += currNode->blockSize;
		prevNode->next = currNode->next;
		free(currNode);
		currNode=prevNode;
	}
	//If unallocated, combine the next memblock/Node with this memblock/Node
	nextNode = currNode->next;
	if(nextNode != NULL && !nextNode->allocated){
		currNode->blockSize += nextNode->blockSize;
		currNode->next = nextNode->next;
		free(nextNode);
	}
	/////////////////////////////End Merge///////////////////////////////////
}

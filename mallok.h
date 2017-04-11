/*
 * mallok.h
 *
 *  Created on: May 9, 2016
 *      Author: Brandon Chambers
 */
#ifndef MALLOK_H_
#define MALLOK_H_

void create_pool(int size);
void *my_malloc(int size);
void my_free(void *block);

#endif /* MALLOK_H_ */

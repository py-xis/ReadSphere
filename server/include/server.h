#ifndef SERVER_H
#define SERVER_H

#include "components.h"
#include <pthread.h>

#define PORT 8080
extern pthread_mutex_t lock;

void *
serverFunc(void  *incomingSock);


#endif
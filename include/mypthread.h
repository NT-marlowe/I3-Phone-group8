#ifndef INCLUDE_pthread_H_
#define INCLUDE_pthread_H_

struct args_pthread
{
  int s; // file descriptor for send and recv
  signed short *buf; // pointer of the head of data array
  int *fail;
};

typedef struct args_pthread Args_pthread;

void *receive_data_from_client(void *arg);

void sum_waves(signed short **waves, signed short *result, int number_of_client);

void *send_data_to_client(void *arg);

#endif



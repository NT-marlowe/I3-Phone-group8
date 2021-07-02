#ifndef INCLUDE_pthread_H_
#define INCLUDE_pthread_H_

struct args_pthread
{
  int s; // file descriptor for send and recv
  signed short *buf; // pointer of the head of data array
};

void *receive_data_from_client(void *arg);

void sum_waves(signed short **waves, signed short *result);

void *send_data_to_client(void *arg);

#endif



#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "../include/die.h"
#include "../include/mypthread.h"

#define N 1024

void sum_waves(signed short **waves, signed short *result, int number_of_client) {
  
  for (int j = 0; j < N; j++) {
    result[j] = 0;
  }

  for (int i = 0; i < number_of_client; i++) {
    for (int j = 0; j < N; j++) {
      result[j] += waves[i][j];
    }
  }
}


void *send_data_to_client(void *arg) {
  Args_pthread* p = (Args_pthread*)arg;
  int s = p->s;
  signed short *buf = p->buf;

  int n = send(s, buf, N * sizeof(buf[0]), 0);
  if (n == -1) die("send");

  return NULL;
}

void *receive_data_from_client(void *arg){
  Args_pthread *pd = (Args_pthread*)arg;
  int s = pd->s;
  signed short *buf = pd->buf;
  int *fail = pd->fail;

 

  int m = recv(s, buf, N * sizeof(signed short),0);
  if (errno == 11) {
    for(int i = 0; i < N; i++){
      buf[i] = 0;
    }
    *fail += 1;
  }
  else if (m == -1) die("recv");
  else {
    ;
  }

  return NULL;
}

#include <sys/types.h>
#include <sys/socket.h>
#include "../include/die.h"
#include "../include/pthread.h"

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

  int n = send(s, buf, N, 0);
  if (n == -1) die("send");
}

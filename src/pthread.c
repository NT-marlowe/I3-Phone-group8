#define N 1024
void *receive_data_from_client(void *arg){
  Args_pthread *pd = (Args_pthread*)arg;
  int s = pd->s;
  signed short *buf = pd->buf;
  int m = recv(s,buf,N,0);
  if (m == -1) die("recv");
}

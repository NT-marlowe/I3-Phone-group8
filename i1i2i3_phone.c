#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define N 128

void die(char *s) {
    perror(s);
    exit(1);
}

int main(int argc, char ** argv) {
  
  if (argc != 3) {
    fprintf(stderr, "usage as server: %s -l Port_number\n", argv[0]);
    fprintf(stderr, "usage as client: %s IP_address Port_number\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int ss = socket(PF_INET, SOCK_STREAM, 0);
  int s;
  int server = strcmp(argv[1], "-l") == 0;

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));

  if (server) { // work as server
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ss, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
      die("bind");
    }

    if (listen(ss, 10) == -1) die("listen");
    
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    s = accept(ss, (struct sockaddr *)&client_addr, &len);
    if (s == -1) die("accept");
    fprintf(stderr, "connection from %s %s succeeded!\n", inet_ntoa(client_addr.sin_addr), argv[2]);
  }

  else {
    s = ss;

    if (inet_aton(argv[1], &addr.sin_addr) == 0) die("inet_aton");

    int connectionTryCount = 0;
    int c = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    
    /*
    クライアント側でも接続待ちができるようにする．接続が失敗した場合，1秒毎に接続を再試行する．
    5回試行しても失敗する場合，プログラムを終了する， 
    */
    while (c == -1 && connectionTryCount < 5)　
    {
      fprintf(stderr, "Trying to connect...\n");
      sleep(1);
      c = connect(s, (struct sockaddr *)&addr, sizeof(addr));
      connectionTryCount++;
    }
    if (c == -1 && connectionTryCount == 5) die("connect");

    fprintf(stderr, "connection to %s %s port succeeded!\n", argv[1], argv[2]);
  }
  

  FILE *fp = NULL;
  char *commandline = "rec -t raw -b 16 -c 1 -e s -r 44100 - ";
  if ((fp = popen(commandline, "r")) == NULL ) {
    close(ss);
    die("peopen");
  }

  unsigned char data[N];
  while (1)
  {
    int n = fread(data, sizeof(unsigned char), N, fp);
    
    if (n == -1) {
      close(ss);
      die("read");
    }

    if (feof(fp)) break; // end of file
    if (ferror(fp)) {
      close(ss);
      die("errorOfStream");
    }

    if (n < N) break; // これもend of file?
    
    if (send(s, data, n * sizeof(unsigned char), 0) == -1) {
      close(ss);
      die("send");
    }
  }

  close(s);
  if (server) close(ss);
  pclose(fp);
  return 0;

}
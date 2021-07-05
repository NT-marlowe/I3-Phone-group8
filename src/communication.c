#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../include/die.h"

#define N 1024

// server関数の引数に配列sを追加．
// client関数の引数にポインタsを追加．
// close(s);, for i; close(s[i])をmain()の中で実行する．

void server(int port,int number_of_client, int *s){
  // int s[number_of_client];
  int ss = socket(PF_INET,SOCK_STREAM,0);
  if (ss == -1) die("socket");
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
  
  int bi = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  if (bi == -1) die("bind");
  if (listen(ss,10) == -1) die("listen");

  // ここで複数回アクセプト
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  for (int i = 0; i < number_of_client; i++) {
    s[i] = accept(ss,(struct sockaddr *)&client_addr, &len);
    if (s[i] == -1) die("accept");
    else {
      fprintf(stderr, "connection from %s %d succeeded!\n", inet_ntoa(client_addr.sin_addr), port);
    }
  }

  //このソケットを用いたすべてのread、recvに対してタイムアウトを設定する
  //タイムアウトしたときのerrnoは11
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 1e6*N/44100;
  for (int i = 0; i < number_of_client; i++) {
    setsockopt(s[i], SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
    if (setsockopt(s[i], SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) == -1) die("setsockopt");
  }
  
  //接続が完了したら各クライアントにダミーデータ(1)を送る
  int data[1];
  data[0] = 1;
  
  for(int i = 0; i< number_of_client; i++){
    if (send(s[i], data, sizeof(int), 0) == -1) die("send dummy_data");
  }


  close(ss); 
}

void client(char* address, int port, int *s){ // このsは参照渡し
  // int s = socket(PF_INET,SOCK_STREAM,0);  // sはmainで宣言して，client()内で接続を行う．
  // if(*s == -1) die("socket");
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  int at = inet_aton(address, &addr.sin_addr);
  if (at == 0) die("inet_aton");
  addr.sin_port = htons(port);
  int ret = connect(*s, (struct sockaddr *)&addr, sizeof(addr));
  if (ret == -1) die("connect");
  else {
    fprintf(stderr, "connection to %s %d succeeded!\n", address, port);
  }

  int data = 0;
  // ここで何かしらのデータが来るまでせきとめないと先に接続した方のクライアントが録音を開始してしまう
  if (recv(*s, &data, sizeof(int), 0) == -1) die("recv dummy_data");
}



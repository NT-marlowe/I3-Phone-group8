#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define N 20


void server(int port,int number){
  int s[number];
  int ss = socket(PF_INET,SOCK_STREAM,0);
  if(ss==-1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
  int bi = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  if(bi==-1){
    perror("bind");
    exit(1);
  }

  if(listen(ss,10)==-1) {perror("listen"); exit(1);};
 //ここで複数回アクセプト
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  for(int i = 0;i<number;i++){
    s[i] = accept(ss,(struct sockaddr *)&client_addr, &len);
    if(s[i] == -1){
	perror("accept");
	exit(1);
    }
  }


  close(ss);
//popen関数でrecを起動
    FILE	*fp;
  if ( (fp=popen("rec -t raw -b 16 -c 1 -e s -r 44100 -","r")) ==NULL) {
		perror ("popen error");
		exit(1);
	}

  
  //サーバー側がデータを送ってせき止めを解除することでクライアント共も録音を始める
  char data[1];
  data[1] = 1;
  for(int i = 0;i<number;i++){
    if(write(s[i], data, 1)==-1) {perror("write"); exit(1);}
   }


  while(1){
    //buf[0]にサーバー、buf[>0]にそれぞれのクライアントから受け取った波形を格納
    char buf[number+1][N];
    int n = fread(buf[0],sizeof(char),N,fp);
    if(n==-1){perror("fread"); exit(1);}
    if(n==0) break;

    for(int i = 0;i<number;i++){
      int m = recv(s[i],buf[i+1],N,0);
      if(m==-1){perror("open"); exit(1);}
      if(m==0) break;
    }
    //send[0]を標準出力、send[>0]は各々のサーバーに送る。bufを足し合わせてこれを作るが、自分の声が入らないように配慮する。
    char send[number+1][N];

    for(int i = 0;i<N;i++){
      for(int j = 0;j<number+1;j++){
	send[j][i] = 0;
	for(int k = 0;k<number+1;k++){
	  if(j!=k){
	    send[j][i] += buf[k][i];
	  }
	}
      }
    }
    //サーバーの標準出力
    if(write(1, send[0], N)==-1) {perror("write"); exit(1);}
    //クライアントへの書き込み
    for(int i = 0;i<number;i++){
      if(write(s[i], send[i+1], N)==-1) {perror("write"); exit(1);}
    }


  }
  pclose(fp);
  for(int i = 0;i<number;i++){ close(s[i]);}
}






void client(char* address,int port){
  int s = socket(PF_INET,SOCK_STREAM,0);
  if(s==-1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  int at = inet_aton(address,&addr.sin_addr);
  if(at==0){
    printf("error:inet_aton");
    exit(1);
  }
  addr.sin_port = htons(port);
  int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if(ret==-1){
    perror("connect");
    exit(1);
  }
  //ここで何かしらのデータが来るまでせきとめないと先に接続した方のクライアントが録音を開始してしまう
  recv(s,0,1,0);

  //popenによってrecを起動
  FILE	*fp;
  if ( (fp=popen("rec -t raw -b 16 -c 1 -e s -r 44100 -","r")) ==NULL) {
		perror ("popen error");
		exit(1);
	}
  
  while(1){
    
    char buf[N];
    int n = fread(buf,sizeof(char),N,fp);
    if(n==-1){perror("fread"); exit(1);}
    if(n==0) break;
    if(write(s, buf, N)==-1) {perror("write"); exit(1);}

    char buf2[N];
    int m = recv(s,buf2,N,0);
    if(m==-1){perror("open"); exit(1);}
    if(m==0) break;
    if(write(1, buf2, N)==-1) {perror("write"); exit(1);}
    }
  pclose(fp);
  close(s);
}













int main(int argc, char ** argv)
{
/*./a.out (ポート番号) (人数) s でサーバーになる*/
  if(argc == 4) {
    if(argv[3][0]=='s'){
      int number = atoi(argv[2])-1;
      int port = atoi(argv[1]);
      server(port,number);

//./a.out (IPアドレス) (ポート番号) c でクライアントになる
    }else if(argv[3][0]=='c'){
    char* address;
    address = (char*)malloc(sizeof(char)*strlen(argv[1]));
    strcpy(address,argv[1]);
    int port = atoi(argv[2]);
    client(address,port);  
    }else {perror("３つめはsかcにしてください。"); exit(1);}




  } else {perror("引数を３つにしてください。(ポート)　(人数)　sでサーバー、(ポート) (人数) cでクライアント");  exit(1);}


  return 0;
}

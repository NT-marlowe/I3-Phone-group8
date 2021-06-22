#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc, char ** argv)
{
//クライアントはsとaとbの3つ
  int s;
  int a;
  int b;
//一サイクルで送るデータ量、少なすぎると音がブチブチ切れていて聞けたものじゃなかった。でもこのプログラムの構造上大きいと遅延が増えそう
  int N = 20;
//引数２つはサーバーになる
  if(argc == 2) {		//printf("please enter filename");  exit(1);}
  int ss = socket(PF_INET,SOCK_STREAM,0);
  if(ss==-1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(atoi(argv[1]));
  int bi = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  if(bi==-1){
    perror("bind");
    exit(1);
  }

  if(listen(ss,10)==-1) {perror("listen"); exit(1);};
 //ここでsとaとbで三回アクセプト
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  s = accept(ss,(struct sockaddr *)&client_addr, &len);
  if(s == -1){
	perror("accept");
	exit(1);
  }
  

  a = accept(ss,(struct sockaddr *)&client_addr, &len);
  if(a == -1){
	perror("accept");
	exit(1);
  }
  b = accept(ss,(struct sockaddr *)&client_addr, &len);
  if(a == -1){
	perror("accept");
	exit(1);
  }

  close(ss);
  }
//引数３つはクライアントになるが、動作はほぼI2時点から変わらず
  if(argc == 3) {//printf("please enter filename");  exit(1);}
  s = socket(PF_INET,SOCK_STREAM,0);
  if(s==-1){
    perror("socket");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  int at = inet_aton(argv[1],&addr.sin_addr);
  if(at==0){
    printf("error:inet_aton");
    exit(1);
  }
  addr.sin_port = htons(atoi(argv[2]));
  int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if(ret==-1){
    perror("connect");
    exit(1);
  }
//ここで何かしらのデータが来るまでせきとめないと先に接続した方のクライアントが録音を開始してしまう
recv(s,0,1,0);
  }
 


//popenによってrecを起動
  FILE	*fp;
  if ( (fp=popen("rec -t raw -b 16 -c 1 -e s -r 44100 -","r")) ==NULL) {
		perror ("popen error");
		exit(1);
	}
//サーバー側がデータを送ってせき止めを解除することでクライアント共も録音を始める
if(argc == 2){
char data[1];
data[1] = 1;
    if(write(s, data, 10)==-1) {perror("write"); exit(1);}
    
    if(write(a, data, 10)==-1) {perror("write"); exit(1);}

    if(write(b, data, 10)==-1) {perror("write"); exit(1);}

}
  
  
  while(1){
//サーバー側はサーバーの音クライアントの音3つの4つのデータを適宜合成する
  if(argc == 2){
    char buf[N];
    int n = fread(buf,sizeof(char),N,fp);
    if(n==-1){perror("fread"); exit(1);}
    if(n==0) break;

    char buf2[N];
    int m = recv(s,buf2,N,0);
    if(m==-1){perror("open"); exit(1);}
    if(m==0) break;

    char buf3[N];
    int l = recv(a,buf3,N,0);
    if(l==-1){perror("open"); exit(1);}
    if(l==0) break;

    char buf4[N];
    int o = recv(b,buf4,N,0);
    if(o==-1){perror("open"); exit(1);}
    if(o==0) break;

    char send1[N];
    char send2[N];
    char send3[N];
    char receive[N];
for(int i = 0;i<N;i++){
    send1[i] = buf[i]+buf3[i]+buf4[i];
    send2[i] = buf[i]+buf2[i]+buf4[i];
    send3[i] = buf[i]+buf2[i]+buf3[i];
    receive[i] = buf2[i]+buf3[i]+buf4[i];  
}
//サーバーの標準出力は3つのクライアントの合成
    if(write(1, receive, N)==-1) {perror("write"); exit(1);}
//クライアントへの書き込みはサーバーの音ともう片方のクライアントの音の合成
    if(write(s, send1, N)==-1) {perror("write"); exit(1);}
    
    if(write(a, send2, N)==-1) {perror("write"); exit(1);}

    if(write(b, send2, N)==-1) {perror("write"); exit(1);}


//クライアント側は二者間のときと同じ
  }else{
    char buf[N];
    int n = fread(buf,sizeof(char),N,fp);
    // int n = read(0,buf,1);
    if(n==-1){perror("fread"); exit(1);}
    if(n==0) break;
    if(write(s, buf, N)==-1) {perror("write"); exit(1);}

    char buf2[N];
    int m = recv(s,buf2,N,0);
    if(m==-1){perror("open"); exit(1);}
    if(m==0) break;
    if(write(1, buf2, N)==-1) {perror("write"); exit(1);}
}
    }
  pclose(fp);
  close(s);
  close(a);
  return 0;
}

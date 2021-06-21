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
  int s;
  int a;
  // argc = 2ならサーバー3ならクライアント
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
 
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
	  
	  //二回アクセプトする。一個目がs,二個目がa.
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


  close(ss);
  }
//クライアント側は変わらず
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
  }
 


  //popen使ってrec起動するやつ
  FILE	*fp;
  if ( (fp=popen("rec -t raw -b 16 -c 1 -e s -r 44100 -","r")) ==NULL) {
		perror ("popen error");
		exit(1);
	}
  
  
  while(1){
  if(argc == 2){
	  //サーバー側は、recからのデータ、２つのクライアントからのデータの３つをもらい、それぞれに必要な２つのデータの足し合わせを送りつける
    char buf[1];
    int n = fread(buf,sizeof(char),1,fp);
    if(n==-1){perror("fread"); exit(1);}
    if(n==0) break;

    char buf2[1];
    int m = recv(s,buf2,1,0);
    if(m==-1){perror("open"); exit(1);}
    if(m==0) break;

    char buf3[1];
    int l = recv(a,buf3,1,0);
    if(l==-1){perror("open"); exit(1);}
    if(l==0) break;
//sに送るやつ
    char send1[1];
//aに送るやつ
    char send2[1];
//自分用
    char receive[1];

    send1[1] = buf[1]+buf3[1];
    send2[1] = buf[1]+buf2[1];
    receive[1] = buf2[1]+buf3[1];  
   
    if(write(1, receive, 1)==-1) {perror("write"); exit(1);}

    if(write(s, send1, 10)==-1) {perror("write"); exit(1);}
    
    if(write(a, send2, 1)==-1) {perror("write"); exit(1);}



//クライアントは変わらず
  }else{
    char buf[1];
    int n = fread(buf,sizeof(char),1,fp);
    // int n = read(0,buf,1);
    if(n==-1){perror("fread"); exit(1);}
    if(n==0) break;
    if(write(s, buf, 1)==-1) {perror("write"); exit(1);}

    char buf2[1];
    int m = recv(s,buf2,1,0);
    if(m==-1){perror("open"); exit(1);}
    if(m==0) break;
    if(write(1, buf2, 1)==-1) {perror("write"); exit(1);}
}
    }
  pclose(fp);
  close(s);
  return 0;
}

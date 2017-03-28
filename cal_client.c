#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // sockaddr_in ,inet_addr
#include <sys/socket.h> //socket connect send recv
#include <stdlib.h> // atoi
#include <unistd.h> //close

#define BUFSIZE 1024
#define INT_SZ 4

int main(int argc, char* argv[])
{
  struct sockaddr_in serv_addr, clnt_addr;
  int clnt_sock, i;
  int modifi_len;
  int total_recvbyte = 0, recvbyte;
  char *serv_ip,modifi[BUFSIZE]={0},recv_msg[BUFSIZE]={0}; 
  unsigned short serv_port;

  clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (clnt_sock < 0){
    perror("socket() error\n");
    exit(1);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(3333);
  
  printf("Input Format : num1' 'num2' 'operator\n");
  for(i=0;i<2;++i){
    scanf("%d",(int *) &modifi[INT_SZ*i]);
  }
  scanf("%c", &modifi[INT_SZ*2]);
  modifi[strlen(modifi)+1]=0;
  if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
    perror("connect() error");
    exit(1);
  }
  write(clnt_sock, modifi, strlen(modifi));
  read(clnt_sock, recv_msg, BUFSIZE);
  printf("%d\n",(int *)recv_msg);
  close(clnt_sock);
  return 0;
  /*
  for (i=0;i<100;i++) {
    total_recvbyte = 0;
    if ( write(clnt_sock, modifi, strlen(modifi)+1) < 0){
      perror("send() error");
      exit(1);
    }

    while (total_recvbyte < modifi_len) {
      if ((recvbyte = recv(clnt_sock, recv_msg, RECV_BUFSIZE-1, 0)) <= 0){
        perror("recv() error");
        exit(1);
      }
      total_recvbyte += recvbyte;
      recv_msg[recvbyte] = '\0';
      printf("Re: %s\n", recv_msg);
    }
  }
      puts("");
      close(clnt_sock);
      return 0;*/
}

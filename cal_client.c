#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // sockaddr_in ,inet_addr
#include <sys/socket.h> //socket connect send recv
#include <stdlib.h> // atoi
#include <unistd.h> //close

#define RECV_BUFSIZE 1024

int main(int argc, char* argv[])
{
  struct sockaddr_in serv_addr, clnt_addr;
  int clnt_sock, i;
  int modifi_len;
  int total_recvbyte = 0, recvbyte;
  char *serv_ip,*modifi,recv_msg[RECV_BUFSIZE]={0}; 
  unsigned short serv_port;

  if (argc != 4) {
    fprintf(stderr, "Usage : [%s] [server_ip] [server_port] [num1] [operator] [num2] \n", argv[0]);
    exit(1);
  }
  serv_ip = argv[1];
  serv_port = atoi(argv[2]);
  modifi = argv[3];

  clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (clnt_sock < 0){
    perror("socket() error\n");
    exit(1);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
  serv_addr.sin_port = htons(serv_port);

  if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
    perror("connect() error");
    exit(1);
  }

  modifi_len = strlen(modifi);
  for (i=0;i<100;i++) {
    total_recvbyte = 0;
    if (send(clnt_sock, modifi, modifi_len, 0) != modifi_len){
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
      return 0;
}

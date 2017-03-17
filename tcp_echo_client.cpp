#include <arpa/inet.h> // sockaddr_in ,inet_addr
#include <sys/socket.h> //socket connect send recv
#include <stdlib.h> // atoi
#include <unistd.h> //close

#define RECV_BUFSIZE 1024

int main(int argc, char* argv[])
{
  struct sockaddr_in serv_addr, clnt_addr;
  int clnt_sock, i;
  int send_msg_len;
  int total_recvbyte = 0, recvbyte;
  char* serv_ip,*send_msg,recv_msg[RECV_BUFSIZE]; // recv msg 초기화 해주나? 어딨누 똥같은넘아
  unsigned short serv_port;


  if (argc != 4) {
    fprintf(stderr, "Usage : [%s] [server_ip] [server_port] [message]\n", argv[0]);
    exit(1);
  }
  serv_ip = argv[1];
  serv_port = atoi(argv[2]);
  send_msg = argv[3];

  clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (clnt_sock < 0){
    perror("socket() error\n")
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

  send_msg_len = strlen(send_msg);
  for (i=0;i<100;i++) {
  //  이렇게 하면 총 10번 메세지 보내겠지?
    total_recvbyte = 0;
    if (send(clnt_sock, send_msg, send_msg_len, 0) != send_msg_len){
    perror("send() error");
    exit(1);
  }

    while (total_recvbyte < send_msg_len) {
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

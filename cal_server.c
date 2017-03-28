#include <stdio.h> //printf, fprintf
#include <string.h> // memset()
#include <stdlib.h> // atoi()
#include <unistd.h> // close()
#include <arpa/inet.h> //sockaddr_in,inet_ntoa()
#include <sys/socket.h> //socket() bind() connect()
#define BUFSIZE 1024
#define INT_SZ 4

int calculate(int n1,int n2,char op);

int main(int argc, char* argv[])
{
  char modifi[BUFSIZE]={0};
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;

  //1. create socket
  serv_sock = socket(PF_INET, SOCK_STREAM, 0); //socket(domain, type, protocol)
  if (serv_sock < 0) {
    perror("socket() error");
    exit(1);
  }
  //2. init server_addr & fill the address struct
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(3333)); //htons : host byte -> short?
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl : host byte -> long int
  //INADDR_ANY : automatically fill the IP addr

  //3. bind()
  if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind error()");
    exit(1);
  }
  //4. listen()
  if (listen(serv_sock, 5) <0 ) {
    perror("listen() error");
    exit(1);
  }

  int modifi_len,num1,num2;
  char op;
  unsigned int clnt_addr_len;
  
  while (1)
  {
    clnt_addr_len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    if (clnt_sock < 0) {
      perror("accept() error");
      exit(1);
    }
    read(clnt_sock, modifi, BUFSIZE);
    printf("recieved : %s\n",modifi);
    num1 = (int *) modifi[INT_SZ*0];
    num2 = (int *) modifi[INT_SZ*1];
    op = modifi[INT_SZ*2];

    printf("%d\n",calculate(num1,num2,op));
    /*
    while(1)
    {
      if(readn(clnt_sock, modifi, 2) <= 0) break;
      short len = *(short*) modifi_len;
      if(readn(clnt_sock, modifi, len-2) <=0 )break;
      modifi[len-2]=0;
      printf("Result : %d\n",calculate(modifi));
    }
    */
    close(clnt_sock);
  }
  return 0;
}
/*
bool readn(int clnt_sock, char *buf, int n)
{
  int sp =0;
  while(n)
  {
    int readed = read(clnt_sock, buf+sp, n);
    if( readed <=0 ) return false;
    n -= readed;
    sp += readed;
  }
  return true;
}
*/
int calculate(int n1, int n2, char op)
{
  if(op == '+')
    return n1+n2;
  else if(op=='-')
    return n1-n2;
  else if(op=='*')
    return n1*n2;
  printf("Input Valid Operator\n");
  return -1;
}

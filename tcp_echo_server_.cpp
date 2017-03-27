#include <stdio.h> //printf, fprintf
#include <string.h> // memset()
#include <stdlib.h> // atoi()
#include <unistd.h> // close()
#include <arpa/inet.h> //sockaddr_in,inet_ntoa()
#include <sys/socket.h> //socket() bind() connect()
#define bufsize 1024

int main(int argc, char* argv[])
{
  char msg[1024];
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clnt_addr;

  if (argc != 2) {
    fprintf(stderr, "Usage : [ %s ] [port]\n",argv[0]);
    exit(1);
  }
  //1. create socket
  serv_sock = socket(PF_INET, SOCK_STREAM, 0); //socket(domain, type, protocol)
  if (serv_sock < 0) {
    perror("socket() error");
    exit(1);
  }
  //2. init server_addr & fill the address struct
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[1])); //htons : host byte -> short?
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
  /*5. while(1)
       {
      5.1. accept()
      5.2. rcvd()
      {
        if failed, continue
        else send()
      }
     }
     close()
  */
  int rcv_size;
  unsigned int clnt_addr_len;
  while (1)
  {
    clnt_addr_len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    if (clnt_sock < 0) {
      perror("accept() error");
      exit(1);
    }
	while (1)
	{
		if (readn(2, clnt_sock, msg) == 0) {
			break;
		}
		short len = *(short*)msg;
		if (readn(len - 2, clnt_sock, msg) == 0) {
			break;
		}
		msg[len - 2] = 0;

		printf("Client said : %s\n", msg);
	}
    close(clnt_sock);
  }
  return 0;
}

bool readn(int n, int clnt_sock, char* buffer)
{
	int start_pointer = 0;
	while (n)
	{
		int readed = read(clnt_sock, n, buffer + start_pointer);
		if (readed <= 0) {
			return false;
		}
		n -= readed;
		start_pointer += readed;
	}
	return true;
}

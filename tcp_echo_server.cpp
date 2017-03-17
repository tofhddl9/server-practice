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

	if (argc != 2)
	{
		fprintf(stderr, "Usage : [ %s ] [port]\n",argv[0]);
		exit(1);
	}
	//1. create socket
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); //socket(domain, type, protocol)
	if (serv_sock < 0)
	{
		fprintf(stderr, "socket() error");
		exit(1);
	}
	//2. init server_addr & fill the address struct
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1])); //htons : host byte order -> short?
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl : host byte -> long int
 
										//INADDR_ANY : automatically fill the IP addr

	//3. bind()
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
          perror("bind error()\n");
//		fprintf(stderr, "bind() error\n");
		exit(1);
	}
	//4. listen()
	if (listen(serv_sock, 5) <0 )
	{
		fprintf(stderr, "listen() error\n");
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
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_len);
		if (clnt_sock < 0)
		{
			fprintf(stderr, "accept() error\n");
			exit(1);
		}
		rcv_size = recv(clnt_sock, msg, bufsize, 0);
		if (rcv_size <= 0)
		{
      perror("recv() error");
      exit(1);
		}
		while (rcv_size > 0)
		{
			if (send(clnt_sock, msg, rcv_size, 0) != rcv_size)
				fprintf(stderr, "send() error\n");
			rcv_size = recv(clnt_sock, msg, bufsize, 0);
			if (rcv_size <= 0)
      {
        perror("recv() error");
        break;
      }
		}
		close(clnt_sock);
	}
	return 0;
}

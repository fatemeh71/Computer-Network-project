#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string>
#include <errno.h>
#include "socket.h"

class ServerSocket{
  public:
  int sock;
  int BACKLOG;

  ServerSocket(string ServerPort){
    socklen_t addr_size;
    struct addrinfo hints, *res;
    /******************/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, ServerPort.c_str(), &hints, &res);

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    if(sock == -1)
      perror("ServerSocket/socket");
    if(bind(sock, res->ai_addr, res->ai_addrlen) == -1)
      perror("ServerSocket/bind");
    BACKLOG=10;
  }
  Socket Accept(){

    if(listen(sock, BACKLOG) == -1)
      perror("Accept/Listen");
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    addr_size = sizeof(their_addr);
    //cout<<"helllooooo"<<endl;
    //cout<<"original socket in accept "<<sock<<endl;
    int new_sock= accept(sock, (struct sockaddr * )&their_addr, &addr_size);
    //cout<<"accept passed"<<endl;
    if(new_sock == -1)
      perror ("Accept/accept");
    Socket temp(new_sock);
    //cout<<"from accept : "<<temp.print_fd()<<endl;
    return temp;
  }
  string getIP(){
	socklen_t len = 32;
	char *buf = new char[32];
	
	struct sockaddr_in res;
	if(getsockname(sock, (sockaddr*)buf, &len) == -1)
		perror("get_server_ip");
	sockaddr *addr = (sockaddr*)buf;
	if (addr->sa_family == AF_INET)
		return inet_ntoa( ((sockaddr_in*)addr)->sin_addr );
	else{
		char *ip6 = new char[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &((sockaddr_in6*)addr)->sin6_addr, ip6, INET6_ADDRSTRLEN);
		return ip6;
	}
  }
  void Close (){
    close(sock);
  }
  ~ServerSocket(){
    
  }
};

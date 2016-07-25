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
#include <iostream>
#define buffer_len 100
using namespace std;
class Socket{

  int sock;
  int buff_head;
  int buff_filled;
  char* buffer;
  string local_ip;
  int local_port;
  public:
  Socket(string ip_address, string port){

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip_address.c_str(), port.c_str(), &hints, &res);
    sock = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sock == -1){
      perror("Socket/socket");
    }
    if(connect(sock, res->ai_addr, res->ai_addrlen) == -1)
      perror("Socket/connect");
    //cout<<"connect passed"<<endl;
    buffer = new char [buffer_len];


    buff_head=0;
    buff_filled=0;
    struct sockaddr_in local_address;
    socklen_t address_len = 2*sizeof(local_address);
    getsockname(sock, (struct sockaddr*)& local_address, &address_len);
    local_ip = inet_ntoa(local_address.sin_addr);
    local_port = (int) ntohs(local_address.sin_port);

  }
  Socket(int s){
    sock = s;
    buffer = new char [buffer_len];
    buff_head=0;
    buff_filled=0;
  }
  string Read_Line(){

    string line;
    //cout<<buff_head<<" "<<buff_filled<<endl;
    for(int i= buff_head; i < buff_filled; i++){
      if(buffer[i] == '\0'){
	    buff_head = i+1;
        return line;
      }
      line += buffer[i];
    }
    while (true){
      buff_head = 0;
      buff_filled = recv(sock, buffer, buffer_len, 0);

      if(buff_filled == -1){
        perror("Read_line/ recv");
        return line;
      }

      for(int i= buff_head; i < buff_filled; i++){
		if(buffer[i] == '\0'){
          buff_head = i+1;
          return line;
        }
        line += buffer[i];
      }
    }
  }
  void Write(string  message){
    message += '\0';
    //cout<<message;
    char* msg = new char[message.length()+1];
    strcpy( msg, message.c_str());
    int len = message.length();

    int bytes_sent = 0;//send(sock, msg, len, 0);
    while(bytes_sent != len){

      bytes_sent += send(sock, msg + bytes_sent, len-bytes_sent, 0);
      //cout<<bytes_sent<<" "<<len<<endl;
      if(bytes_sent == -1)
        perror("Write/whilesend");
    }

  }
  string getIP(){

    return local_ip;

  }
  int getPort(){
    return local_port;
  }
  int print_fd(){
    return sock;
  }
  string get_des_IP(){
	socklen_t len = 32;
	char *buf = new char[32];
	
	struct sockaddr_in res;
	if(getpeername(sock, (sockaddr*)buf, &len) == -1)
		perror("get_des_port");
	sockaddr *addr = (sockaddr*)buf;
	if (addr->sa_family == AF_INET)
		return inet_ntoa( ((sockaddr_in*)addr)->sin_addr );
	else{
		char *ip6 = new char[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &((sockaddr_in6*)addr)->sin6_addr, ip6, INET6_ADDRSTRLEN);
		return ip6;
	}
  }
  
  int get_des_port(){
	struct sockaddr_in res;
	socklen_t len = sizeof(res);
	if(getpeername(sock, (sockaddr*)&res, &len) == -1)
		perror("get_des_port");
	return (int) ntohs(res.sin_port);
	
	
  }

  void Close(){
    close(sock);
  }
  ~Socket(){

  }

};

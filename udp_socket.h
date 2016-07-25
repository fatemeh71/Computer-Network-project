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
#include <strings.h>
#define buffer_len 100
using namespace std;
class UDP_Socket{
  int sock;
  char* buffer;
  int buff_head, buff_filled;
  struct sockaddr_in my_addr, other_addr;
  string local_ip;
  int local_port;
  public:
  UDP_Socket(string ip_address, int port){//called by client
    sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
      perror("UDP_Socket");
    bzero(&(other_addr), sizeof(other_addr));
    other_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address.c_str(), &other_addr.sin_addr);
    other_addr.sin_port = htons(port);
    buff_head = 0;
    buff_filled = 0;
    buffer = new char [buffer_len];
	
    //struct sockaddr_in local_address;
    socklen_t address_len = sizeof(my_addr);
	if (getsockname(sock, (struct sockaddr*)& my_addr, &address_len) == -1)
		perror("udp_socket_constructor");
    
	local_ip = inet_ntoa(my_addr.sin_addr);
    local_port = (int) ntohs(my_addr.sin_port);

	
  }
  UDP_Socket(int server_port){ //called by server
    sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
      perror("UDP_Socket");
    bzero(&(my_addr),sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    my_addr.sin_port = htons(server_port);
    if( bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr) ) == -1)
      perror("UDP_Socket/bind");
    buff_head = 0;
    buff_filled = 0;
    buffer = new char [buffer_len];
	local_ip = inet_ntoa(my_addr.sin_addr);
    local_port = server_port;
  }

  string Read_Line(){
    string line;
    socklen_t len = sizeof(other_addr);
    for(int i= buff_head; i < buff_filled; i++){
	  if(buffer[i] == '\0'){
        buff_head = i+1;
        return line;
      }
      line += buffer[i];
    }
    while (true){
      buff_head = 0;
      buff_filled = recvfrom(sock, buffer, buffer_len, 0, (struct sockaddr *)&other_addr, &len);
      if(buff_filled == -1)
        perror("Read_line/ recv");

      for(int i= buff_head; i < buff_filled; i++){
		if(buffer[i] == '\0'){
          buff_head = i+1;
          return line;
        }
        line += buffer[i];
      }
    }

  }
  void Write( string message){
    message += '\0';
	//cout<<"message is ::: "<<message<<endl;
    char* msg = new char[message.length()+1];
    strcpy( msg, message.c_str());
    int len = message.length();

    int bytes_sent = 0;//send(sock, msg, len, 0);
    while(bytes_sent != len){
      bytes_sent += sendto(sock, msg, len, 0 , (struct sockaddr*)&other_addr, sizeof(other_addr));
      if(bytes_sent == -1 )
        perror("UDP_Socket/Write/ send to");

    }
  }
  void Close(){
    close(sock);
  }
  string getIP(){
  socklen_t address_len = sizeof(my_addr);
	if (getsockname(sock, (struct sockaddr*)& my_addr, &address_len) == -1)
		perror("udp_socket_constructor");
    
	local_ip = inet_ntoa(my_addr.sin_addr);
  return local_ip;}
  int getPort(){
  return local_port;}
  int get_des_port(){
	return (int) ntohs(other_addr.sin_port);
	
  }



};

#include "ServerSocket.h"
#include "UDP_Socket.h"
#include <iostream>
using namespace std;

int main(){
	
	ServerSocket serv("3499");
	Socket new_sock =serv.Accept();
	cout<<new_sock.Read_Line();
	UDP_Socket udp_sock(3496);
	cout<<udp_sock.Read_Line();
	cout<<udp_sock.Read_Line();
	cout<<udp_sock.Read_Line();
	
	
	
return 0;}

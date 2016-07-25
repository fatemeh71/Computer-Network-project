#include "Socket.h"
#include "UDP_Socket.h"

int main(){
	Socket client("127.0.0.1", "3499");
	client.Write("salam\nchetori\n");
	client.Close();
	UDP_Socket client2("127.0.0.1", 3496);
	client2.Write("aleyk\nhal e shoma? \n");
	client2.Write("aleyk2222\n");
	//client.Write("chetori\n");

return 0;}

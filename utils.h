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
#include <sstream>
#define buffer_len 100
#define serverPort "21276"
#include <algorithm>
using namespace std;
class network_utilities{
  public:
  static string getHostName(string name){
    struct hostent* host_address = gethostbyname(name.c_str());
    int index, size;
    struct in_addr addr;
    size =0;
    for (int i=0; host_address->h_addr_list[i] != NULL; i++)
      size++;
    index = rand() % size;
    addr.s_addr = *(u_long *) host_address->h_addr_list[index];
    string ip = inet_ntoa(addr);
    return ip;
  }

};
string int_to_string(int in){
	string res = "";
  if (in == 0)
		return "0";
	
	while(in>0){
		res += char('0' + in%10);
		in /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}
int string_to_int (string in){
	int res;
	stringstream ss;
	ss<<in;
	ss>>res;
	return res;


}

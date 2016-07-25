#include "udp_socket.h"
#include "utils.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
using namespace std;

struct insurance{
    string insurance_type;
    int cost;
	
};

class doctor{
  UDP_Socket* udp_sock;
  vector <insurance> insurances;
  string name;
public:
  doctor(string inputFilename, int port_number, string n){
	name = n;
    string line,type;
    int cost;
    ifstream fin(inputFilename.c_str());

    while( getline(fin, line) ){

      insurance in;
      stringstream ss(line);
      if (!(ss >> type >> cost))
		break;
      in.insurance_type = type;
      in.cost = cost;
	  //cout<<"insurance is "<<in.insurance_type<<" "<<in.cost<<endl;
      insurances.push_back(in);
    }
    udp_sock = new UDP_Socket (port_number);
	cout<<"Phase 3: "<<name<<" has a static UDP port "<< port_number<< " and IP address "<<udp_sock->getIP()<<"."<<endl;
  }
  void Run (){
  while(true){
    string str = udp_sock->Read_Line();
	cout<<"Phase 3: "<<name<<" receives the request from the patient with port number "<<udp_sock->get_des_port()<<" and insurance plan "<<str<<"."<<endl;
	
    for (int i=0; i < insurances.size(); i++){
      if(insurances[i].insurance_type == str){

        udp_sock->Write(int_to_string(insurances[i].cost));
		cout<<"Phase 3: "<<name<<" has sent estimated price "<<insurances[i].cost<<"$ to patient with port number "<<udp_sock->get_des_port()<<"."<<endl;

      }
    }
	}
  }
};

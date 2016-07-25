#include "serversocket.h"
#include "udp_socket.h"
#include "utils.h"
#include <fstream>
#include <istream>
#define HOST_URL "nunki.usc.edu"
class patient{
public:
  string NAME;
  string user_name;
  string password;
  string insurance_type;
  Socket* tcp_socket;
  UDP_Socket* udp_socket;
  //string insurance;
  patient(string filename, string insurance_file, string n){
	NAME = n;
    string line;
    ifstream fin(filename.c_str());
    getline(fin, line);
    user_name = line.substr(0, line.find(' '));
    password = line.substr(line.find(' ')+1 );
    //cout<<user_name<<" "<<password<<endl;
    fin.close();

    string ip = network_utilities::getHostName(HOST_URL);
    //cout<<"got nunki address"<<endl;
    //cout<<ip<<" "<<serverPort;
    tcp_socket= new Socket(ip, serverPort);
    ifstream ins_fin(insurance_file.c_str());
    getline(ins_fin, insurance_type);
    cout<<"Phase 1: "<<NAME<<" has tcp port number"<<tcp_socket->getPort()<<" and IP address "<<tcp_socket->getIP()<<"."<<endl;

  }
  int authenticate(){

    string authenticate_message = "authenticate ";
    authenticate_message += user_name;
    authenticate_message += " ";
    authenticate_message += password;
    tcp_socket->Write(authenticate_message);
    cout<<"Phase1: Authentication request from "<<NAME<<" with username "<<user_name<< " and password "<<password<<" has been sent to the Health Center Server."<<endl;
    string res = tcp_socket->Read_Line();
	cout<<"Phase 1: "<<NAME<<" authentication result: "<<res<<"."<<endl;
    if(res == "Failure" )
      return 0;
    else if(res == "Success")
      return 1;
    else{
      perror(res.c_str());
      return 0;
    }

  }
  
  void  connect_to_doctor(string ip, string port, string doc){
	//cout<<"insurance is ::::"<<insurance_type<<endl;
    udp_socket = new UDP_Socket(ip, string_to_int(port));
	udp_socket->Write(insurance_type);
	cout<<"Phase 3: "<<NAME<<"has a dynamic UDP port number "<<udp_socket->getPort()<<" and IP address "<<udp_socket->getIP()<<"."<<endl;
	cout<<"Phase 3: The cost estimation request from "<<NAME<<"with insurance plan "<<insurance_type<<" has been sent to the doctor with port number "<<port<< " and IP address "<<ip<<"."<<endl;
	
		
    string cost_ = udp_socket->Read_Line();
	cout<<"Phase 3: "<<NAME<<" receives "<<cost_<<"$ estimation cost from doctor with port number "<<port<<" and name "<<doc<<endl;
	cout<<"Phase 3: End of Phase 3 for "<<NAME<<"."<<endl;
	udp_socket->Close();
  }


  void request_availabilities(){
  string request_message = "available";
  tcp_socket->Write(request_message);
  string response;
  response = tcp_socket->Read_Line();
  size_t currTab =0;
  size_t nextTab = response.find('\t');
  string description;
  cout<<"The following appointments are available for "<<NAME<<":"<<endl;
  while ( nextTab < string::npos ){
    description = response.substr(currTab, nextTab-currTab);
    cout<<description<<endl;
    currTab = nextTab+1;
    nextTab = response.find('\t',currTab+1);

  }
  char selection;
  cout<<"Please enter the preferred appointment index and press enter:"<<endl;
  cin>>selection;
  string selection_message = "selection ";
  selection_message += selection;
  tcp_socket->Write(selection_message);
  response = tcp_socket->Read_Line();
  if(response == "reserved"){
	tcp_socket->Close();
	cout<<"Phase 2: The requested appointment from "<<NAME<<" is not available.  Exiting..."<<endl;
	return;
  }
  //cout << "RECEIVING " << response << endl; 
  string doc = response.substr(0, response.find(' '));
  string udp_port = response.substr(response.find(' ')+1);
  cout<<"Phase 2: The requested appointment is available and reserved to "<<NAME<<". The assigned doctor port number is "<<udp_port<<"."<<endl;
  connect_to_doctor (tcp_socket->get_des_IP(), udp_port, doc);
  tcp_socket->Close();
  }
};

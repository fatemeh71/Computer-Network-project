#include "serversocket.h"
#include "udp_socket.h"
#include <vector>
#include <fstream>
#include <istream>
#include "utils.h"
#include <pthread.h>
struct user {
  string user_name;
  string password;
};
struct time_slot{
  int index;
  string dsc;
  string doctor_name;
  string doctor_port;
  bool reserved;

};
class healthcenterserver{

  ServerSocket* tcp_socket;
  vector <user> users;
  vector <time_slot> available_times;
  vector <pthread_t> threads;
  pthread_mutex_t lock;
  public:
  healthcenterserver();
  void Run();
  string* authenticate(string);
  void* thread_method(Socket* );
  static void* thread_run(void* );
  void initialize_users();
  void initialize_availabilities();
  string get_available();
};
struct thread_args{
  Socket* ns;
  healthcenterserver * hcs;

};
  healthcenterserver::healthcenterserver(){
    initialize_users();
    initialize_availabilities();
    tcp_socket = new ServerSocket(serverPort);
	pthread_mutex_init(&lock, NULL);
	cout<<"Phase 1: The Health Center Server has port number "<<serverPort<<" and IP address "<<tcp_socket->getIP()<<"."<<endl;
  }
  void healthcenterserver::Run(){
    while(true){
      //cout<<"DAMN: "<<tcp_socket->sock<<endl;
      Socket new_sock = tcp_socket->Accept();
      pthread_t t;
      struct thread_args args;
      threads.push_back(t);
      args.ns = & new_sock;
      args.hcs = this;
      //cout<<"file descriptor from Run"<<new_sock.print_fd()<<endl;
      pthread_create(&t, NULL, thread_run, (void*)&args);
    }
  }
  string* healthcenterserver::authenticate(string cmd){
    size_t first = cmd.find(' ');
    size_t second = cmd.find(' ', first+1);
    string command, usr, pwd;
    command = cmd.substr(0, first);
    usr = cmd.substr(first + 1, second-first-1);
    pwd = cmd.substr(second+1);
    
    if (command != "authenticate")
      return NULL;
    else {
		cout<<"Phase 1: The Health Center Server has received request from a patient with username "<<usr<<" and password "<<pwd<<"."<<endl;
		for (int i=0; i < users.size() ; i++){
			if(usr == users[i].user_name && pwd == users[i].password){
				
				cout<<"Phase 1: The Health Center Server sends the response Success to patient with username "<<usr<<"."<<endl;
				return new string(usr);
				
				}
      }
    }
	
		cout<<"Phase 1: The Health Center Server sends the response Failure to patient with username "<<usr<<"."<<endl;
    return NULL;
  }
  void* healthcenterserver::thread_method(Socket* ns){
    Socket* new_socket = ns;
    //cout<<"file descriptor from thread_method"<<new_socket->print_fd()<<endl;
    string msg = new_socket->Read_Line();
    //cout<<msg<<endl;
	
	string *username;
    if( (username=authenticate(msg)) == NULL ){
      new_socket->Write("Failure");
      new_socket->Close();
      pthread_exit(NULL);
    }
    new_socket->Write("Success");
  /*************************Phase 1 finished******************************/
  msg = new_socket->Read_Line();
  if ( msg == "available" ){
	cout<<"Phase 2: The Health Center Server receives a request for available time slots from patients with port number "<< new_socket->get_des_port()<<" and IP address "<<new_socket->get_des_IP()<<"."<<endl;
    new_socket->Write(get_available());
  }
  else;
  msg = new_socket->Read_Line();
  size_t space = msg.find(' ');
  string cmd = msg.substr(0, space);
  int selected;
  if ( cmd == "selection"){
    selected = msg[space +1] - '0';
	
	pthread_mutex_lock(&lock);
    //cout<<"Selected time slot is : "<<selected<<endl;
	
	cout<<"Phase 2: The Health Center Server receives a request for appointment "<<selected<<" from patient with port number "<<new_socket->get_des_port()<<" and username "<<(*username)<<"."<<endl;
    
	if( !available_times[selected-1].reserved ){
		cout<<"Phase 2: The Health Center Server confirms the following appointment, "<<selected<<" to patient with username "<<(*username)<<"."<<endl;
      available_times[selected-1].reserved = 1;
      /********************Phase 2 finished*********************************/
	  string s = available_times[selected-1].doctor_name;
	  s += " ";
	  s += available_times[selected-1].doctor_port;
      //cout << "SENDING " << s << endl;
	  new_socket->Write(s);
    }
	else cout<<"Phase 2: The Health Center Server rejects the following appointment, "<<selected <<" to patient with username "<<(*username)<<"."<<endl;
	pthread_mutex_unlock(&lock);
  }
    pthread_exit(NULL);
  }
  void* healthcenterserver::thread_run(void* args){
    struct thread_args * t_args = (struct thread_args *) args;
    //cout<<"file descriptor from thread_run: "<<t_args->ns->print_fd()<<endl;
    t_args->hcs->thread_method(t_args->ns);
    pthread_exit(NULL);
  }

  void healthcenterserver::initialize_users(){
    string line;
    fstream fin ("users.txt");
    while( getline(fin, line ) ){

      struct user new_user;
      new_user.user_name = line.substr(0, line.find(' '));
      new_user.password = line.substr(line.find(' ')+1);
      users.push_back(new_user);
    }

  }
  void healthcenterserver::initialize_availabilities(){
  fstream avail_file("availabilities.txt");
  string line;
  while(getline(avail_file, line)){
    struct time_slot ts;
    int first = line.find(' ');
    int second = line.find(' ', first+1);
    int third = line.find(' ', second+1);
    int fourth = line.find(' ', third+1);
    ts.index = string_to_int( line.substr(0, first) );
	ts.dsc = line.substr(first+1, third-first-1);
    ts.doctor_name = line.substr(third+1, fourth-third-1);
    ts.doctor_port = line.substr(fourth+1);
    ts.reserved = false;
    //cout<<ts.doctor_name<<ts.doctor_port<<endl;
    available_times.push_back(ts);

  }
    //cout<<"hi from availabilities"<<endl;
  }
  string healthcenterserver::get_available(){
	string str;
	pthread_mutex_lock(&lock);
	for( int i=0; i < available_times.size(); i++ ){
		if( !available_times[i].reserved ){
			str += int_to_string(available_times[i].index);
			str += " ";
			str +=available_times[i].dsc;
			str += '\t';
			//cout<<"Sending "<<str<<endl;
		}
	}
	pthread_mutex_unlock(&lock);
	return str;
  }

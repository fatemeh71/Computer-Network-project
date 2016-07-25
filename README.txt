Fatemeh Aghamohammadi 3242 2182 76
************************************************
To run the project, put all files in the same directory, compile with g++ as described in the project, run !
just remember that healthcenterserver and doctor must run before client
************************************************
run the server, 
run the doctors, 
run the patient(s), the patients will automatically ask for the available times from the server
choose one of the time slots
the patients will automatically connect to the doctors and get the cost estimaation and desplay it.
************************************************
In this project I used Unix socket programming to connect to write a client/server program. 
I used Pthreads to implement multithreading in the server and mutexes to avoid race condition when more that one thread
are using the same data.
************************************************
project files: 

socket.h
serversocket.h
udp_socket.h
utils.h
patient.h
healthcenterserver.h
doctor.h
patient1.cpp
patient2.cpp
healthcenterserver.cpp
doctor1.cpp
doctor2.cpp
***************************************************
Reused codes:

server socket constructor function in serversocket.h —> from beej 
socket constructor function  in socket.h———————> from beej
socket Read_Line and Write functions  in socket.h——> from beej
udp_socket constructor and Read_Line and Write ——> partly from http://www.binarytides.com/programming-udp-sockets-c-linux/

***************************************************
Note: in the on screen messages in server, IPv6 addresses are printed.
server and doctor return 0.0.0.0 as the local ip address.

***************************************************
file description: 


socket.h
In socket.h, I defined a new class Socket to make working with sockets easier. It provides two constructor and some 
functions like Read_Line(), Write(). This socket has 2 constructors. One constructor is used by the client when it wants 
to connect to a specific ip address and port number. the other is used by the server, for the socket returned by accept() 
function when a client connects to the server.


serversocket.h
In this file, I provided a class(ServerSocket) for listening socket used by server. this socket is bind()ed to a specific 
port number and listen()s to incoming connections. this socket  also accepts connection. and makes a new socket (of type 
Socket) when it accept()s a connection.

udp_socket.h
In this file, I provided an interface to work with UDP sockets. this socket is used both for server side and client side,
 but it has different constructors for each of them.

healthcenterserver.h
it provides the healthcenterserver functionalities like initializing user list, available times, doctor lists,.. 
in the Run function, in an infinite while loop, it listens for incoming patients. when patients connect to the server, 
it creates a new thread to work with the patient. the new thread runs thread_method function. 
in thread_methos function, it authenticates the patient, send availablities to patients, receives the patient's selection,
reserves the available time and sends the doctor port number and name to the patient.
Note: I used mutex variable to avaoid problems that might cause by two threads accessing the same data (available times lists.)

doctor.h
this file contains the class for doctor. it has a udp socket. when patient sends a message to this socket, doctor generates
the appropriate answer and sends it back to the client.

patient.h
this file contains the class for patient. patient class has a tcp socket to connect to health center server and a udp 
socket to connect to doctor. This class implements the functionalities of the client like initalizing user name, password,
and insurance type, authenticating the patient to the server, asking for available times, displaying times to the user,  sending the selected time to the 
server, receiving doctor port.
when doctor port was received, this class sends the request to the doctor and receives the doctor's response.

healthcenterserver.cpp
it runs the server with the appropriate input files like users.txt, availabilities.txt

patient1.cpp and patient2.cpp
creates two patients with appropriate names from appropriate files;

doctor1.cpp and doctor2.cpp
creates doctors with the appropriate port numbers from doctor1.txt and doctor2.txt

utils.h
it contains some useful functions


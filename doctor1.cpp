#include "doctor.h"
#define doctor_port 41276

int main(){

    doctor doc1("doc1.txt",doctor_port, "Doctor 1" );
    doc1.Run();



  return 0;}

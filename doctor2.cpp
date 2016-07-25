#include "doctor.h"
#define doctor_port 42276

int main(){

    doctor doc1("doc2.txt",doctor_port, "Doctor 2" );
    doc1.Run();



  return 0;}

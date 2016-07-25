#include "patient.h"
int main(){
  patient patient2("patient2.txt", "patient2insurance.txt", "Patient 2");
  
  if( !patient2.authenticate())
    return 0;
  patient2.request_availabilities();

return 0;}

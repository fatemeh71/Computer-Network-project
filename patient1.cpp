#include "patient.h"
int main(){
  patient patient1("patient1.txt", "patient1insurance.txt", "Patient 1");
  
  if( !patient1.authenticate())
    return 0;
  patient1.request_availabilities();

return 0;}

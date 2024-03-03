
#include <cvzone.h>

SerialData serialData;

int sendVals[2]; // Array of values to send

void setup() {

  serialData.begin(9600);

}

void loop() {

  int potVal1 = analogRead(A0);
  int potVal2 = analogRead(A1);
  sendVals[0] = potVal1;
  sendVals[1] = potVal2;
  
  serialData.Send(sendVals);
  
}

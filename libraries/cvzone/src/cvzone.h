/*
Computer Vision Zone Serial Library
For communicating with python for Computer Vision Applications.
https://www.computervision.zone/
Created by Computer Vision Zone.
Released under the MIT License
*/


#ifndef cvzone_h     // avoid including it twice 
#define cvzone_h

// import arduino library
#include "Arduino.h"


class SerialData
{
  private:
    int _numOfValsSend;
    String _sendString;
    bool _counterStart = false;
    int _counter = 0;
    String _receivedString;
    int _stringLength=0;
    int _numOfValsRec;
    int _digitsPerValRec;

  public:
    void begin(int baudRate = 9600);
    SerialData(int numOfValsRec=1, int digitsPerValRec=1);
    void Send(int sendVals[]);
    void Get(int* valsRec);

};

#endif

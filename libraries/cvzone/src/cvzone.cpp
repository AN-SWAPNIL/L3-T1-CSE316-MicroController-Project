/*
Computer Vision Zone Serial Library 
For communicating with python for Computer Vision Applications.  
https://www.computervision.zone/
Created by Computer Vision Zone.
Released under the MIT License
*/

#include "cvzone.h"

SerialData::SerialData(int numOfValsRec, int digitsPerValRec) {

  _numOfValsRec = numOfValsRec;
  _digitsPerValRec = digitsPerValRec;
  _stringLength = _numOfValsRec * _digitsPerValRec + 1;

}

void SerialData::begin(int baudRate) {
  Serial.begin(baudRate);
}

void SerialData::Send(int sendVals[]) {

  String _sendString;
  for (int i = 0; i < sizeof(sendVals); i++)
  {
    _sendString += String(sendVals[i]) + "#";

  }
  Serial.println(_sendString);
}


void SerialData::Get(int* valsRec)
{

  while (Serial.available()) {

    char c = Serial.read();

    if (c == '$') {
      _counterStart = true;
    }
    if (_counterStart) {
      if (_counter < _stringLength) {
        _receivedString = String(_receivedString + c);
        _counter++;
      }

      if (_counter >= _stringLength) {
        for (int i = 0; i < _numOfValsRec; i++)
        {
          int num = (i * _digitsPerValRec) + 1;
          valsRec[i] = _receivedString.substring(num, num + _digitsPerValRec).toInt();
        }
        _receivedString = "";
        _counter = 0;
        _counterStart = false;
      }
    }
  }

}

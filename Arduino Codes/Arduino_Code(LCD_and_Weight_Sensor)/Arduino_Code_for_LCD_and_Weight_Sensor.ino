/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711_ADC.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout = 6; //mcu > HX711 dout pin
const int HX711_sck = 7; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
int isWeight = 0;
int isStarted = 0;

void setup() {
  
  pinMode(2, OUTPUT); //LSB
  pinMode(4, OUTPUT); //MSB
  pinMode(8, OUTPUT);
  
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(8, LOW);
  
  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
  // initialize the LCD
  lcd.begin();
  // // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0); // set cursor to first row
  lcd.print("Weight[g]:"); // print out to LCD
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      // Serial.print("Load_cell output val: ");
      // Serial.println(i);
      lcd.setCursor(0, 1); // set cursor to second row
      lcd.print(i); // print out the retrieved value to the second row
      if(i>=20.00&&!isWeight) {
        isWeight = 1;
        digitalWrite(8, HIGH);
      } else if(i<20.00) {
        isWeight = 0;
        digitalWrite(8, LOW);
      }
      newDataReady = 0;
      t = millis();
    }
  }

  ////////////////

  if (Serial.available() > 0) {
    String fruit = Serial.readString();
    if (fruit == "orange" ) {
      digitalWrite(2, HIGH);
      digitalWrite(4, LOW);
    }
    else if (fruit == "apple") {
      digitalWrite(2, LOW);
      digitalWrite(4, HIGH);
    }
    else if (fruit == "banana" ) {
      digitalWrite(2, HIGH);
      digitalWrite(4, HIGH);
    }
    else {
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
    }
    Serial.print(" ");
  }
  
  delay(100);

}
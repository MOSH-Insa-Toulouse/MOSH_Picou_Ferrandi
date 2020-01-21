// RN2483_temperature.ino - Simple LoRaWAN temperature sensor using Microchip RN2483 module
#include "SoftwareSerial.h"
#include "gasCom.h"
#include <TheThingsNetwork.h>



SoftwareSerial loraSerial(10,11); //Rx, Tx
#define debugSerial Serial
#define freqPlan TTN_FP_EU868
float R0;
int buttonState = 0;
const int buttonPin = 2; 
const int ledPin =  3; 
const int buttonAlarmPin = 4;

boolean alarmDisabled =  false;


TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan );

void  convertToByteArrays(int value, unsigned char* arrayByte){
  union {
    int a;
    unsigned char bytes[2];
  } thing;
  thing.a = value;
  memcpy(arrayByte,thing.bytes,2);
  
}

void message(const byte* payload, size_t length, port_t port) {
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(length) + " bytes on port " + String(port) + ":");
    
  for (int i = 0; i < length; i++) {
    debugSerial.print(" " + String(payload[i]));
  }
    
  debugSerial.println();
}


void setup()
{
  debugSerial.begin(57600);
  loraSerial.begin(57600);
  debugSerial.println("RN2483 Test");
  const char *devAddr = "2601160F";
  const char *nwkSKey = "5E7DE6B986D2206F86B95ECDCBCF8AF4";
  const char *appSKey = "1286128B17C38E88E4F86CC7C19586A7";

  ttn.onMessage(message);
  ttn.personalize(devAddr, nwkSKey, appSKey);
  R0 = gasCalibration();  

}

void loop() {

    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);

    float ratio = gasMesure(R0);

    if(ratio < 20 ) digitalWrite(ledPin,HIGH);
    else digitalWrite(ledPin,LOW);


  if (buttonState == HIGH) {
    unsigned char ratioB[2];
    convertToByteArrays(floor(ratio), ratioB);
    ttn.sendBytes(ratioB, sizeof(ratioB));
  } else {

  }


  //ttn.poll();
  
  //delay(7000);
   

}

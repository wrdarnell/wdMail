// File: Transmitter.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"

const int  sensorPin      =       2; // Number of pin to listen to
const long debouncingMs   =     250; // Milliseconds to wait for debounce
const int  dupAlertWaitS  =       1; // Seconds to wait until firing another alert

volatile          int  sensorState = 0;
volatile unsigned long lastSensorFireMs;
volatile unsigned long lastAlertMs;

RF24 radio(7, 8); // CE, CSN

void setup() {
  // See how much can be moved into alert code to reduce power usage
  radio.begin();
  radio.openWritingPipe(pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.stopListening();
  radio.powerDown();
  pinMode(sensorPin, INPUT);
  attachInterrupt(0, debounceInterrupt, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void debounceInterrupt() {
  if ((long)(millis() - lastSensorFireMs) >= debouncingMs) {
    onSensorChange();
    lastSensorFireMs = millis();
  }
}

void onSensorChange() {
   if (((long)millis() - lastAlertMs) >= (dupAlertWaitS * 1000)) {
      sendAlert();
      lastAlertMs = millis(); 
   }
}

void sendAlert() {
  sendMessage(mailMessage);
}

void sendMessage(const char* msg) {
  radio.powerUp();
  radio.write(msg, strlen(msg));
  radio.powerDown();
}

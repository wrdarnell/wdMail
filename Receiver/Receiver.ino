// File: Receiver.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"

const int ledPin = LED_BUILTIN;

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Setup Onboard LED
  pinMode(ledPin, OUTPUT); // Setup the on-board LED

  // Warm up serial port
  Serial.begin(9600);
  while (!Serial) {;}
  
  // Setup Radio
  radio.begin();
  radio.openReadingPipe(0, pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.startListening();
}

void loop() {
  CheckForRadioMessage();
}

void CheckForRadioMessage() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    FlashLED();
    NotifySerial(text);
  }
}

void FlashLED() {
  int i;
  for (i=0;i<5;i++) {
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
    delay(50);
  }
}

void NotifySerial(char* message) {
  Serial.write(message);
}

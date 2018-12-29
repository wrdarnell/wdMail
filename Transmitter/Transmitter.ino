// File: Transmitter.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"

const int  sensorPin      =       2; // Number of pin to listen to
const long debouncingMs   =     250; // Milliseconds to wait for debounce
const int  dupAlertWaitS  =       1; // Seconds to wait until firing another alert
const int  heartbeatS     =     30 ; // Seconds between heartbeats.  0 for no heartbeats

volatile          int  sensorState = 0;
volatile unsigned long lastSensorFireMs;
volatile unsigned long lastAlertMs;
volatile unsigned long lastHeartbeat;

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Radio Setup
  radio.begin();
  radio.openWritingPipe(pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.setDataRate(radioDataRate);
  radio.stopListening();
  radio.powerDown();

  // Setup an interrupt to watch the sensor pin
  pinMode(sensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), debounceInterrupt, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  heartbeat();
}

void debounceInterrupt() {
  if ((long)(millis() - lastSensorFireMs) >= debouncingMs) {
    onSensorChange();
    lastSensorFireMs = millis();
  }
}

void onSensorChange() {
   if (((long)millis() - lastAlertMs) >= (dupAlertWaitS * 1000)) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      sendMessage(mailMessage);
      
      lastAlertMs = millis(); 
   }
}

void sendMessage(const char* msg) {
  radio.powerUp();
  radio.write(msg, strlen(msg));
  radio.powerDown();
}

void heartbeat() {
  if (heartbeatS && (((long)millis() - lastHeartbeat) > (heartbeatS * 1000))) {
    sendMessage(heartbeatMessage);
    lastHeartbeat = millis();
  }
}

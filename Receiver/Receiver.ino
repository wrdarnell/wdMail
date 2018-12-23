// File: Receiver.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"

const int    statusLedPin = 2;
const int     alertLedPin = 3;
const int heartBeatLedPin = 4;
const int   alertResetPin = 5;

const int alertDelayS  = 5;

volatile unsigned long lastAlert;
volatile unsigned long lastStatus;
volatile unsigned long lastReset;
volatile           int alertOn;

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Setup LED Pins
  pinMode(alertLedPin,     OUTPUT);
  pinMode(statusLedPin,    OUTPUT);
  pinMode(heartBeatLedPin, OUTPUT);
  pinMode(alertResetPin,   INPUT );

  // Warm up serial port
  Serial.begin(9600);
  while (!Serial) {;}
  
  // Setup Radio
  radio.begin();
  radio.openReadingPipe(0, pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.startListening();

  // Defaults
  alertOn = 0;
}

void loop() {
  WatchResetButton();
  CheckForRadioMessage();
  AlertLED();
  SteadyStateLED();
}

void WatchResetButton() {
  // Watches the reset button.  If pushed, clear alert status and flash all LEDs
  if ((((long)millis() - lastReset) > 2000) && digitalRead(alertResetPin)) {
    alertOn = 0;
    digitalWrite(alertLedPin,     HIGH);
    digitalWrite(statusLedPin,    HIGH);
    digitalWrite(heartBeatLedPin, HIGH);
    delay(250);
    digitalWrite(alertLedPin,     LOW);
    digitalWrite(statusLedPin,    LOW);
    digitalWrite(heartBeatLedPin, LOW);
    lastReset = millis();
  }
}

void CheckForRadioMessage() {
  if (radio.available()) {
    HeartBeatLED(); // Flash for radio traffic
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (!strcmp(text, mailMessage)) {
      alertOn = 1;
    }
    NotifySerial(text);
  }
}

void SteadyStateLED() {
  if (Serial && ((long)millis() - lastStatus) > 3000) {
    digitalWrite(statusLedPin, HIGH);
    delay(5);
    digitalWrite(statusLedPin, LOW);
    lastStatus = millis(); 
  }
}

void AlertLED() {
  int i;
  if (alertOn && ((long)millis() - lastAlert) > (alertDelayS * 1000)) {
    for (i=0;i<10;i++) {
      digitalWrite(alertLedPin, HIGH);
      delay(25);
      digitalWrite(alertLedPin, LOW);
      delay(25);
      lastAlert = millis();
    }
  }
}

void NotifySerial(char* message) {
  char buf[51];
  sprintf(buf, "%s\n", message);
  Serial.write(buf);
}

void HeartBeatLED() {
  digitalWrite(heartBeatLedPin, HIGH);
  delay(250);
  digitalWrite(heartBeatLedPin, LOW);  
}

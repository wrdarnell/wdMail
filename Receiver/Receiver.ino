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

volatile unsigned long lastStatus;
volatile unsigned long lastReset;

volatile LED_BLINKER alertBlinker;

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Setup LED Pins
  pinMode(statusLedPin,    OUTPUT);
  pinMode(heartBeatLedPin, OUTPUT);
  pinMode(alertResetPin,   INPUT );

  initBlinker(&alertBlinker, alertLedPin, 10, 25, 5);

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
  watchResetButton();
  checkForRadioMessage();
  blinkLED(&alertBlinker);
  steadyStateLED();
}

void watchResetButton() {
  // Watches the reset button.  If pushed, clear alert status and flash all LEDs
  if ((((long)millis() - lastReset) > 2000) && digitalRead(alertResetPin)) {
    stopBlinker(&alertBlinker);
    
    digitalWrite(statusLedPin,    HIGH);
    delay(250);
    digitalWrite(statusLedPin,    LOW);

    notifySerial((char*)"Reset Button Pressed");
    lastReset = millis();
  }
}

void checkForRadioMessage() {
  if (radio.available()) {
    radioLED();
    
    char text[32] = "";
    radio.read(&text, sizeof(text));
    
    if (!strcmp(text, mailMessage)) {
      startBlinker(&alertBlinker);
    }
    
    notifySerial(text);
  }
}

void steadyStateLED() {
  if (Serial && ((long)millis() - lastStatus) > 3000) {
    digitalWrite(statusLedPin, HIGH);
    delay(5);
    digitalWrite(statusLedPin, LOW);
    lastStatus = millis(); 
  }
}

void notifySerial(char* message) {
  char buf[51];
  sprintf(buf, "%s\n", message);
  Serial.write(buf);
}

void radioLED() {
  digitalWrite(heartBeatLedPin, HIGH);
  delay(5);
  digitalWrite(heartBeatLedPin, LOW);  
}

void initBlinker(volatile LED_BLINKER* led, int pin, int burst, int freq, int delay) {
  pinMode(pin, OUTPUT);
  led->pin     = pin;
  led->burst   = burst;
  led->freq    = freq;
  led->delay   = delay;
  led->enabled = 0;  
}

void blinkLED(volatile LED_BLINKER* led) {
  int state = digitalRead(led->pin);
  if (!led->enabled) {
    if (state)
      digitalWrite(led->pin, LOW);
  } else {
    unsigned long now = millis();
    if (now > led->next) {
      if (state) {
        digitalWrite(led->pin, LOW);
        if (led->count > led->burst) {
          led->next = now + (led->delay * 1000);
          led->count = 0;
        } else {
          led->next = now + led->freq;
        }
      } else {
        digitalWrite(led->pin, HIGH);
        (*led).count++;
        led->next = now + led->freq;        
      }
    }
  }
}

void startBlinker(volatile LED_BLINKER* led) {
  led->enabled = 1;
  led->count   = 0;
  led->next    = millis();
}

void stopBlinker(volatile LED_BLINKER* led) {
  led->enabled = 0;
}

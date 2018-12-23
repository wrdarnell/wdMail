// File: wdMail.h
// Auth: Will Darnell
// Date: 19-DEC-2018

const byte pipeAddress[6]     = "11977"; // Transmit pipe
const char mailMessage[]      = "You've Got Mail!";
const char heartbeatMessage[] = "Heartbeat";
const int  radioPowerLevel    = RF24_PA_MIN;

struct LED_BLINKER {
	int           pin;	 	// Pin LED is tied to
	int           burst;	// Number of on cycles per burst
	int           freq;		// Burst frequency (ms)
	int           delay;	// Seconds between bursts
	int           enabled;	// True if we're blinking
	int           count;	// On cycles so far this burst
	unsigned long next;		// Next event
};
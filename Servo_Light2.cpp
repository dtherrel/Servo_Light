// Servo_Light.cpp - Implementation of main program.
// Servo finds brightest light via the photoresistor and moves to point towards it.
// By Dylan Herrel and Matthew Hirsch
// CSC 113 - Assignment 3
// 10/24/17

#include <Arduino.h>
#include <Servo.h>
#include "ServoController.h"

ServoController servoCon;  // Create ServoController object to control a Servo
int buttonPin = 2; 		   // Input pin used to read button
int period = 0; 	       // Default value for period between scans (value is chosen by user)
bool valid_period = false; // Boolean used to test if user's period is valid
bool firstTime = true;	   // Boolean used to check if it's the first time running
int delayCount = 0;		   // Variable to count time passing (used to run the periodic scans)

void setup() {
	Serial.begin(115200);			  // Set the data rate for serial data transmission
	pinMode(buttonPin, INPUT_PULLUP); // Assigns digital pin as input for button input
	servoCon.myServo.attach(11); 	  // Assigns pin to Servo

	// While the user inputs invalid information, keep asking them for a valid input until they do.
	while(!valid_period){
		// Ask the user for the period between each scan
		Serial.println("How long is the period between each scan (in ms)?");
		while(Serial.available() == 0){} // Wait for the user to enter a value

		// Read as input the user's chosen period between scans
		String period_str = Serial.readString();
		period = period_str.toInt(); // Convert the input string to an integer

		// Test to see if the user's input is a valid integer in the range.
		// If the input does not start with an integer number, a valid conversion cannot occur,
		// and a 0 is returned. Any letter or symbol will return a 0.
		if (period < 3000) {
			Serial.println("Please enter a valid integer between greater than 3000.");
		} else { // The user entered a valid integer, and the program continues
			valid_period = true;
		}
	}
}

void loop() {
	// Top of the main loop
	if(firstTime){							// Checks if it's first time running
		Serial.println("Running scan for the first time.");
		servoCon.scan();					// Runs scan for the first time
		firstTime = false;					// Set 'firstTime' to false so initial scan only runs once
	}

	// A significant change in light level triggers a rescan
	if(servoCon.mutexLock){		// Checks if scan is unlocked currently ('mutexLock = true' means the lock is unlocked)
		if ((servoCon.getCurrentLight() > (200 + servoCon.getOldLight())) || (servoCon.getCurrentLight() < (servoCon.getOldLight() - 200))){
			servoCon.mutexLock = false; // Locks mutexLock for scan so another scan cannot be run simultaneously
			Serial.println("Light level changed drastically. Running scan.");
			servoCon.scan(); // Run a scan
		}
	}

	// Button push triggers a scan
	int val = digitalRead(buttonPin); // Read input value from button
	if (val == 0){	// If the button is pressed
		Serial.println("Button was pressed. Running scan.");
		servoCon.scan(); // Run a scan
	}

	delay(1); // Incur a delay of 1ms during each loop iteration
	delayCount += 1; // Increment 'delayCount' by 1 each loop to eventually reach 'period'
	if (delayCount == period){  // Checks if the amount of time that has passed since last periodic scan equals 'period'
		Serial.println("Running Periodic Scan set by User.");
		servoCon.scan();			// Scans after periodic delay is met
		delayCount = 0;			// Resets the delayCount so the period can be reached again
	}

}

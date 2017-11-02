// ServoController.cpp - Implementation of ServoController class
// By Dylan Herrel and Matthew Hirsch
// CSC 113 - Assignment 3
// 10/24/17

#include "ServoController.h"

// Default Constructor: initializes values
ServoController::ServoController() {
	restPosition = 0;		// Set the initial rest position to 0
	lightPosition = 0;		// Set the initial light position to 0
	light = 0;				// Set the initial light value to 0
	oldLight = 0;			// Set the initial previous brightest light value to 0
	analogPin = 0;			// Assigns the A0 pin to read the Photoresister
	mutexLock = true;  		// Boolean used as a pseudo-mutexLock for lock the scan function (true = unlock, false = lock)
}

// Returns the value of restPosition
int ServoController::getPosition(){
	return restPosition;
}

// Moves the Servo to look for brightest light source
void ServoController::scan(){
	light = 0; 									// Reset value of 'light' to 0 to find new brightest value for each scan
	for (int pos = 0; pos <= 180; pos += 1) { 	// Servo arm rotates from 0 degrees to 180 degrees
												// Increment the position in steps of 1 degree
		myServo.write(pos);				 		// Tell Servo to go to position in variable 'pos'
		delay(15);                       		// Wait 15ms for the Servo to reach the position
		int val = analogRead(analogPin); 		// Read analog pin connected to photoresistor
		if(light < val) {                		// Compare brightest light value to most recently read light value
			lightPosition = pos;		 		// Save new position for brightest light
			light = val; 				 		// Save new value of brightest light
			oldLight = light;			 		// Save current light value to be compared to later
		}
	}
	updatePosition(); 					// Move Servo to brightest light position
}

// Checks if brightest light source has changed, then updates the position and moves to it.
void ServoController::updatePosition(){
	if (restPosition != lightPosition){   // Check if Servo is on the newest correct position
		restPosition = lightPosition;	  // Update the new position
		myServo.write(restPosition);	  // Move Servo to updated position
	}
	delay(500);							// Gives time for Servo to move into position
	mutexLock = true;					// Unlocks the mutexLock
}

// Reads the analog pin and gets the current light value
int ServoController::getCurrentLight(){
	int val = analogRead(analogPin); // Read analog pin connected to photoresistor
	if (light != val){ 				 // If currently stored brightest light value does not equal current light value
		return val; 				 // Return new light value
	}else { 						 // Brightest light value has not changed
		return light; 				 // Return currently stored brightest light value
	}
}

// Returns the value of oldLight
int ServoController::getOldLight(){
	return oldLight;
}

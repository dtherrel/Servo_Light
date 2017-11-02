// ServoController.h - Definition of ServoController class
// By Dylan Herrel and Matthew Hirsch
// CSC 113 - Assignment 3
// 10/24/17

#ifndef SERVOCONTROLLER_H_ // If header file is not already defined
#define SERVOCONTROLLER_H_ // Define the header file

#include <Arduino.h>
#include <Servo.h>

class ServoController {
private:
	int restPosition; 	// The position of rest for the Servo on the brightest light
	int light; 			// The value of the brightest light
	int analogPin;		// Analog pin on the Arduino used to read value of Photoresistor
	int lightPosition;	// The position of the Servo arm to the brightest light
	int oldLight;		// The value of the previous brightest light

public:
	// Default Constructor: initializes values
	ServoController();

	// Returns the value of restPosition
	int getPosition();

	// Moves the Servo to look for brightest light source
	void scan();

	// Checks if brightest light source has changed, then updates the position and moves to it.
	void updatePosition();

	// Reads the analog pin and gets the current light value
	int getCurrentLight();

	// Returns the value of oldLight
	int getOldLight();

	bool mutexLock; // Boolean used as a pseudo-mutexLock for lock the scan function

	Servo myServo; // Each ServoController object has a Servo object within it
};

#endif /* SERVOCONTROLLER_H_ */

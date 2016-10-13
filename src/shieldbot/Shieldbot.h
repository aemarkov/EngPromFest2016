/*
  Created to support the release of the Sheildbot from SeeedStudios
  http://www.seeedstudio.com/wiki/Shield_Bot

  Created by Jacob Rosenthal and Colin Ho, December 30, 2012.
  Released into the public domain.
*/

// ensure this library description is only included once
#ifndef Shieldbot_h
#define Shieldbot_h


// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class Shieldbot
{
  // user-accessible "public" interface
  public:
	Shieldbot();
	int readS1();
	int readS2();
	int readS3();
	int readS4();
	int readS5();
	void setMaxSpeed(int);
	void setMaxSpeed(int, int);
	void setMaxLeftSpeed(int);
	void setMaxRightSpeed(int);
	void rightMotor(char);
	void leftMotor(char);
	void drive(char, char);
	void forward();
	void backward();
	void stop();
	void stopRight();
	void stopLeft();
	void fastStopLeft();
	void fastStopRight();
	void fastStop();

};

#endif
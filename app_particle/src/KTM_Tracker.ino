/*****************************************************************/
/*****************************************************************/
/*
	Motocycle / Car Tracker firmware for Particle Electron
	(c) Copyright 2017, Jason R. Haddix

*/
/*****************************************************************/
/*****************************************************************/

#include "_libs/AssetTracker/AssetTracker.h"
#include "math.h"




// DEFINITIONS
#define TIME millis()                                // Global system run time - milliseconds




// PIN VARS
int POWER_PIN = D0;									 // Pin used to determine power states from external source
int ALARM_PIN = D1;                                  // Pin used to trigger piezo alarm
int WKP_PIN = A1;                                    // Pin used to used to wake tracker from SLEEP state




void setup()
{

}



void loop()
{


}

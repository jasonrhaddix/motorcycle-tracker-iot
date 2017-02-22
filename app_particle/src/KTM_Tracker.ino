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

// GLOBAL VARS
long GLOBAL_LAST_CHECK = 0;                          // TIME since last APP_MODE check
long GLOBAL_LAST_CHECK_DELAY = 1;                    // Check/set app mode every [x] minutes
int GLOBAL_BOOT_CHECK = 1;                           // Initial app mode check (runs loop until it gets out of 0:BOOT mode)
int DEEP_SLEEP_TIME = 120;                           // [x] minutes until it exits DEEP SLEEP MODE to check app state
bool GPS = 0;                                         // GPS - 1:TRUE (within geo-fence) | 2: FALSE (outside ge-fence)
bool POWER = 0;                                       // POWER - 1:TRUE (vehicle running) | 2: FALSE (vehicle off)
bool ACCEL = 0;										 // ACCEL - 1: TRUE (threshold hit) | 2: FALSE (threshold not hit)
bool ALARM = 0;
int APP_MODE = 0;                                    // 0:BOOT / 1:SLEEP / 2: REST / 3: GUARD / 4:ALERT 
int PREV_APP_MODE;                                   // Previous APP_MODE state




void setup()
{

}



void loop()
{


}

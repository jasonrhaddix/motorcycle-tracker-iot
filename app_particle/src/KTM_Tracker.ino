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




// USER CONTROLLED VARS
bool MASTER_ALERT = 0;                               // Boolean to force ALERT mode

// PIN VARS
int POWER_PIN = D0;									 // Pin used to determine power states from external source
int ALARM_PIN = D1;                                  // Pin used to trigger piezo alarm
int WKP_PIN = A1;                                    // Pin used to used to wake tracker from SLEEP state

// GLOBAL VARS
long GLOBAL_LAST_CHECK = 0;                          // TIME since last APP_MODE check
long GLOBAL_LAST_CHECK_DELAY = 1;                    // Check/set app mode every [x] minutes
int GLOBAL_BOOT_CHECK = 1;                           // Initial app mode check (runs loop until it gets out of 0:BOOT mode)
int DEEP_SLEEP_TIME = 120;                           // [x] minutes until it exits DEEP SLEEP MODE to check app state
bool GPS = 0;                                        // GPS - 1:TRUE (within geo-fence) | 2: FALSE (outside ge-fence)
bool POWER = 0;                                      // POWER - 1:TRUE (vehicle running) | 2: FALSE (vehicle off)
bool ACCEL = 0;										 // ACCEL - 1: TRUE (threshold hit) | 2: FALSE (threshold not hit)
bool ALARM = 0;
int APP_MODE = 0;                                    // 0:BOOT / 1:SLEEP / 2: REST / 3: GUARD / 4:ALERT 
int PREV_APP_MODE;                                   // Previous APP_MODE state

// WATCHDOG TIMER 
long timer_WatchDog_ResetLast = 0;                   // TIME since last last system reset
int timer_WatchDog_ResetDelay = 24;                  // [x] hours until full system reset

// HAVERSINE (distance) FORMULA
const float earthRadius = 6378100;                   // Radius of earth in meters
const float PI = 3.14159265;                         // Math.PI

// GPS VARS
int gps_GeoFence_Radius = 100;                       // Geo-fence radius in meters
float gps_HomePos[2] = { 33.773016, -118.149690 };   // long/lat of geo-fence point (HOME)
float gps_TrackerPos[2];                             // Array used to store long/lat of GPS tracker
long timer_getGPS_GetLast = 0;                       // TIME since last GPS reset
long timer_getGPS_GetTimeout = 60;                   // (if no GPS fix) [x] seconds until system reset
long gps_SampleSize_Ticks = 5;                       // [x] ticks*60000UL (seconds) to sample GPS tracker long/lat (increases accuracy)




// PARTICLE HARDWARE VARS
AssetTracker tracker = AssetTracker();               // Particle Tracker shield
FuelGauge fuel;                                      // LiPo Battery
CellularSignal cell;                                 // Electron cell module




void setup()
{
	Serial.begin(9600);

	pinMode(POWER_PIN, INPUT);
	pinMode(ALARM_PIN, OUTPUT);
	pinMode(WKP_PIN, INPUT);

	Time.zone(-8); 
  	Time.hourFormat12();

  	tracker.begin();
	tracker.gpsOn();

	define_ExternalFunctions();
	timer_WatchDog();

	timer_getGPS_GetLast = TIME;
}




void define_ExternalFunctions()
{
	Particle.function( "ALERT", set_Mode_ALERT );
	Particle.function( "CELL", get_Cell_Strength );
	Particle.function( "BATT_LEVEL", get_Batt_Level );	
	Particle.function( "BATT_VOLTS", get_Batt_Voltage );	
}




void loop()
{


}




int get_GPS()
{	
	if ( tracker.gpsFix() ) {

		// Particle.publish("t-notify", "GPS FOUND", 60, PRIVATE);
		
		float dist;
		int i = 0;

		while( i < gps_SampleSize_Ticks ) {

			delay(1000);

			gps_TrackerPos[0] = tracker.readLatDeg();
		    gps_TrackerPos[1] = tracker.readLonDeg();

		    dist = get_Distance( gps_HomePos[0], gps_HomePos[1], gps_TrackerPos[0], gps_TrackerPos[1] );

			++i;
		}

		Serial.println("Distance from HOME: " + String(dist));

		timer_getGPS_GetLast = TIME;

		int gps = ( dist < gps_GeoFence_Radius) ? 1 : 0;
		return gps;

	} else if ( TIME - timer_getGPS_GetLast > timer_getGPS_GetTimeout*1000UL ) {

		// Particle.publish("t-notify", "NO GPS", 60, PRIVATE);

		delay(1000);
		System.reset();

	}
}




// Haversine Formula - determins distance of 2 gps points
double get_Distance( float start_lat, float start_long, float end_lat, float end_long )
{  
  start_lat /= 180 / PI; 
  start_long /= 180 / PI;
  end_lat /= 180 / PI;
  end_long /= 180 / PI;
  
  float a = pow( sin( (end_lat-start_lat)/2 ), 2 ) + cos( start_lat ) * cos( end_lat ) * pow( sin( (end_long-start_long)/2 ), 2 );
  float dist = earthRadius * 2 * atan2( sqrt(a), sqrt(1-a) );
  return double( dist );
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// EXTERNAL FUNCTIONS
//****************************************************************/
// MANUALLY SET ALERT MODE
int set_Mode_ALERT( String command )
{
	MASTER_ALERT = 1;
	return 1;
}


// GET CELLULAR SIGNAL STRENGTH
int get_Cell_Strength( String command )
{
	cell = Cellular.RSSI();
	// String rssi = String(cell.rssi) + String(",") + String(cell.qual);
	return 1;
}


// GET BATTERY LEVEL
int get_Batt_Level( String command )
{
	int battLevel = fuel.getSoC();
	return battLevel;
}


// GET BATTERY VOLTAGE
int get_Batt_Voltage( String command )
{
	int battVoltage = fuel.getVCell();
	return battVoltage;
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// WATCHDOG TIMER / SYSTEM RESET / Avoids stack overflow
//****************************************************************/
void timer_WatchDog()
{
	if ( TIME - timer_WatchDog_ResetLast > timer_WatchDog_ResetDelay*60000UL*60000UL ) {

		System.reset();

	}
}
//****************************************************************/
//****************************************************************/	

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
int POWER_PIN = D1;									 // Pin used to determine power states from external source
int ALARM_PIN = D2;                                  // Pin used to trigger piezo alarm
// int WKP_PIN = A7;                                 // Pin used to used to wake tracker from SLEEP state

// GLOBAL VARS
long GLOBAL_CHECK_LAST = 0;                          // TIME since last APP_MODE check
long GLOBAL_CHECK_LAST_DELAY = 1;                    // Check/set app mode every [x] minutes
int GLOBAL_CHECK_BOOT = 1;                           // Initial app mode check (runs loop until it gets out of 0:BOOT mode)
int DEEP_SLEEP_TIME = 120;                           // [x] minutes until it exits DEEP SLEEP MODE to check app state
int APP_MODE = 0;                                    // 0:BOOT / 1:SLEEP / 2: REST / 3: GUARD / 4:ALERT 
int PREV_APP_MODE;                                   // Previous APP_MODE state
bool MODE_SETUP = 1;

bool GPS = 0;                                        // GPS - 1:TRUE (within geo-fence) | 2: FALSE (outside ge-fence)
bool POWER = 0;                                      // POWER - 1:TRUE (vehicle running) | 2: FALSE (vehicle off)
bool ACCEL = 0;										 // ACCEL - 1: TRUE (threshold hit) | 2: FALSE (threshold not hit)
bool ALARM = 0;

// WATCHDOG TIMER 
long WATCHDOG_Timer_ResetLast = 0;                   // TIME since last last system reset
int WATCHDOG_Timer_ResetDelay = 24;                  // [x] hours until full system reset

// HAVERSINE (distance) FORMULA
const float earthRadius = 6378100;                   // Radius of earth in meters
// const float PI = 3.1415926535897932384626433832795;  // Math.PI

//
int HARDWARE_LAST = 1;

// GPS VARS
float gps_HomePos[2] = { 33.773016, -118.149690 };   // long/lat of geo-fence point (HOME)
float gps_TrackerPos[2];                             // Array used to store long/lat of GPS tracker
long gps_Timer_GetLast = 0;                          // TIME since last GPS reset
long gps_Timer_GetTimeout = 60;                      // (if no GPS fix) [x] seconds until system reset
long gps_SampleSize_Ticks = 5;                       // [x] ticks*60000UL (seconds) to sample GPS tracker long/lat (increases accuracy)
int gps_GeoFence_Radius = 100;                       // Geo-fence radius in meters
bool HARWARE_GPS = 0;

// ACCELEROMETER VARS
int accel_Threshold = 9000;                          // Threshold to trigger ALERT mode. 9000 is VERY sensitive, 12000 will detect small bumps
int accel_Current = 0;                               // Combined accelerometer reading - current registered
bool accel_Timer_Start = 0;
long accel_Timer_GetLast = 0;                        // TIME since last battery level check
long accel_Timer_GetDelay = 10;                      // Check battery level every [x] minutes
long accel_Timer_SampleStart = 0;
long accel_Timer_SampleEnd = 75;

// BATTERY VARS
int batt_AlertLevel = 20;                            // Send alert is less than [x] percentage
int batt_CurrentLevel;                               // Current battery level
int batt_Timer_GetLast = 0;						     //
int batt_Timer_GetDelay = 15;					     //

// REST VARS
long REST_LastPub = 0;                               // TIME since last battery level check
long REST_PubDelay = 5;                              // Check battery level every [x] minutes
String rest_pub = "";

// ALERT VARS
long ALERT_LastPub = 0;                              // TIME since last battery level check
long ALERT_PubDelay = 30;                            // Check battery level every [x] seconds




// PARTICLE HARDWARE VARS
AssetTracker tracker = AssetTracker();               // Particle Tracker shield
FuelGauge fuel;                                      // LiPo Battery
CellularSignal cell;                                 // Electron cell module




void setup()
{
	Serial.begin(9600);

	pinMode(POWER_PIN, INPUT);
	pinMode(ALARM_PIN, OUTPUT);

	Time.zone(-8); 
  	Time.hourFormat12();

	set_HardwareMode(0);
	define_ExternalFunctions();

	gps_Timer_GetLast = TIME;
	
	WATCHDOG_Timer();
}




void define_ExternalFunctions()
{
	Particle.function( "RESET", reset_System );
	Particle.function( "ALERT", set_Mode_ALERT );
	Particle.function( "CELL", get_Cell_Strength );
	Particle.function( "BATT_LEVEL", get_Batt_Level );	
	Particle.function( "BATT_VOLTS", get_Batt_Voltage );	
	Particle.function( "GPS_SET_HOME", set_GPS_HOME );
}




void loop()
{
	if( HARWARE_GPS ) tracker.updateGPS();
	if( MASTER_ALERT ) APP_MODE = 4;
	

	if( GLOBAL_CHECK_BOOT == 1 || 
		TIME - GLOBAL_CHECK_LAST > GLOBAL_CHECK_LAST_DELAY*60000UL &&
		APP_MODE != 4 ) check_StateMode();


	if( APP_MODE != PREV_APP_MODE ) MODE_SETUP = 1;
	if( APP_MODE != PREV_APP_MODE || APP_MODE > 1 )
	{
		switch( APP_MODE )
		{
			case 0 :
				manageMode_BOOT();
				break;

			case 1 :
				manageMode_SLEEP();
				break;

			case 2 :
				manageMode_REST();
				break;

			case 3 :
				manageMode_GUARD();
				break;

			case 4 :
				manageMode_ALERT();
				break;
		}

		PREV_APP_MODE = APP_MODE;
	}

}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// StateMode FUNCTIONS
//****************************************************************/
void check_StateMode()
{
	if ( tracker.gpsFix() ) {

		GLOBAL_CHECK_BOOT = 0;
		GLOBAL_CHECK_LAST = TIME;

		POWER = digitalRead( POWER_PIN );
		GPS = get_GPS();

		check_BatteryLevel();

		if ( GPS && POWER ) {			// HOME || ON - REST
			APP_MODE = 2;
		} else if ( !GPS && POWER ) {	// NOT HOME || ON - REST
			APP_MODE = 2;
		} else if ( GPS && !POWER ) {	// HOME || OFF - SLEEP
			APP_MODE = 1;	
		} else if ( !GPS && !POWER ) {	// NOT HOME || OFF - GUARD
			APP_MODE = 3;
		} else {						// DEFAULT - GUARD
			APP_MODE = 3;
		}
	}
}




void manageMode_BOOT()
{
	// Particle.publish("t-status", "BOOT", 60, PRIVATE);
	Serial.println("BOOT MODE");
}



void manageMode_SLEEP()
{
	// Particle.publish("t-status", "SLEEP", 60, PRIVATE);
	Serial.println("SLEEP MODE");

	// if( ALARM ) kill_Alarm();

	blink_RGB("#7200FF", 255, 1, 5);

	// delay(2000);
	System.sleep( WKP, FALLING /*, DEEP_SLEEP_TIME*60000UL*/ );

	// delay(2000);
	System.reset();
}



void manageMode_REST()
{
	// Particle.publish("t-status", "REST", 60, PRIVATE);
	if( MODE_SETUP )
	{
		Serial.println("REST MODE");
	}

	if( ALARM ) kill_Alarm();
	if ( TIME - REST_LastPub > REST_PubDelay*60000UL )
	{
		REST_LastPub = TIME;

		int i = 0;
		while ( i < gps_SampleSize_Ticks ) {

			delay(1000);

			gps_TrackerPos[0] = tracker.readLatDeg();
		    gps_TrackerPos[1] = tracker.readLonDeg();

			++i;
		}	
		
		/*rest_pub += String::format("{\"l\":%.5f,\"L\":%.5f}",gps_TrackerPos[0],gps_TrackerPos[1]);
		Particle.publish("REST", "["+rest_pub+"]", 60, PRIVATE);*/
	}
}



void manageMode_GUARD()
{
	if( MODE_SETUP )
	{
		// Particle.publish("t-status", "GUARD", 60, PRIVATE);
		Serial.println("GUARD MODE");

		set_HardwareMode(1);
		MODE_SETUP = 0;
	}

	
	POWER = digitalRead( POWER_PIN );
	if( POWER ) APP_MODE = 2; 
	if( ALARM ) kill_Alarm();
	ACCEL = check_Accel();


	if ( ACCEL && TIME - accel_Timer_GetLast < accel_Timer_GetDelay*1000UL && accel_Timer_Start == 1 )
	{
		Serial.println("ALARM");

		GLOBAL_CHECK_LAST = TIME;
		APP_MODE = 4;

	} else if( TIME - accel_Timer_GetLast > accel_Timer_GetDelay*1000UL && accel_Timer_Start == 1 )
	{
		accel_Timer_Start = 0;
	}


	if ( ACCEL && accel_Timer_Start == 0 ) {
		
		accel_Timer_GetLast = TIME;
		accel_Timer_Start = 1;

		digitalWrite( ALARM_PIN, HIGH ); // <------------------------REWORK CODE
		delay(250);
		digitalWrite( ALARM_PIN, LOW );  // 

		Serial.println("CHIRP");
	}
}



void manageMode_ALERT()
{
	if( MODE_SETUP )
	{
		// Particle.publish("t-status", "ALERT", 60, PRIVATE);
		Serial.println("ALERT MODE");

		set_HardwareMode(0);
		MODE_SETUP = 0;
	}
	
	
	if( !ALARM ) trigger_Alarm();
	ALARM = 1;

	if ( TIME - ALERT_LastPub > ALERT_PubDelay*1000UL )
	{
		ALERT_LastPub = TIME;

		int i = 0;
		while( i < gps_SampleSize_Ticks ) {

			delay(1000);

			gps_TrackerPos[0] = tracker.readLatDeg();
		    gps_TrackerPos[1] = tracker.readLonDeg();

			++i;
		}	
		
		// rest_pub += String::format("{\"l\":%.5f,\"L\":%.5f}",gps_TrackerPos[0],gps_TrackerPos[1]);
		// Particle.publish("REST", "["+rest_pub+"]", 60, PRIVATE);

	}
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// GPS FUNCTIONS
//****************************************************************/
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

		gps_Timer_GetLast = TIME;

		int gps = ( dist < gps_GeoFence_Radius ) ? 1 : 0;
		return gps;

	} else if ( TIME - gps_Timer_GetLast > gps_Timer_GetTimeout*1000UL ) {

		Particle.publish("t-notify", "NO GPS", 60, PRIVATE);
		Serial.println("NO GPS");

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
int check_Accel()
{
	int accel = 0;
	accel_Current = tracker.readXYZmagnitude();
	
	if ( accel_Current > accel_Threshold ) { // reduces accelerometer bounce

		if( TIME - accel_Timer_SampleStart > accel_Timer_SampleEnd )
		{
			Serial.println("BUMP!");

			accel_Timer_SampleStart = TIME;
			return accel = 1;
			
		}
	} else {
		return accel;
	}
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// EXTERNAL FUNCTIONS
//****************************************************************/
// 0:GPS | 1:ACCEL
void set_HardwareMode(int hardware)
{
	if( HARDWARE_LAST != hardware )
	{
		switch( hardware )
		{
			case 0 :
				Serial.println("Hardware - GPS");

				tracker.gpsOn();
				HARWARE_GPS = 1;

				break;

			case 1 :
				Serial.println("Hardware - ACCEL");

				tracker.gpsOff();
				HARWARE_GPS = 0;
				
				delay(250);
				tracker.begin();
				
				break;
		}

		HARDWARE_LAST = hardware;
	}
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// EXTERNAL FUNCTIONS
//****************************************************************/
void check_BatteryLevel()
{
	batt_CurrentLevel = fuel.getSoC();

	if( batt_CurrentLevel <= batt_AlertLevel && TIME - batt_Timer_GetLast > batt_Timer_GetDelay*60000UL)
	{
		//Particle.publish("t-notify", "LOW BATTERY", 60, PRIVATE);
		Serial.println("LOW BATTERY");

		batt_Timer_GetLast = TIME;
	}
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

// MANUALLY SET ALERT MODE
int set_GPS_HOME( String command )
{
	int set_gps = get_GPS();
	
	while(set_gps != 1)
	{
		// loop	
	}
	
	gps_HomePos[0] = tracker.readLatDeg();
    gps_HomePos[1] = tracker.readLonDeg();

	return 1;
}


// MANUALLY SET ALERT MODE
int reset_System( String command )
{
	System.reset();
	return 1;
}


// GET CELLULAR SIGNAL STRENGTH
int get_Cell_Strength( String command )
{
	cell = Cellular.RSSI();
	String rssi = String(cell.rssi) + String(",") + String(cell.qual);
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
// UTILITY FUNCTIONS
//****************************************************************/
void trigger_Alarm()
{
	ALARM = 1;
	digitalWrite( ALARM_PIN, HIGH );
}



void kill_Alarm()
{
	ALARM = 0;
}




void blink_RGB( String color, int brightness, int rate, int duration )
{
	long hex_num = (long) strtol( &color[1], NULL, 16);

	int r = hex_num >> 16;
	int g = hex_num >> 8 & 0xFF;
	int b = hex_num & 0xFF;

	RGB.control(true);
	delay(10);

	RGB.color(r, g, b);	

	long last = TIME;
	long tick = 0;
	bool led = 1;
	
	while( TIME - last < duration*1000UL )
	{
		if ( tick >= 20000 / (rate/2) ) {
			tick = 0;
			
			if( led ) {
				// RGB.brightness(0); // NOT WORKING
			} else {
				// RGB.brightness(255); // NOT WORKING
			}

			led = !led;
		}

		++tick;
	}

	delay(10);
	RGB.control(false);
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// WATCHDOG TIMER / SYSTEM RESET / Avoids stack overflow
//****************************************************************/
void WATCHDOG_Timer()
{
	if ( TIME - WATCHDOG_Timer_ResetLast > WATCHDOG_Timer_ResetDelay*60000UL*60000UL ) {

		System.reset();

	}
}
//****************************************************************/
//****************************************************************/	

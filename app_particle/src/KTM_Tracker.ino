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
int POWER_PIN = D1;									 // Pin used to determine power states from external source
int ALARM_PIN = D2;    


int APP_MODE = 0;                                    // 0:BOOT / 1:SLEEP / 2: REST / 3: GUARD / 4:ALERT 
int APP_MODE_LAST;

//
bool __GPS__;
bool __POWER__;
bool __ACCEL__;
bool __ALARM__;

//
bool BOOT_INIT = 1;
bool MODE_INIT = 1;
bool UPDATING_GPS = 0;
bool POWER_LAST = 0;
bool HARDWARE_MODE_LAST = 0;
bool HARWARE_MODE = 0;
bool POWER_MASTER = 1;

// HAVERSINE (distance) FORMULA
const float earthRadius = 6378100;                       // Radius of earth in meters
const float PI = 3.1415926535897932384626433832795;      // Math.PI

// GPS VARS
float gps_HomePos[2] = { 33.773016, -118.149690 };       // long/lat of geo-fence point (HOME)
float gps_TrackerPos[2];                                 // Array used to store long/lat of GPS tracker
long gps_Timer_GetLast = 0;                              // TIME since last GPS reset
long gps_Timer_GetTimeout = 60;                          // (if no GPS fix) [x] seconds until system reset
long gps_SampleSize_Ticks = 5;                           // [x] ticks*60000UL (seconds) to sample GPS tracker long/lat (increases accuracy)
int gps_GeoFence_Radius = 100	;                           // Geo-fence radius in meters

// ACCELEROMETER VARS
int accel_Threshold = 9000;                          // Threshold to trigger ALERT mode. 9000 is VERY sensitive, 12000 will detect small bumps
int accel_Current = 0;                               // Combined accelerometer reading - current registered
bool accel_HitTimer_Start = 0;
long accel_HitTimer_GetLast = 0;                        // TIME since last battery level check
long accel_HitTimer_GetDelay = 10;                      // Check battery level every [x] minutes
long accel_HitTimer_SampleStart = 0;
long accel_HitTimer_SampleEnd = 75;

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



// WATCHDOG TIMER 
long WATCHDOG_Timer_ResetLast = 0;                       // TIME since last last system reset
int WATCHDOG_Timer_ResetDelay = 24;                      // [x] hours until full system reset



// PARTICLE HARDWARE VARS
AssetTracker tracker = AssetTracker();                   // Particle Tracker shield
FuelGauge fuel;                                          // LiPo Battery
CellularSignal cell;                                     // Electron cell module



void setup()
{
	Serial.begin(9600);

	pinMode(POWER_PIN, INPUT);

	Time.zone(-8); 
  	Time.hourFormat12();

  	define_ExternalFunctions();
}



void define_ExternalFunctions()
{
	Particle.function( "POWER_ON", set_Power_On );
	Particle.function( "POWER_OFF", set_Power_Off );
}




void loop()
{
	// __POWER__ = digitalRead( POWER_PIN );
	__POWER__ = POWER_MASTER;

	if( HARWARE_MODE ) tracker.updateGPS();
	if( UPDATING_GPS ) check_TrackerDistance();

	manage_TrackerMode();

	if ( __POWER__ != POWER_LAST || BOOT_INIT )
	{
		BOOT_INIT = 0;
		POWER_LAST = __POWER__;

		if( __POWER__ )
		{
			Serial.println("POWER CHANGE => ON");

			//UPDATING_GPS = 1;
			//set_HardwareMode(1);
			APP_MODE = 2;

		} else if( !__POWER__ ) {
			
			Serial.println("POWER CHANGE => OFF");

			UPDATING_GPS = 1;
			set_HardwareMode(1);
			check_TrackerDistance();

		} else {
			
			// CANNOT GET __POWER__ : Notify
		}
	}
}



void check_TrackerDistance()
{
	if( tracker.gpsFix() )
	{
		Serial.println("GPS | Getting Tracker Position");
		
		__GPS__ = get_GPS();
		UPDATING_GPS = 0;

		if( __GPS__ ) {
			Serial.println("GPS | State => 1 | Within Radius");
			
			APP_MODE = 1;

		} else {
			Serial.println("GPS | State => 2 | Outside Radius");

			APP_MODE = 3;
		}
	}
}




void check_TrackerPosition()
{

}



void manage_TrackerMode()
{
	if( APP_MODE != APP_MODE_LAST ) MODE_INIT = 1;

	switch( APP_MODE )
	{
		case 0 :
			trackerMode_BOOT();
			break;

		case 1 :
			trackerMode_SLEEP();
			break;

		case 2 :
			trackerMode_REST();
			break;

		case 3 :
			trackerMode_GUARD();
			break;

		case 4 :
			trackerMode_ALERT();
			break;
	}

	APP_MODE_LAST = APP_MODE;
}



void trackerMode_BOOT()
{
	if( MODE_INIT )
	{
		MODE_INIT = 0;

		Serial.println("BOOT MODE");
		
		blink_RGB("#FFFF00", 255, 1, 5);
	}	
}



void trackerMode_SLEEP()
{
	if( MODE_INIT )
	{
		MODE_INIT = 0;
		
		Serial.println("SLEEP MODE");
		
		blink_RGB("#7200FF", 255, 1, 5);
		
		delay(500);
		System.sleep( WKP, FALLING /*, DEEP_SLEEP_TIME*60000UL*/ );

		delay(500);
		System.reset();
	}	
}



void trackerMode_REST()
{
	if( MODE_INIT )
	{
		MODE_INIT = 0;
		
		Serial.println("REST MODE");

		blink_RGB("#0000FF", 255, 1, 5);
	}


	/*if ( TIME - REST_LastPub > REST_PubDelay*60000UL )
	{
		REST_LastPub = TIME;

		int i = 0;
		while ( i < gps_SampleSize_Ticks ) {

			delay(1000);

			gps_TrackerPos[0] = tracker.readLatDeg();
		    gps_TrackerPos[1] = tracker.readLonDeg();

			++i;
		}	
	}*/
}



void trackerMode_GUARD()
{
	if( MODE_INIT )
	{
		// Particle.publish("t-status", "GUARD", 60, PRIVATE);
		Serial.println("GUARD MODE");

		set_HardwareMode(0);
		MODE_INIT = 0;
	}


	__ACCEL__ = check_Accel();
	
	if ( __ACCEL__ && TIME - accel_HitTimer_GetLast < accel_HitTimer_GetDelay*1000UL && accel_HitTimer_Start == 1 )
	{
		Serial.println("ALARM");

		// GLOBAL_CHECK_LAST = TIME;
		APP_MODE = 4;

	} else if( TIME - accel_HitTimer_GetLast > accel_HitTimer_GetDelay*1000UL && accel_HitTimer_Start == 1 )
	{
		accel_HitTimer_Start = 0;
	}


	if ( __ACCEL__ && accel_HitTimer_Start == 0 ) {
		
		Serial.println("CHIRP");

		// GLOBAL_CHECK_LAST = TIME; 
		accel_HitTimer_GetLast = TIME;
		accel_HitTimer_Start = 1;

		digitalWrite( ALARM_PIN, HIGH ); // <------------------------REWORK CODE
		delay(250);
		digitalWrite( ALARM_PIN, LOW );  // 
	}
}



void trackerMode_ALERT()
{
	if( MODE_INIT )
	{
		// Particle.publish("t-status", "ALERT", 60, PRIVATE);
		Serial.println("ALERT MODE");

		set_HardwareMode(1);
		// if( !ALARM ) trigger_Alarm();
		// ALARM = 1;
		MODE_INIT = 0;
	}
	
	
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
		if( dist > 500000.00 ) System.reset();

		gps_Timer_GetLast = TIME;

		int gps = ( dist < gps_GeoFence_Radius ) ? 1 : 0;
		return gps;

	} else if ( TIME - gps_Timer_GetLast > gps_Timer_GetTimeout*1000UL ) {

		blink_RGB("#FF0000", 255, 1, 5);
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
// 
//****************************************************************/
int check_Accel()
{
	int accel = 0;
	accel_Current = tracker.readXYZmagnitude();
	
	if ( accel_Current > accel_Threshold ) { // reduces accelerometer bounce

		if( TIME - accel_HitTimer_SampleStart > accel_HitTimer_SampleEnd )
		{
			Serial.println("BUMP!");

			accel_HitTimer_SampleStart = TIME;
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
void set_HardwareMode(int hardware_int)
{
	if( HARDWARE_MODE_LAST != hardware_int )
	{
		switch( hardware_int )
		{
			case 0 :
				Serial.println("Hardware - ACCEL");

				tracker.gpsOff();
				HARWARE_MODE = 0;
				delay(250);
				
				tracker.begin();
				delay(250);
				
				break;

			case 1 :
				Serial.println("Hardware - GPS");

				tracker.gpsOn();
				HARWARE_MODE = 1;

				break;
		}

		HARDWARE_MODE_LAST = hardware_int;
	}
}
//****************************************************************/
//****************************************************************/	



//****************************************************************/
// EXTERNAL FUNCTIONS
//****************************************************************/
// MANUALLY SET ALERT MODE
/*int set_Mode_ALERT( String command )
{
	MASTER_ALERT = 1;
	return 1;
}*/

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


int set_Power_On( String command )
{
	POWER_MASTER = 1;
	return 1;
}

int set_Power_Off( String command )
{
	POWER_MASTER = 0;
	return 0;
}
//****************************************************************/
//****************************************************************/	



//****************************************************************/
// UTILITY FUNCTIONS
//****************************************************************/
void trigger_Alarm()
{
	__ALARM__ = 1;
	digitalWrite( ALARM_PIN, HIGH );
}



void kill_Alarm()
{
	__ALARM__ = 0;
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

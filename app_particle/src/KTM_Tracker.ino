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

// ACCELEROMETER VARS
int accel_Threshold = 9000;                          // Threshold to trigger ALERT mode. 9000 is VERY sensitive, 12000 will detect small bumps
int accel_Current = 0;                               // Combined accelerometer reading - current registered
bool timer_Accel_Start = 0;
long timer_Accel_GetLast = 0;                        // TIME since last battery level check
long timer_Accel_GetDelay = 20;                      // Check battery level every [x] minutes

// BATTERY VARS
int batt_AlertLevel = 20;                            // Send alert is less than [x] percentage
int batt_CurrentLevel;                               // Current battery level
int timer_GetBatt_GetLast = 0;
int timer_getBatt_GetDelay = 15;

// REST VARS
long REST_LastPub = 0;                               // TIME since last battery level check
long REST_PubDelay = 5;                              // Check battery level every [x] minutes
String rest_pub = "";

// ALERT VARS
long ALERT_LastPub = 0;                              // TIME since last battery level check
long ALERT_PubDelay = 15;                            // Check battery level every [x] seconds




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
	tracker.updateGPS();
	// POWER = digitalRead( POWER_PIN );

	if( MASTER_ALERT ) APP_MODE = 4;
	
	if ( TIME - GLOBAL_LAST_CHECK > GLOBAL_LAST_CHECK_DELAY*60000UL || GLOBAL_BOOT_CHECK == 1 && APP_MODE != 4 ) check_StateMode();

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

		GLOBAL_BOOT_CHECK = 0;
		GLOBAL_LAST_CHECK = TIME;

		POWER = digitalRead( POWER_PIN );
		GPS = get_GPS();

		check_BatteryLevel();

		if ( GPS && POWER ) {             // <------------------------------------------- // REST
			// HOME || ON
			APP_MODE = 2;
		} else if ( !GPS && POWER ) {     // <------------------------------------------- // REST
			// NOT HOME || ON
			APP_MODE = 2;
		} else if ( GPS && !POWER ) {     // <------------------------------------------- // SLEEP
			// HOME || OF
			APP_MODE = 1;	
		} else if ( !GPS && !POWER ) {    // <------------------------------------------ // GUARD
			// NOT HOME || OFF
			APP_MODE = 3;
		} else {                          // <------------------------------------------ // GUARD
			// DEFAULT
			APP_MODE = 3;
		}
	}
}




void manageMode_BOOT()
{
	// Particle.publish("t-status", "BOOT", 60, PRIVATE);
}



void manageMode_SLEEP()
{
	// Particle.publish("t-status", "SLEEP", 60, PRIVATE);
	if( ALARM ) kill_Alarm();

	blink_RGB("0x7200FF", 255, 2, 2);

	delay(2000);
	System.sleep( WKP_PIN, RISING, DEEP_SLEEP_TIME*60000UL );

	delay(2000);
	System.reset();
}



void manageMode_REST()
{
	// Particle.publish("t-status", "REST", 60, PRIVATE);
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
	// Particle.publish("t-status", "GUARD", 60, PRIVATE);
	if( ALARM ) kill_Alarm();
	ACCEL = check_Accel();

	if ( ACCEL && !timer_Accel_Start ) {
		
		timer_Accel_GetLast = TIME;
		timer_Accel_Start = 1;

		trigger_Alarm();

		delay(100);
	}

	if ( ACCEL && TIME - timer_Accel_GetLast < timer_Accel_GetDelay*1000UL && timer_Accel_Start )
	{
		APP_MODE = 4;

	} else if( TIME - timer_Accel_GetLast > timer_Accel_GetDelay*1000UL && timer_Accel_Start )
	{
		timer_Accel_Start = 0;
	}
}



void manageMode_ALERT()
{
	// Particle.publish("t-status", "ALERT", 60, PRIVATE);
	if( !ALARM ) trigger_Alarm();
	ACCEL = check_Accel();

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
int check_Accel()
{
	int accel;
	accel_Current = tracker.readXYZmagnitude();

	if ( accel_Current > accel_Threshold ) {

		//accel_Threshold = accel_Current;
		/*accel_Max_X = tracker.readX();
		accel_Max_Y = tracker.readY();
		accel_Max_Z = tracker.readZ();*/
		
		accel = 1;

		Serial.println( tracker.readXYZmagnitude() );
		// pubAccel = String::format( "{\"x\":%d,\"y\":%d,\"z\":%d}", accel_Max_X, accel_Max_Y,maxZ );
	} else {

		accel = 0;
	}

    return accel;
}
//****************************************************************/
//****************************************************************/	




//****************************************************************/
// EXTERNAL FUNCTIONS
//****************************************************************/
void check_BatteryLevel()
{
	batt_CurrentLevel = fuel.getSoC();

	if( batt_CurrentLevel <= batt_AlertLevel && TIME - timer_GetBatt_GetLast > timer_getBatt_GetDelay*60000UL)
	{
		timer_GetBatt_GetLast = TIME;
		// Particle.publish("t-notify", "LOW BATTERY", 60, PRIVATE);
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
// UTILITY FUNCTIONS
//****************************************************************/
void trigger_Alarm()
{
	ALARM = 1;
}



void kill_Alarm()
{
	ALARM = 0;
}




void blink_RGB( String color, int brightness, int rate, int duration )
{
	// int c = colorConverter(color);
	RGB.control(true);
	delay(10);
	
	int * rgb_color = colorConverter(color);
	RGB.color(rgb_color[0], rgb_color[1], rgb_color[2]);	

	long last = TIME;
	long tick = 0;
	boolean led = 1;
	
	while( TIME - last < duration*1000UL )
	{
		if ( tick >= 20000 / (rate/2) ) {
			tick = 0;
			
			if(led) {
				RGB.brightness(255);
			} else {
				RGB.brightness(0);
			}

			led = !led;
		}

		++tick;
	}

	delay(10);
	RGB.control(false);
}





int * colorConverter(String hexValue)
{
	int color[3];

	long long hex_num = strtoll( &hexValue[1], NULL, 16);
	color[0] = hex_num >> 16;
	color[1] = hex_num >> 8 & 0xFF;
	color[2] = hex_num & 0xFF;

  return color; 
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

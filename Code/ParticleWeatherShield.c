// This #include statement was automatically added by the Particle IDE.
#include "SparkFun_Photon_Weather_Shield_Library/SparkFun_Photon_Weather_Shield_Library.h"

char Org[] = "ORGANIZATION_NAME";
char Disp[] = "DISPLAY_NAME";
char Locn[] = "LOCATION";

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
//This is from the SparkFun WeatherShield library
Weather sensor;


//The amount of time (in milliseconds) to wait between each 
//publication of data via the Particle WebHook
//Particle webhooks are rate limited to a maximum of 10/minute/device
//if we can only send 10 times a minute, that means we can send on average
//about once every six seconds, or 6000 milliseconds.  So to avoid any 
//rate limits by the particle cloud, keep the sendDelay at at least 6000.
int sendDelay = 6000;

void setup()

{

  //Open up the Serial port for local diagnostics
  Serial.begin(9600);

  //Initialize the I2C sensors and ping them
  sensor.begin();

  /*You can only receive acurate barrometric readings or acurate altitiude
  readings at a given time, not both at the same time. The following two lines
  tell the sensor what mode to use. You could easily write a function that
  takes a reading in one made and then switches to the other mode to grab that
  reading, resulting in data that contains both acurate altitude and barrometric
  readings. For this example, we will only be using the barometer mode. Be sure
  to only uncomment one line at a time. */

  sensor.setModeBarometer();//Set to Barometer Mode

  //baro.setModeAltimeter();//Set to altimeter Mode
  //These are additional MPL3115A2 functions the MUST be called for the sensor to work.
  sensor.setOversampleRate(7); // Set Oversample rate
  //Call with a rate from 0 to 7. See page 33 for table of ratios.
  //Sets the over sample rate. Datasheet calls for 128 but you can set it
  //from 1 to 128 samples. The higher the oversample rate the greater
  //the time between data samples.
	
  //Give the sensors some time to initialize
  delay(10000);
}

void loop()

{
  //Measure Relative Humidity from the HTU21D or Si7021
  float h = sensor.getRH();

  //Measure Temperature from the HTU21D or Si7021
  float f = sensor.getTempF();
  //Temperature is measured every time RH is requested.
  //It is faster, therefore, to read it from previous RH
  //measurement with getTemp() instead with readTemp()

  //Initialize the data payload
  char payload[255]; 
  
  //Generate the temperature data payload
  snprintf(payload, sizeof(payload),"{\"s\":\"Weather\",\"u\":\"F\",\"m\":\"Temperature\",\"v\": %f,\"o\":\"%s\",\"d\":\"%s\",\"l\":\"%s\"}",f,Org,Disp,Locn);
  //Emit the payload to the serial port for monitoring purposes
  Serial.println(payload);
  //Send the temperature data payload
  Particle.publish("ConnectTheDots", payload);
  //Wait before sending the next message to avoid Particle cloud rate limit errors
  delay(sendDelay);

  //Generate the humidity data payload
  snprintf(payload, sizeof(payload),"{\"s\":\"Weather\",\"u\":\"%%\",\"m\":\"Humidity\",\"v\": %f,\"o\":\"%s\",\"d\":\"%s\",\"l\":\"%s\"}",h,Org,Disp,Locn);
  //Emit the payload to the serial port for monitoring purposes
  Serial.println(payload);
  //Send the humidity data payload
  Particle.publish("ConnectTheDots", payload);
   //Wait before sending the next message to avoid Particle cloud rate limit errors
  delay(sendDelay);

}
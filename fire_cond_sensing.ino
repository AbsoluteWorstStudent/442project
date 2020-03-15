#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

/*          TSL2591 DETAILS           //
  Dynamic Range: 600M:1
  Max Lux: 88K
//                                    */
/*          LM35  DETAILS             //
  NOTE: Temp will be input as 10 bit analog voltage reading.
  Voltage at pin in Volts = (reading from ADC) * (5/1024) 
  double analog2mV = (5/1024);
  converts the number 0-1023 from the ADC into 0-5V
  Then, convert mV into temperature
  Convert from 10mV per degree w/ 500mV offset.
  double mVtoTemp = ((sensormV)-0.5)*100;
//                                    */

const int tempPin = A0;    //Sensor connected to A0 in
int anaRead;          //read analog data from sensor
float sensormV;       //reading converted to mV
double tempC;         //actual temp in celcius
double tempF;         //actual temp in Farenheit
float luxDetected;

SoftwareSerial mySerial(4, 2); // RX, TX    //initialize HC-06

String command = ""; // Stores response of the HC-06 Bluetooth device
Adafruit_TSL2591 tsl = Adafruit_TSL2591(1); // pass in a number for the sensor identifier

void setup() {
//Initialize Fire Conditions
  double fireTempF = 400;
  double fireLux = 700;
  //NOTE, CHANGE fireLux DEPENDING ON ENVIRONMENT. 
  //Illumination of Kitchen can be 500lux, so Fire must be greater
  pinMode(tempPin, INPUT);
  
// Open serial communications:
  Serial.begin(115200);                   //Initialize Serial Monitor
  Serial.println("Start communication."); //Confirm successful connection to HC-06
  mySerial.begin(115200);                 //Initialize HC-06 Data Channel

  //TSL2591 Setup
  Adafruit_TSL2591 tsl = Adafruit_TSL2591(1); // pass in a number for the sensor identifier
  sensor_t luxSensorSettings;               //Get Sensor Event
  tsl.getSensor(&luxSensorSettings);    
  tsl.setGain(TSL2591_GAIN_MED);    //25x Gain. Recommended for General Purpose
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  //Shorter integration for quicker 
                                                 //detection of spike in lux (fire)
}

void loop() {
  sensors_event_t luxSensor;               //Get Sensor Event
  tsl.getEvent(&luxSensor);
  luxDetected = luxSensor.light;
  anaRead = analogRead(tempPin);          //Read Vout of LM35
  sensormV = (anaRead/1024.0)*5000;       //Convert LM35 binary to mVolt value
  tempC = sensormV/10;                    //Convert mVolt to Centigrade
  tempF = (tempC/5)*9+32;                 //Convert Centigrade to Farenheit
  Serial.print("Temperature: ");          //Print Celcius Reading to Serial Monitor
  Serial.print(tempC);
  Serial.println(" degrees Celcius");     
  Serial.print("Temperature: ");          //Print Farenheit Reading to Serial Monitor
  Serial.print(tempF);
  Serial.println(" degrees Farenheit");
  Serial.print("Lux: ");                    //Print Lux reading to Serial
  Serial.println(luxDetected);
  mySerial.write(luxDetected);            //Send Lux reading to HC-06
  delay(500);
  mySerial.write(tempF);                  //Send Farenheit temp to HC-06 for Transmission.
  Serial.println("Bluetooth Transmission Complete.");   //Confirm Succesfull Transmission.
  delay(1000);
  //If Fire Conditions met, activate servo -> Extinquish Fire
  /*if(tempF > FireTempF && luxDetect > fireLux){
  /
  //INSERT SERVO ACTIVATION CODE HERE
  }*/
}

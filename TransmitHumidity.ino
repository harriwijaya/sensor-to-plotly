/* Transmitter */

// for RF24 module
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// humidity sensor input
int HIH4030_Pin = A0; //analog pin 0

// force sensor input
int FSR400_Pin = A1; // analog pin 1

// nanocellulose pressure sensor
int NanoCellPress_Pin = A2;

// radio object
RF24 radio(7, 8);
//const byte rxAddr[6] = "00001";
const uint64_t rxAddr = 0x7878787878LL;

void setup(){
  //Serial.begin(9600);

  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();
}

#define MSG_SIZE 16

void loop()
{
  // message structure
  char charMsg[MSG_SIZE];
  /* Components:
   * 0-3  : "Tag "
   * 4    : reserved
   * 5-6  : (int) humidity
   * 7    : " "
   * 8    : reserved
   * 9-10 : (int) force
   * 11   : " "
   * 12   : reserved
   * 13-14: (int) pressure
   * 15   : '\0'
   * 
   * Note:
   *  integer size is 2 bytes,
   *  float size is 4 bytes,
   */
  char buf[4]; // integer to string

  int humInt = 0; // humidity in integer
  int forceInt = 0;
  int pressureInt = 0;

  //To properly caculate relative humidity, we need the temperature.
  float temperature = 25; //replace with a thermometer reading if you have it
  float relativeHumidity  = getHumidity(temperature);

  float measForce  = getForce();

  float measPressure = getPressure();

  //Serial.println(relativeHumidity);

  humInt = (int)relativeHumidity;

  charMsg[0] = 'T';
  charMsg[1] = 'a';
  charMsg[2] = 'g';
  charMsg[3] = ' ';
  charMsg[4] = 0; // redundant: just to avoid byte shifting in receiver side
  charMsg[5] = (char)(humInt>>8);
  charMsg[6] = (char)humInt;
  charMsg[7] = ' ';
  
  forceInt = (int)measForce;

  charMsg[8]  = 0; // redundant: just to avoid byte shifting in receiver side
  charMsg[9]  = (char)(forceInt>>8);
  charMsg[10] = (char)forceInt;
  charMsg[11] = ' ';

  pressureInt = (int)measPressure;

  charMsg[12] = 0; // redundant: just to avoid byte shifting in receiver side
  charMsg[13] = (char)(pressureInt>>8);
  charMsg[14] = (char)pressureInt;
  
  charMsg[15] = '\0';

  radio.write(&charMsg, sizeof(charMsg));

  delay(100); //just here to slow it down so you can read it
}


float getHumidity(float degreesCelsius){
  //caculate relative humidity
  float supplyVolt = 5.0;

  // read the value from the sensor:
  int HIH4030_Value = analogRead(HIH4030_Pin);
  float voltage = HIH4030_Value/1023. * supplyVolt; // convert to voltage value

  // convert the voltage to a relative humidity
  // - the equation is derived from the HIH-4030/31 datasheet
  // - it is not calibrated to your individual sensor
  //  Table 2 of the sheet shows the may deviate from this line
  float sensorRH = 161.0 * voltage / supplyVolt - 25.8;
  float trueRH = sensorRH / (1.0546 - 0.0026 * degreesCelsius); //temperature adjustment 

  return trueRH;
}

float getForce()
{
  float supplyVolt = 5.0;

  // read the value from the sensor:
  int FSR400_Value = analogRead(FSR400_Pin);
  float voltage = FSR400_Value/1023. * supplyVolt; // convert to voltage value

  float valForce = voltage * 10; // do conversion: here just simply x10

  return valForce;
  
}

float getPressure()
{
  float supplyVolt = 5.0;

  // read the value from the sensor:
  int NanoCellPress_Value = analogRead(NanoCellPress_Pin);
  /* trial: bump up */ NanoCellPress_Value *= 10*2;
  float voltage = NanoCellPress_Value/1023. * supplyVolt; // convert to voltage value

  float valPress = voltage; // do conversion: 

  return valPress;
}


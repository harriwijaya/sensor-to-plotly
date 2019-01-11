/* Receiver: Arduino Uno */

// I2C for receive data from QCM
#include <Wire.h>
// for RF24 module
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DBG_LOG // for debuging purpose

// radio object
RF24 radio(7, 8);
//const byte rxAddr[6] = "00002";
const uint64_t rxAddr = 0xB3B4B5B6F1LL;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(8); // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  #ifdef DBG_LOG
  Serial.begin(9600);
  #endif

  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();
}

#define MSG_SIZE (4+1+4+2+1)

void loop() {

  // do nothing, just wait
  // delay(100);

}

// event function
void receiveEvent(int howMany)
{
  /* NOTE: 
   * somehow arduino compiler do not promote ((byte) << 8) to higher type,
   * so, keep byte component as 4-bytes (or original).
  */
  unsigned long Count0, Count1, Count2, Count3;
  unsigned long CountAll;
  int Tempr0, Tempr1;
  int TemprAll;
  /* format: RAW []   []
   *         123-4567-89
  */

    // message structure
  char charMsg[MSG_SIZE];
  /* Components:
   * 0-3  : "Tag "
   * 4    : reserved
   * 5-8  : (unsigned long) counter
   * 9-10 : (int) temperature
   * 11   : '\0'
   * 
   * Note:
   *  integer size is 2 bytes,
   *  float size is 4 bytes,
   */
   
  /*while ( 6 < Wire.available() )
  {
    char c = Wire.read();
    Serial.print(c);
  }*/
  char aa = Wire.read(); // R
  char bb = Wire.read(); // A
  char cc = Wire.read(); // W

  #ifdef DBG_LOG
  Serial.println(aa); Serial.println(bb); Serial.println(cc); 
  #endif

  Count0 = Wire.read();
  Count1 = Wire.read();
  Count2 = Wire.read();
  Count3 = Wire.read();
  
  CountAll = 0;
  CountAll |= (unsigned long)(Count0      );
  CountAll |= (unsigned long)(Count1 <<  8);
  CountAll |= (unsigned long)(Count2 << 16);
  CountAll |= (unsigned long)(Count3 << 24);

  #ifdef DBG_LOG
  char buf[8]; // integer to string
  sprintf(buf, "%08lu", CountAll);
  Serial.println(buf);
  #endif
  
  Tempr0 = Wire.read();
  Tempr1 = Wire.read();
  TemprAll = (int)( (Tempr1 << 8) | (Tempr0));
  //Serial.println(TemprAll);

  charMsg[0] = 'T';
  charMsg[1] = 'a';
  charMsg[2] = 'g';
  charMsg[3] = ' ';
  
  charMsg[4] = (byte)Count0;
  charMsg[5] = (byte)Count1;
  charMsg[6] = (byte)Count2;
  charMsg[7] = (byte)Count3;

  charMsg[8] = (byte)Tempr0;
  charMsg[9] = (byte)Tempr1;

  charMsg[MSG_SIZE-1] = '\0';

  #ifdef DBG_LOG
  Serial.println(charMsg);
  #endif

  radio.write(&charMsg, sizeof(charMsg));
}


/* to communicate with PC which running node.js */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const uint64_t rAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin();

  //radio.setChannel(108);
  
  radio.openReadingPipe(0, rAddress[0]);
  radio.openReadingPipe(1, rAddress[1]);
  
  radio.startListening();
}

#define MSG_SIZE 16
#define MSG_TO_PC 255

void loop()
{
  byte sensorNode = 0; // which sensor node

  char txtMsg[MSG_SIZE] = {0};
  char txtMsg2[12] = {0};

  int digall;
  char buf[4]; // integer to string
  unsigned long digULb, digULall;
  int digIntb, digIntAll;
  char buf8[8];

  char txtMsgToPC[MSG_TO_PC] = {0};

  int idx=0;

  //
    txtMsgToPC[  idx] = 'T';
    txtMsgToPC[++idx] = 'a';
    txtMsgToPC[++idx] = 'g';
    txtMsgToPC[++idx] = ' ';

    // humidity : 4
    txtMsgToPC[++idx] = 'N';    // 'Y':available
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = ' ';

    // force : 10
    txtMsgToPC[++idx] = 'N';    // 'Y':available
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = ' ';

    // pressure : 16
    txtMsgToPC[++idx] = 'N';    // 'Y':available
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = ' ';

    // frequency : 22
    txtMsgToPC[++idx] = 'N';    // 'Y':available
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = ' ';

    // temperature : 32
    txtMsgToPC[++idx] = 'N';    // 'Y':available
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = 255;
    txtMsgToPC[++idx] = ' ';

    txtMsgToPC[++idx] = '\0';
  //
  
  if( radio.available(&sensorNode) )
  {
    // node-0
    switch(sensorNode)
    {
      case 0:
        //
        //Serial.println("Sensor node 0");
        radio.read(&txtMsg, sizeof(txtMsg));

        // - humidity
        digall = (int)( ((txtMsg[5]<<8)&0xFF00) | (txtMsg[6]&0x00FF) );
        sprintf(buf, "%04i", digall);

        /*txtMsg[4] = buf[0];
        txtMsg[5] = buf[1];
        txtMsg[6] = buf[2];*/

            // humidity
        idx = 4;
        txtMsgToPC[  idx] = 'Y';    // 'Y':available
        txtMsgToPC[++idx] = buf[0];
        txtMsgToPC[++idx] = buf[1];
        txtMsgToPC[++idx] = buf[2];
        txtMsgToPC[++idx] = buf[3];

        // - force
        digall = (int)( ((txtMsg[9]<<8)&0xFF00) | (txtMsg[10]&0x00FF) );
        sprintf(buf, "%04i", digall);

        /*txtMsg[8]  = buf[0];
        txtMsg[9]  = buf[1];
        txtMsg[10] = buf[2];*/

            // force
        idx = 10;
        txtMsgToPC[  idx] = 'Y';    // 'Y':available
        txtMsgToPC[++idx] = buf[0];
        txtMsgToPC[++idx] = buf[1];
        txtMsgToPC[++idx] = buf[2];
        txtMsgToPC[++idx] = buf[3];

        // - pressure
        digall = (int)( ((txtMsg[13]<<8)&0xFF00) | (txtMsg[14]&0x00FF) );
        sprintf(buf, "%04i", digall);
    
        /*txtMsg[12] = buf[0];
        txtMsg[13] = buf[1];
        txtMsg[14] = buf[2];*/
            // pressure
        idx = 16;
        txtMsgToPC[  idx] = 'Y';    // 'Y':available
        txtMsgToPC[++idx] = buf[0];
        txtMsgToPC[++idx] = buf[1];
        txtMsgToPC[++idx] = buf[2];
        txtMsgToPC[++idx] = buf[3];


        //Serial.println(txtMsg);
        break;
      case 1:
        //
        //Serial.println("Sensor node 1");
        radio.read(&txtMsg2, sizeof(txtMsg2));

        // - counter
        digULall = 0;
        digULb = (unsigned long)txtMsg2[4]; //Serial.println(digULb);
        digULall |= (unsigned long)((digULb      ) & 0x000000FF);
        digULb = (unsigned long)txtMsg2[5]; //Serial.println(digULb);
        digULall |= (unsigned long)((digULb <<  8) & 0x0000FF00);
        digULb = (unsigned long)txtMsg2[6]; //Serial.println(digULb);
        digULall |= (unsigned long)((digULb << 16) & 0x00FF0000);
        digULb = (unsigned long)txtMsg2[7]; //Serial.println(digULb);
        digULall |= (unsigned long)((digULb << 24) & 0xFF000000);
        sprintf(buf8, "%08lu", digULall);

            // frequency
        idx = 22;
        txtMsgToPC[  idx] = 'Y';    // 'Y':available
        txtMsgToPC[++idx] = buf8[0];
        txtMsgToPC[++idx] = buf8[1];
        txtMsgToPC[++idx] = buf8[2];
        txtMsgToPC[++idx] = buf8[3];
        txtMsgToPC[++idx] = buf8[4];
        txtMsgToPC[++idx] = buf8[5];
        txtMsgToPC[++idx] = buf8[6];
        txtMsgToPC[++idx] = buf8[7];

        // - temperature 
        digIntAll = 0;
        digIntb = (int)txtMsg2[8];
        digIntAll |= (int)(digIntb);
        digIntb = (int)txtMsg2[9];
        digIntAll |= (int)(digIntb << 8);
        sprintf(buf, "%04i", digIntAll);

            // temperature
        idx = 32;
        txtMsgToPC[  idx] = 'Y';    // 'Y':available
        txtMsgToPC[++idx] = buf[0];
        txtMsgToPC[++idx] = buf[1];
        txtMsgToPC[++idx] = buf[2];
        txtMsgToPC[++idx] = buf[3];
        
        //Serial.println(buf8);//(txtMsg2);
        break;
      default:
        //Serial.println("ERROR: invalid sensor node!!!");
        break;
    }

    Serial.println(txtMsgToPC);
    delay(250); //delay(100);



  }

}


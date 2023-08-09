// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>
using namespace std;

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
// 433.075;433.275;433.575
#define RF95_FREQ 470.0
// Change to 125000 / 250000 / 500000 OR other Bandeidth.
#define RF95_BW 250000
// Change to 7、8、9、10、11、12 OR other SF.
#define RF95_SF 12
// Change to 8、16、32、64... OR others PreambleLength.
#define RF95_PrL 8
// Change to 5、6、7 OR 8 other CodingRate.
#define RF95_CR 7
// Change TxPower from +2 to +20 (For 18, 19 and 20, PA_DAC is enabled).
#define RF95_TxP 17

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

// Setting frequency part
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

// Setting SpreadingFactor part
  rf95.setSpreadingFactor((uint8_t)RF95_SF);
  Serial.print("Set SF to: "); Serial.println(RF95_SF);

// Setting PreambleLength part
  rf95.setPreambleLength((uint16_t )RF95_PrL);
  Serial.print("Set PreambleLength to: "); Serial.println(RF95_PrL);

// Setting CodingRate part
  rf95.setCodingRate4((uint8_t)RF95_CR);
  Serial.print("Set CodingRate to: "); Serial.println(RF95_CR);

// Setting TxPower part
  rf95.setTxPower(RF95_TxP, false);
  Serial.print("Set TxPower to: "); Serial.println(RF95_TxP);

// Setting bandwidth part
  rf95.setSignalBandwidth((long)RF95_BW);
  Serial.print("Set BW to: "); Serial.println(RF95_BW);
/*
// Setting bandwidth part
  rf95.setSignalBandwidth((long)RF95_BW);
  Serial.print("Set BW to: "); Serial.println(RF95_BW);



  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  // rf95.setTxPower(23, false);
*/
Serial.println("............................................................................"); 
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  //setting payload for packet  Part 
  char message[2] = "3"; // delaytime = 300ms
 // packetnum++;
 // char num[10];
//  char * res;
 // itoa(packetnum, num, 10);
 // strcat(message,num);

  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server

  Serial.print("Sending "); Serial.println(message);

  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)message, sizeof(message));

  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  Serial.println("............................................................................"); 

  /*
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply..."); delay(10);
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
  */
  
  delay(5000);
}

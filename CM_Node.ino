// LoRa-Node1 TX

#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>
using namespace std;

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
//#define RF95_TX_FREQ 450.0
//#define RF95_RX_FREQ 444.
//#define RF95_TX_SF 9
//#define RF95_RX_SF 12

#define RF95_TX_FREQ 470.0
#define RF95_RX_FREQ 470.0
// Change to 125000 / 250000 / 500000 OR other Bandeidth.
#define RF95_BW 250000
// Change to 7、8、9、10、11、12 OR other SF.
#define RF95_TX_SF 9
#define RF95_RX_SF 12
// Change to 8、16、32、64... OR others PreambleLength.
#define RF95_PrL 8
// Change to 5、6、7 OR 8 other CodingRate.
#define RF95_CR 7
// Change TxPower from +2 to +20 (For 18, 19 and 20, PA_DAC is enabled).
#define RF95_TxP 17

//不同demand所需要的不同的时间间隔
#define delaytime1 800
#define delaytime2 500
#define delaytime3 300
#define delaytime4 200
#define delaytime5 133
#define delaytime6 100
#define delaytime7 86
#define delaytime8 50
//#define Node_Number 49 // 节点1
//#define Node_Number 50 // 节点2
//#define Node_Number 51 // 节点3
#define Node_Number 52 // 节点4
#define Collision_flag 53 // 冲突发送beacon
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("TX Test of LoRa's Node with ALHOA!!!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);delay(10);
  digitalWrite(RFM95_RST, HIGH);delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

// Setting frequency part
  rf95.setFrequency(RF95_RX_FREQ); 
  Serial.print("Set Freq to: "); Serial.println(RF95_RX_FREQ);

// Setting SpreadingFactor part
  rf95.setSpreadingFactor((uint8_t)RF95_RX_SF);
  Serial.print("Set SF to: "); Serial.println(RF95_RX_SF);

// Setting bandwidth part
  rf95.setSignalBandwidth((long)RF95_BW);
  Serial.print("Set BW to: "); Serial.println(RF95_BW);

// Setting CodingRate part
  rf95.setCodingRate4((uint8_t)RF95_CR);
  Serial.print("Set CodingRate to: "); Serial.println(RF95_CR);

// Setting PreambleLength part
  rf95.setPreambleLength((uint16_t )RF95_PrL);
  Serial.print("Set PreambleLength to: "); Serial.println(RF95_PrL);

// Setting TxPower part
  rf95.setTxPower(RF95_TxP, false);
  Serial.print("Set TxPower to: "); Serial.println(RF95_TxP);
  
Serial.println("---------------------------");
}

/*参数设置区*/
unsigned long Stime = 0;    //用于程序重启
unsigned long Etime = 0;
int16_t packetnum = 0; // packet counter, we increment per xmission
int flag = 0;         //the flag for TX/RX 
unsigned long DelayTime = 0;

void loop()
{
//waiting beacon part
  if(flag == 0)
  {
    //Serial.println("Waiting Beacon......");
    if(rf95.available())
    {
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

      if (rf95.recv(buf, &len))
      {
        if(len != 2)
        {
//          Serial.print("Got: ");
//          Serial.println((char*)buf);
          Serial.println("---------------------------");
          Serial.println("This is not a legal DelayTimeBeacon !");
          Serial.println("---------------------------");
        }
        else
        {
          if (buf[0] == Node_Number || buf[0] == Collision_flag)
          {
            // 开始发送
            DelayTime = 500;
            flag = 1;
            Serial.println("---------------------------");
            Serial.println("Received Starting Beacon!");
  
            rf95.setFrequency(RF95_TX_FREQ);
            Serial.print("ReSet Freq to: "); Serial.println(RF95_TX_FREQ);
  
            rf95.setSpreadingFactor((uint8_t)RF95_TX_SF);
            Serial.print("ReSet SF to: "); Serial.println(RF95_TX_SF);
  
            Serial.println("---------------------------");
          }       
//          if (buf[0] == 49)
//          {DelayTime = delaytime1;}
//          
//          else if (buf[0] == 50)
//          {DelayTime = delaytime2;}
//
//          else if (buf[0] == 51)
//          {DelayTime = delaytime3;}
//
//          else if (buf[0] == 52)
//          {DelayTime = delaytime4;}
//
//          else if (buf[0] == 53)
//          {DelayTime = delaytime5;}
//
//          else if (buf[0] == 54)
//          {DelayTime = delaytime6;}
//
//          else if (buf[0] == 55)  
//          {DelayTime = delaytime7;}
//
//          else if (buf[0] == 56)
//          {DelayTime = delaytime8;}

          //接收到合法启动信标，可以开启发送阶段。
//          flag = 1;
//          Serial.println("---------------------------");
//          Serial.println("Received Starting Beacon!");
//
//          rf95.setFrequency(RF95_TX_FREQ);
//          Serial.print("ReSet Freq to: "); Serial.println(RF95_TX_FREQ);
//
//          rf95.setSpreadingFactor((uint8_t)RF95_TX_SF);
//          Serial.print("ReSet SF to: "); Serial.println(RF95_TX_SF);
//
//          Serial.println("---------------------------");
          
        }
      }
       else
      {
        Serial.println("---------------------------");
        Serial.println("Receive failed");
        Serial.println("---------------------------");
      }
    }
   }

//Sending part
   else if(flag == 1)
   {
    //setting payload for packet  Part 
    packetnum++;     //发送数量自增记录
    
//  char message[54] = "Lived on;sad,no help.Lived on;sad,no help.Lived on.N1"; // node 1
//    char message[54] = "How daring and hot we are!How daring and hot we are!2";// node 2
//    char message[54] = "Bright light of day,my joy.Bright light of day,my jo3";// node 3
    char message[54] = "In a dormitory,dreamt of room.In a dormitory,dreamt 6";// node 6
    

    // Send a message to rf95_server
    Serial.println("Sending to LoRa_Gateway");

    Serial.print("Sending "); Serial.println(message);

    Serial.println("Sending...");
    rf95.send((uint8_t *)message, sizeof(message));

    Serial.println("Waiting for packet to complete..."); 
    rf95.waitPacketSent();
    
    Serial.print("Sending completed! And this is the ");
    Serial.print(packetnum);
    Serial.println(" time to sending message.");
    Serial.println("---------------------------");

    //开始准备重启操作
    if(packetnum == 1)
    {Stime = millis();}

    Etime = millis() - Stime;
    //运行超过一分钟则回到监听状态
    if(Etime >= 10000)
    {
      flag = 0;  //回到监听状态
      rf95.setFrequency(RF95_RX_FREQ); 
      rf95.setSpreadingFactor((uint8_t)RF95_RX_SF);
      packetnum = 0;  //重置发送次数
    }

    //发送的时间间隔
    delay(DelayTime);
    }

}

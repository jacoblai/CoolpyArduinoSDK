#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[6];
//服务器域名或IP地址
char server[] = "i.icoolpy.com";
//服务器端口号
int port = 1338;
//UserKey用户密钥(必改项)
char ukey[] = "549232b9539207700c7f0361";
//Hub ID(必改项)
char hub[]="1";
//Node ID(必改项)
char node[]="9";
//byte server[] = { 192, 168, 1, 179 };

String instr;
String outstr;

//led light
int led = 7;

void callback(char* topic, byte* payload, unsigned int length) {
  //display all message by serial port
  Serial.print("topic:");
  Serial.println(topic);
  instr = String();
  for(int i=0;i<length;i++){
    char c = payload[i];
    instr += c;
  }
  //显示所有数据
  Serial.println(instr);
  
  //根据内容判断LED的开或关
        if(instr.startsWith("{\"value\":\"1\"}")){
          digitalWrite(led, HIGH);
        }else if(instr.startsWith("{\"value\":\"0\"}")){
          digitalWrite(led, LOW);  
        }
        //end
}

EthernetClient ethClient;
PubSubClient client(server, port, callback, ethClient);

void setup()
{
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  
  Serial.begin(9600);
  mac[0]=random(1, 254);
  mac[1]=random(1, 254);
  mac[2]=random(1, 254);
  mac[3]=random(1, 254);
  mac[4]=random(1, 254);
  mac[5]=random(1, 254);
  if (Ethernet.begin(mac) == 0) {
    for(;;)
      ;
  }
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  String mqttid = String("arduino:");
  mqttid += random(1,99999);
  char charBuf[mqttid.length()+1];
  mqttid.toCharArray(charBuf, mqttid.length()+1);
  if (client.connect(charBuf)) {
    //client.publish("outTopic","hello world");
    outstr = String(ukey);
    outstr += "/hub/";
    outstr += hub;
    outstr += "/node/";
    outstr += node;
    outstr += "/datapoint";
    char charBuf1[outstr.length()+1];
    outstr.toCharArray(charBuf1, outstr.length()+1);
    client.subscribe(charBuf1);
    
    Serial.println("connect ok");
  }
}

void loop()
{
  client.loop();
}


#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[6];
//Server UrL
char server[] = "i.icoolpy.com";
//Server port
int port = 1338;
//mqtt authentication by user key
char ukey[] = "3aa6bdfb-8b94-49e3-ad16";
//Device ID
char device[]="5";
//Sensor ID
char sensor[]="20";
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
  Serial.println(instr);
  
  //only edit in here ^^ begin
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
    outstr += "/device/";
    outstr += device;
    outstr += "/sensor/";
    outstr += sensor;
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


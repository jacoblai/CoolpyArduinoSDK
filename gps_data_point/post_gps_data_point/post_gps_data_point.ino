/*
  Power By Coolpy
  Site:http://icoolpy.com
 
  This example code is in the public domain.
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[6];
//服务器域名或IP地址
char server[] = "i.icoolpy.com";
//服务器端口号
int port = 1337;
//UserKey用户密钥(必改项)
char ukey[] = "549232b9539207700c7f0361";
//Hub ID(必改项)
char hub[]="1";
//Node ID(必改项)
char node[]="9";

EthernetClient client;

void setup() {
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
  
  char buffer[25];
  //修改此处代码修改发送的内容
  //{\"timestamp\":\"2012-03-15T16:13:14\",\"value\":{\"lat\":35.4567,\"lng\":46.1234,\"speed\":98.2,\"offset\":\"yes\"}}
  String json = "";
  //post has key data point
  //json += String("{\"timestamp\":\"2012-03-15T16:13:14\",\"value\":{\"lat\":") 
  //     + dtostrf(35.456712, 10, 6, buffer) + String(",\"lng\":") + dtostrf(46.123456, 10, 6, buffer) + String(",\"speed\":") 
  //     + dtostrf(98.20, 5, 2, buffer) + String(",\"offset\":\"yes\"}}");
  //post no key data point
  json += String("{\"value\":{\"lat\":") 
       + dtostrf(23.189047, 10, 6, buffer) + String(",\"lng\":") + dtostrf(113.28873, 10, 6, buffer) + String(",\"speed\":") 
       + dtostrf(98.20, 5, 2, buffer) + String(",\"offset\":\"yes\"}}");  
  Post(json);
}

String inStr="";
boolean isOnData=false;

void loop()
{
  if (client.available()) {
    char c = client.read();
    if(c == '{'){
     inStr = "";
     isOnData = true;
     inStr += c;
    }else if(c == '}'){
      isOnData = false;
      inStr += c;
      
      //显示请求返回内容
      Serial.println("");
      Serial.println(inStr); 
      
    }else if(isOnData){
      inStr += c;
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}

void Post(String PostData)
{
// if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    // Make a HTTP request:
    client.print("POST /v1.0/hub/");
    client.print(hub);
    client.print("/node/");
    client.print(node);
    client.println("/datapoints HTTP/1.1");
    client.print("Host:");
    client.println(server);
    client.println("Content-Type:application/json");
    client.print("U-ApiKey:");
    client.println(ukey);
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(PostData.length()+1);
    client.println();
    client.println(PostData);
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}


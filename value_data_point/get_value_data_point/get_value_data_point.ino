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
//数据结点Key值(必改项)
char dpkey[] = "2012-03-15T16:13:14";

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
  
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    // Make a HTTP request:
    client.print("GET /v1.0/hub/");
    client.print(hub);
    client.print("/node/");
    client.print(node);
    client.print("/datapoint/");
    client.print(dpkey);
    client.println(" HTTP/1.1");
    client.print("Host:");
    client.println(server);
    client.println("Accept:");
    client.print("U-ApiKey:");
    client.println(ukey);
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

String inStr="";
boolean isOnData=false;
int packer = 0;

void loop()
{
  if (client.available()) {
    char c = client.read();
    if(c == '{'){
      if(!isOnData){
         inStr = "";
         isOnData = true;
      }else{
         packer++;
      }
        inStr += c;
    }else if(c == '}'){
      inStr += c;
      if(packer>0){
        packer--; 
      }else{
       isOnData = false;
       
        //完成操作显示所有数据
        Serial.println("");
        Serial.println(inStr);  
      
      }  
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


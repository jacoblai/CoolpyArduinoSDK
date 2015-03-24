/*
  Power By Coolpy
  Site:http://icoolpy.com
 
  This example code is in the public domain.
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[6];
//Server UrL
char server[] = "api.icoolpy.com";
//Server port
int port = 1337;
//User Access Key
char ukey[] = "54925256cbfa3e7006e59088";
//Device ID
char device[]="1";
//Sensor ID
char sensor[]="3";
//Gen data point key
char dpkey[] = "mygpskey";

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
    client.print("DELETE /v1.0/device/");
    client.print(device);
    client.print("/sensor/");
    client.print(sensor);
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
       
        ///finish read data from coolpy server
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


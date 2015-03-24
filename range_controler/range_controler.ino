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
char sensor[]="5";

EthernetClient client;

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 11*1000;  // delay between updates, in milliseconds

//led light pwm
int led = 6;

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
     //only edit in here ^^ begin
        //if(inStr.startsWith("{\"value\":\"on\"}")){
         // digitalWrite(led, HIGH);
        //}else if(inStr.startsWith("{\"value\":\"off\"}")){
          //digitalWrite(led, LOW);  
        //}
        //end
      }  
    }else if(isOnData){
      inStr += c;
    }
  }
  
  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  
  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() { 
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    // Make a HTTP request:
    client.print("GET /v1.0/device/");
    client.print(device);
    client.print("/sensor/");
    client.print(sensor);
    client.print("/datapoint");
    client.print("?time=");
    client.print(random(1, 50000));
    client.println(" HTTP/1.1");
    client.print("Host:");
    client.println(server);
    client.println("Content-Type:application/json");
    client.print("U-ApiKey:");
    client.println(ukey);
    client.println("Connection: close");
    client.println();
    
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
  
}

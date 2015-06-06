////
//
// General code from http://www.pushingbox.com for Arduino + Ethernet Shield (official)
//
////

#include <SPI.h>
#include <Ethernet.h>

  /////////////////
 // MODIFY HERE //
/////////////////
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };   // Be sure this address is unique in your network

//Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
#define DEVID1 "v376F85117727661"        //Scenario : "The mailbox is open"
#define mopen "APERTA"
#define mclose "CHIUSA"

//Numeric Pin where you connect your switch
#define pinDevid1 3  // Example : the mailbox switch is connect to the Pin 3

// Debug mode
#define DEBUG true
  ///////
 //End//
///////

char serverName[] = "notifrier.appspot.com";
boolean pinDevid1State = false;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  Serial.begin(9600);
  pinMode(pinDevid1, INPUT);
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else{
    Serial.println("Ethernet ready");
  }
  digitalWrite(pinDevid1, HIGH);
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop()
{
      ////
      // Listening for the pinDevid1 state
      ////
      if (digitalRead(pinDevid1) == HIGH && pinDevid1State == false) // switch on pinDevid1 is ON 
      {
        if(DEBUG){Serial.println("pinDevid1 is HIGH");}
        pinDevid1State = true;
        //Sending request to PushingBox when the pin is HIGHT
        sendToPushingBox(mopen);
      }
       if (digitalRead(pinDevid1) == LOW && pinDevid1State == true) // switch on pinDevid1 is OFF
      {
        if(DEBUG){Serial.println("pinDevid1 is LOW");}
        pinDevid1State = false;
        //Sending request to PushingBox when the pin is LOW
        sendToPushingBox(mclose);
      }
      delay(15);
}


//Function for sending the request to PushingBox
void sendToPushingBox(String msg){
    if(DEBUG){Serial.println("connecting...");}

  if (client.connect(serverName, 80)) {
    if(DEBUG){Serial.println("connected");}

    if(DEBUG){Serial.println("sendind request");} 
    client.print("GET /notifry?format="); 
    client.print("json"); 
    client.print("&source="); 
    client.print("5fa58a41ebf19ab430bad3cd8bac6b76"); 
    client.print("&title="); 
    client.print("titARD"); 
    client.print("&message="); 
    client.print(msg);     
    client.println(" HTTP/1.1"); 
    client.print("Host: "); 
    client.println(serverName); 
    client.println("User-Agent: Arduino"); 
    client.println();
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
  
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if(DEBUG){
    if (client.available()) {
    char c = client.read();
    Serial.print(c);
    }
  }

    if(DEBUG){Serial.println();}
    if(DEBUG){Serial.println("disconnecting.");}
    client.stop();
}

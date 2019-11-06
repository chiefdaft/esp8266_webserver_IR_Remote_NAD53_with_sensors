#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "irRemoteNADHexCodes.h"
#include "getWebPage.h"
#define ssid      "your_wifi_SSID"       // WiFi SSID
#define password  "your_wifi_password"  // WiFi password
#define DHTTYPE   DHT22       // DHT type (DHT11, DHT22) D1 = GPIO4 data/ D2 = GPIO5 clock
#define DHTPIN    D4          // DHT Pin - GPIO2
#define IR_LED D7  // ESP8266 GPIO pin to use. Recommended: 4 (D2). - GPIO13

IRsend irsend(IR_LED);   // Set the GPIO to be used to sending the message.
const int REPEATVOL = 4; // Number of volume up or down signal repetitions
                         // changes the volume increase and decrease steps, +1 or -1 step doesn't work well

int repVol = REPEATVOL;
float   t = 0 ;
float   h = 0 ;
float   p = 0;
String etatGpio[4] = {"OFF","OFF","Uit","OFF"};
String function[4] = {"CD/DVD", "FM", "Sat/Cable", "Video 4"};
String functionSelected = function[0];
String powerStatus [2] = {"Uit", "Aan"};
String state = powerStatus[0];

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );

void handleRefresh(){
  t = dht.readTemperature();
  h = dht.readHumidity();
  p = bmp.readPressure() / 100.0F;
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );
}
void handlePower(String valStr) {
  
  if (valStr == "0") {
      state = powerStatus[0];
      irsend.sendPronto(irRemoteNADAHexCodes[0], 76);
  } else if (valStr == "1") {
      state = powerStatus[1];
      irsend.sendPronto(irRemoteNADAHexCodes[1], 76);
  } 
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );

}
void handleFunction(String valStr) {

  if (valStr == "0") { // CD/DVD
      functionSelected = function[0];
      irsend.sendPronto(irRemoteNADAHexCodes[2], 76);
  } else if (valStr == "1") { // FM
      functionSelected = function[1];
      irsend.sendPronto(irRemoteNADAHexCodes[3], 76);
  } else if (valStr == "2") { // Cab/Sat
      functionSelected = function[2];
      irsend.sendPronto(irRemoteNADAHexCodes[4], 76);
  } else if (valStr == "3") { // Video 4
      functionSelected = function[3];
      irsend.sendPronto(irRemoteNADAHexCodes[5], 76);
  } 
  
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );
}
void handleVolume(String valStr) {
  int v = 0;
  if (valStr == "0") { // Mute
      irsend.sendPronto(irRemoteNADAHexCodes[8], 76);
  } else if (valStr == "1") { // Volume down
      irsend.sendPronto(irRemoteNADAHexCodes[6], 76);
  } else if (valStr == "2") { // Volume up
      irsend.sendPronto(irRemoteNADAHexCodes[7], 76);
  } else if (valStr == "3") { // Volume down more
    for (v=0;v<repVol;v++) { 
      irsend.sendPronto(irRemoteNADAHexCodes[6], 76);
    }
  } else if (valStr == "4") { // Volume up more
    for (v=0;v<repVol;v++) { 
      irsend.sendPronto(irRemoteNADAHexCodes[7], 76);
    }
  } else if (valStr == "5") { // Cab/Sat
      if (repVol > 1) repVol--;
  } else if (valStr == "6") { // Video 4
      if (repVol < 9) repVol++;
  } 
  
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );
}
void handleDVD(String valStr) {

  if (valStr == "0") { // Play
      irsend.sendPronto(irRemoteNADAHexCodes[9], 76);
  } else if (valStr == "1") { // Pause
      irsend.sendPronto(irRemoteNADAHexCodes[10], 76);
  } else if (valStr == "2") { // Open/Close
      irsend.sendPronto(irRemoteNADAHexCodes[11], 76);
  } else if (valStr == "3") { // Stop
      irsend.sendPronto(irRemoteNADAHexCodes[12], 76);
  } 
  
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );
}
void handleCursor(String valStr) {
if (functionSelected=="CD/DVD") {
  if (valStr == "up") { // Cursor up
      irsend.sendPronto(irRemoteNADAHexCodes[13], 76);
  } else if (valStr == "down") { // Down
      irsend.sendPronto(irRemoteNADAHexCodes[14], 76);
  } else if (valStr == "left") { // Left
      irsend.sendPronto(irRemoteNADAHexCodes[15], 76);
  } else if (valStr == "right") { // Right
      irsend.sendPronto(irRemoteNADAHexCodes[16], 76);
  } else if (valStr == "enter") { // Enter
      irsend.sendPronto(irRemoteNADAHexCodes[9], 76);
  } else if (valStr == "0") { // 0
      irsend.sendPronto(irRemoteNADAHexCodes[21], 76);
  } else if (valStr == "1") { // 1
      irsend.sendPronto(irRemoteNADAHexCodes[22], 76);
  } else if (valStr == "2") { // 2
      irsend.sendPronto(irRemoteNADAHexCodes[23], 76);
  } else if (valStr == "3") { // 3
      irsend.sendPronto(irRemoteNADAHexCodes[24], 76);
  } else if (valStr == "4") { // 4
      irsend.sendPronto(irRemoteNADAHexCodes[25], 76);
  } else if (valStr == "5") { // 5
      irsend.sendPronto(irRemoteNADAHexCodes[26], 76);
  } else if (valStr == "6") { // 6
      irsend.sendPronto(irRemoteNADAHexCodes[27], 76);
  } else if (valStr == "7") { // 7
      irsend.sendPronto(irRemoteNADAHexCodes[28], 76);
  } else if (valStr == "8") { // 8
      irsend.sendPronto(irRemoteNADAHexCodes[29], 76);
  } else if (valStr == "9") { // 9
      irsend.sendPronto(irRemoteNADAHexCodes[30], 76);
  } else if (valStr == "skipbw") { // Skip Backward DVD
      irsend.sendPronto(irRemoteNADAHexCodes[31], 76);
  } else if (valStr == "skipfw") { // Skip Forward DVD
      irsend.sendPronto(irRemoteNADAHexCodes[32], 76);
  }
}
if (functionSelected=="FM") {
  if (valStr == "up") { // Cursor up
      irsend.sendPronto(irRemoteNADAHexCodes[20], 76);
  } else if (valStr == "down") { // Down
      irsend.sendPronto(irRemoteNADAHexCodes[19], 76);
  } else if (valStr == "left") { // Left
      irsend.sendPronto(irRemoteNADAHexCodes[19], 76);
  } else if (valStr == "right") { // Right
      irsend.sendPronto(irRemoteNADAHexCodes[20], 76);
  } else if (valStr == "enter") { // Enter
      irsend.sendPronto(irRemoteNADAHexCodes[18], 76);
  } else if (valStr == "0") { // 0
      irsend.sendPronto(irRemoteNADAHexCodes[33], 76);
  } else if (valStr == "1") { // 1
      irsend.sendPronto(irRemoteNADAHexCodes[34], 76);
  } else if (valStr == "2") { // 2
      irsend.sendPronto(irRemoteNADAHexCodes[35], 76);
  } else if (valStr == "3") { // 3
      irsend.sendPronto(irRemoteNADAHexCodes[36], 76);
  } else if (valStr == "4") { // 4
      irsend.sendPronto(irRemoteNADAHexCodes[37], 76);
  } else if (valStr == "5") { // 5
      irsend.sendPronto(irRemoteNADAHexCodes[38], 76);
  } else if (valStr == "6") { // 6
      irsend.sendPronto(irRemoteNADAHexCodes[39], 76);
  } else if (valStr == "7") { // 7
      irsend.sendPronto(irRemoteNADAHexCodes[40], 76);
  } else if (valStr == "8") { // 8
      irsend.sendPronto(irRemoteNADAHexCodes[41], 76);
  } else if (valStr == "9") { // 9
      irsend.sendPronto(irRemoteNADAHexCodes[42], 76);
  }
}
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );
}
void handleSource(String valStr) {

  if (valStr == "0") { // Cursor up
      irsend.sendPronto(irRemoteNADAHexCodes[18], 76);
  } else if (valStr == "up") { // Down
      irsend.sendPronto(irRemoteNADAHexCodes[19], 76);
  } else if (valStr == "down") { // Left
      irsend.sendPronto(irRemoteNADAHexCodes[20], 76);
  }   
  server.send ( 200, "text/html", getWebPage(&t,&p,&h,&repVol,state,functionSelected) );
}
void handleRoot(){ 
  Serial.print("Posted args: " );
  Serial.print(server.args());
  Serial.print("; Argument: ");
  Serial.print(server.argName(0));
  Serial.print(" waarde = ");
  Serial.println(server.arg(0));

  if (server.hasArg("power"))         handlePower(server.arg(0));
  else if (server.hasArg("function")) handleFunction(server.arg(0));
  else if (server.hasArg("volume"))   handleVolume(server.arg(0));
  else if (server.hasArg("dvd"))      handleDVD(server.arg(0));
  else if (server.hasArg("source"))   handleSource(server.arg(0));
  else if (server.hasArg("cursor"))   handleCursor(server.arg(0));
  else                                handleRefresh();

}

void setup() {
// Open serial port for debugging purposes  
  irsend.begin();
  Serial.begin ( 115200 );
  
  // Init BMP180
  if ( !bmp.begin() ) {
    Serial.println("BMP180 KO!");
    while(1);
  } else {
    Serial.println("BMP180 OK");
  }
  
  WiFi.begin ( ssid, password );
  // Wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  // WiFi connection is OK
  Serial.println ( "" ); 
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );
 
  // Link to the function that manages the launch page 
  server.on ( "/", handleRoot );
 
  server.begin();
  Serial.println ( "HTTP server started" );
  
}
 
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  t = dht.readTemperature();
  h = dht.readHumidity();
  p = bmp.readPressure() / 100.0F;
 
  delay(750);
}

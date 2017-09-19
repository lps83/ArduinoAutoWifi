/*
 * Written by Pierson Sylvain
 * 
 * 
 */

#include <EEPROM.h>
#include "wificonf.h" 
#include <ESP8266WiFi.h>


char ssid[60];
char Password[60];

const char* host = "google.com";

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Just Start");
  
  EEPROM.begin(512);

  Serial.println(EEPROM.read(0));
  if(EEPROM.read(0) == 255 ){
    // Configuration Wifi
    configwifi();
  }

  int i=1;
  while(EEPROM.read(i)!= 255){
  ssid[i-1] = char(EEPROM.read(i));
  i++;
  }
  i = 255;
  while(EEPROM.read(i)!= 255){
  Password[i-255] = char(EEPROM.read(i));
  i++;
  }

  // Test de co

   delay(10);

Serial.println("ssid");
Serial.println(ssid);
Serial.println("Password");
Serial.write(Password);

WiFi.begin(ssid, Password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    delay(500);

}

void loop() {



// Suite test Co
 Serial.print("connecting to ");
Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/";

  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
    delay(5000);


}

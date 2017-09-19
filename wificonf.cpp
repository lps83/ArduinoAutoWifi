/*
   Source by : M. Ray Burnette 20150831
   Modify by Pierson Sylvain
   */

#include "wificonf.h" 
#include <ESP8266WiFi.h>
#include "./DNSServer.h"                  // Patched lib
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const byte        DNS_PORT = 53;          // Capture DNS requests on port 53
IPAddress         apIP(10, 10, 10, 1);    // Private network for server
DNSServer         dnsServer;              // Create the DNS object
ESP8266WebServer  webServer(80);          // HTTP server

String responseHTML = "<title></title>"
"<style></style></head>"
"<body>"
"<div id=""header"">"
"  <div id=""header_c"">"
"    <div id=""top"">"
"      <div id=""top-menu""></div>"
"    </div>"
"  </div>"
"  <div style=""clear: both""></div>"
"</div>"
"<div style=""background:url(../im/bg3.gif) repeat-x; height: 800px"">"
"  <div style=""width:400px; margin:auto; text-align: center"">"
"    <div style=""width:400px; margin:auto"">"
"      <div style=""font-size:21px; color:#666; padding: 10px 0px 0px 0px"" >CONFIGURATION DU SERVICE <span style=""color: #dd0000"">BOX WiFi</span></div>"
"      <br>"
"      <div id=""block_2"">"
"        <p>Merci d'indiquer vos identifiants WiFi<br>"
"        </p><form id=""form1"" name=""form1"" method=""post"">"
"          "
"          <label for=""ssid"" style=""display:block; width:100px; float:left; text-align:right; padding-right:15px; font-weight:bold; padding: 10px""> Ssid</label>"
"          "
"          <input name=""ssid"" id=""ssid"" style=""display:block; width:200px; float:left; font-size: 16px; padding: 5px; font-weight: bold"" value="""">"
"          <br>"
"          <br>"
"          <br>"
"          <label for=""password"" style=""display:block; width:100px; float:left; text-align:right; padding-right:15px; font-weight:bold; padding: 10px""> MOT DE PASSE</label>"
"          <input type=""password"" name=""password"" id=""password"" style=""display:block; width:200px; float:left; font-size: 16px; padding: 5px; font-weight: bold"" value="""">"
"          <br>"
"          <br>"
"          <br>"
"          <input name=""submit"" type=""submit"" value=""Valider"" style=""width:100px"">"
"        </form>"
"         <div style=""clear: both""></div> "
"      </div>"
"  </div>"
"</div>"
"</body></html>";
                      


void configwifi() {
   Serial.println("just Start config");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Wifi BOX");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();


while(1){
  dnsServer.processNextRequest();
  webServer.handleClient();

int nbargs = webServer.args();
String ssid;
String Password;

if(nbargs && nbargs == 2){
  Serial.print(nbargs);
  ssid = webServer.arg(0);
  Password = webServer.arg(1);
  Serial.print(ssid);
  Serial.print(Password);
  delay(1000);

  webServer.send(200, "text/html", "Mise en memoire");

  int adr = 1;
  int i = 0;

  while(ssid[i] != '\0' ){
    EEPROM.write(adr++, ssid[i]);
    Serial.println(ssid[i]);
    i++;
  }

while(adr < 255 ){
  EEPROM.write(adr++, 255);
  delay(10);
}

adr = 255;
i = 0;

  while(Password[i] != '\0' ){
    EEPROM.write(adr++, Password[i]);
    Serial.println(Password[i]);
    i++;
  }

  while(adr < 511 ){
  EEPROM.write(adr++, 255);
  delay(10);
}

  EEPROM.write(0, 0);
EEPROM.commit();

ESP.reset();




    }
  }
}




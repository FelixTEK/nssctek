#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

//import IKB-1 library
#include "IKB1.h"

//Set variable to IKB1
IKB1 ikb1;
const char* ssid     = "NSSCTEK02";
const char* password = "314159265";

IPAddress local_ip(192,168,5,1);
IPAddress gateway(192,168,5,2);
IPAddress subnet(255,255,255,0);

// Set web server port number to 80
WebServer server(80);

// Variable to store the HTTP request
const String webpage = "<!DOCTYPE html>\n<html>  \n  <head>\n    <meta charset=\"utf-8\">\n\t<style>\n\t.myButton {\n\t\tbackground-color:#44c767;\n\t\t-webkit-border-radius:18px;\n\t\t-moz-border-radius:18px;\n\t\tborder-radius:18px;\n\t\tdisplay:inline-block;\n\t\tcursor:pointer;\n\t\tcolor:#ffffff;\n\t\tfont-family:Arial;\n\t\tfont-size:20px;\n\t\tfont-weight:bold;\n\t\twidth:160px;\n\t\tpadding:20px 18px;\n\t\ttext-decoration:none;\n\t\ttext-shadow:2px 4px 0px #2f6627;\n\t\t}\n\t.myButton:hover {\n\t\tbackground-color:#5cbf2a;\n\t}\n\t.myButton:active {\n\t\tposition:relative;\n\t\ttop:1px;\n\t}\n\n  </style>\n  </head>\n  <body>\n    <table border=\"0\" cellpadding=\"1\" cellspacing=\"1\" style=\"width:100%\">\n\t\t<tbody>\n\t\t\t<tr>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td><a href=\"/forward\"><input class=\"myButton\" name=\"btnForward\" type=\"button\" value=\"Forward\" /></a></td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t</tr>\n\t\t\t<tr>\n\t\t\t\t<td><a href=\"/left\"><input class=\"myButton\" name=\"btnLeft\" type=\"button\" value=\"LEFT\" /></a></td>\n\t\t\t\t<td><a href=\"/stop\"><input class=\"myButton\" name=\"btnStop\" type=\"button\" value=\"STOP\" /></a></td>\n\t\t\t\t<td><a href=\"/right\"><input class=\"myButton\" name=\"btnRight\" type=\"button\" value=\"RIGHT\" /></a></td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td><a href=\"/cmd1\"><input class=\"myButton\" name=\"btnCommand1\" type=\"button\" value=\"KICK\" /></a></td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t</tr>\n\t\t\t<tr>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td><a href=\"/backward\"><input class=\"myButton\" name=\"btnBackward\" type=\"button\" value=\"Backward\" /></a></td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t\t<td><a href=\"/cmd2\"><input class=\"myButton\" name=\"btnCommand2\" type=\"button\" value=\"STOW\" /></a></td>\n\t\t\t\t<td>&nbsp;</td>\n\t\t\t</tr>\n\t\t</tbody>\n\t</table>\n  </body>\n</html>\n";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleForward(){  
  //Serial.println("forward");
  ikb1.fd(100);
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "forward");  
}

void handleBackward(){
  //Serial.println("backward");
  ikb1.bk(100);
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "backward");
}

void handleLeft(){
  //Serial.println("left");
  ikb1.tr(50);
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "left");  
}

void handleRight(){
  //Serial.println("right");
  ikb1.tl(50);
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "right");  
}
void handleStop(){
  //Serial.println("Stop");
  ikb1.ao();
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "stop"); 
}

void handleCommand1(){
  //Serial.println("cmd1");
  ikb1.servo(10,0);
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "cmd1"); 
}

void handleCommand2(){
  //Serial.println("cmd2");
  ikb1.servo(10,90);
  server.sendHeader("Location", "/",true);  
  server.send(302, "text/plain", "");
  //server.send(200, "text/plain", "cmd2"); 
}

void setup()
{
    //reset IKB-1
  ikb1.reset();
  
  Serial.begin(115200);
  //Serial.println("\n[*] Creating AP");

  WiFi.mode(WIFI_AP);
/*  
void WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  (IPAddress) local_ip – กำหนดค่า IP ให้กับ ESP32 เมื่อมีอุปกรณ์อื่น ๆ มีเชื่อมต่อกับ ESP32 จะเห็น IP ของ ESP32 ตามค่าพารามิเตอร์ที่กำหนดนี้
  (IPAddress) gateway – กำหนด IP ของ Gateway ให้กับ ESP32
  (IPAddress) subnet – กำหนด IP ของ Subnet ให้กับ ESP32
*/
  
  WiFi.softAPConfig(local_ip, gateway, subnet);
/*
 void WiFi.softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
   (const char*) ssid – ชื่อของ WiFi ที่ต้องการตั้ง เมื่อใช้อุปกรณ์อื่นค้นหาจะแสดงชื่อนี้
   (const char*) passphrase – รหัสผ่านที่ต้องการตั้ง กรณีต้องการให้มีรหัสผ่านต้องมีตัวอักษรที่ตั้งย่างน้อย 8 ตัว มีค่าเริ่มต้นเป็น NULL หรือไม่มีรหัสผ่าน
   (int) channel – ช่องสัญญาณ WiFi ที่ต้องการ มีค่าเริ่มต้นเป็นช่องที่ 1
   (int) ssid_hidden – ซ่อนจากการค้นหา
 */
  WiFi.softAP(ssid, password);

  //Serial.print("[+] AP Created with IP Gateway ");
  //Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/stop", handleStop);
  server.on("/backward", handleBackward);
  server.on("/right", handleRight);
  server.on("/left", handleLeft);
  server.on("/cmd1", handleCommand1);
  server.on("/cmd2", handleCommand2);

  server.onNotFound(handleNotFound);

  server.begin();
  //Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();
}

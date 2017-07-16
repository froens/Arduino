#include<stdlib.h>
#include "LowPower.h"
#include <SoftwareSerial.h>

#define SSID "Froens2TheMaxExtreme"
#define PASS "Iam1337God4Evah"
//#define SSID "NCWLAN"
//#define PASS "netcompany-123"

#define IP "184.106.153.149" // thingspeak.com
#define analogReadPin A0
#define switchPin 13
#define DEFAULT_WAIT_TIME 1000
#define SLEEP_TIME 1200000 // 20 minutes

String GET = "GET /update?key=R2OZ6MJ9UTSEHJ8Y&field1=0&field2=";

SoftwareSerial monitor(8, 9); // RX, TX

void setup()
{
  monitor.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(2000);
  
  pinMode(switchPin, OUTPUT);
}

void loop(){
  digitalWrite(switchPin, HIGH);
  delay(1000);
  
  String respAT = sendDebug("AT", DEFAULT_WAIT_TIME);
  if(respAT.indexOf("OK") == -1) {
    monitor.println("Wifi-module not found");
  }
  
  while(!connectWiFi()) {
    delay(3000);
  }
  
  int soilMoisture = analogRead(analogReadPin);
  updateTemp(String(soilMoisture));
  
  digitalWrite(switchPin, LOW);
  Serial.println("Turned off aux");
  SleepyTime(1200);  
}

void SleepyTime(int seconds) {
  int w_times = (seconds / 8);

  for(int i = 0; i < w_times; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

void updateTemp(String soilMoisture){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  
  String respCIPSTART = sendDebug(cmd, DEFAULT_WAIT_TIME);
  if(respCIPSTART.indexOf("OK") == -1){
    monitor.println("ERROR");
    return;
  }
  
  cmd = GET;
  cmd += soilMoisture;
  cmd += "\r\n";
  
  String respCIPSEND = sendDebug("AT+CIPSEND=" + String(cmd.length()), DEFAULT_WAIT_TIME);
  if(respCIPSEND.indexOf(">") == -1){
    monitor.println("ERROR");
    return;
  }
  
  String respGET = sendDebug(cmd, DEFAULT_WAIT_TIME);
  sendDebug("AT+CIPCLOSE", DEFAULT_WAIT_TIME);
  
  if(respGET.indexOf("OK") == -1){
    monitor.println("RECEIVED: OK");
  }else{
    monitor.println("RECEIVED: Error");
  }
}

String sendDebug(String cmd, int waitTime){
  monitor.print("SEND: ");
  monitor.println(cmd);
  
  Serial.println(cmd);
  delay(waitTime);
  String response = Serial.readString();
  
  monitor.print("Response: ");
  monitor.println();

  return response;
}
 
boolean connectWiFi(){
  String respCWMode = sendDebug("AT+CWMODE=1", DEFAULT_WAIT_TIME);
  if(respCWMode.indexOf("OK") == -1) {
    monitor.println("ERROR");
    return false;
  }
  
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  String respCWJAP = sendDebug(cmd, 5000);
  
  if(respCWJAP.indexOf("OK") == -1){
    monitor.println("ERROR");
    return false;
  }
    
  monitor.println("Connected to Wifi");
  return true;
}

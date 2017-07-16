#include "ESP8266WiFi.h"
#include <Wire.h>

char buffer[20];
char* password = "Iam1337God4Evah";
char* ssid     = "Froens2TheMaxExtreme";
String MyNetworkSSID = "Froens2TheMaxExtreme"; // SSID you want to connect to Same as SSID
bool Fl_MyNetwork = false; // Used to flag specific network has been found
bool Fl_NetworkUP = false; // Used to flag network connected and operational.

extern "C" {
#include "user_interface.h"
}


void setup() {
  Serial.begin(115200);
  delay(2000); // wait for uart to settle and print Espressif blurb..
  // print out all system information
  Serial.print("Heap: "); Serial.println(system_get_free_heap_size());
  Serial.print("Boot Vers: "); Serial.println(system_get_boot_version());
  Serial.print("CPU: "); Serial.println(system_get_cpu_freq());
  Serial.println();
  Serial.println("OLED network Acquire Application Started....");
  //Wire.pins(int sda, int scl), etc
  Wire.pins(0, 2); //on ESP-01.
  Wire.begin();
  //StartUp_OLED(); // Init Oled and fire up!
  //Serial.println("OLED Init...");
  //clear_display();
  //sendStrXY(" DANBICKS WIFI ", 0, 1); // 16 Character max per line with font set
  //sendStrXY("   SCANNER     ", 2, 1);
  //sendStrXY("START-UP ....  ", 4, 1);
  delay(4000);
  Serial.println("Setup done");
}

void Connect()
{
  Serial.println("Starting Process Scanning...");
  Scan_Wifi_Networks();
  delay(2000);

  if (Fl_MyNetwork)
  {
    // Yep we have our network lets try and connect to it..
    Serial.println("MyNetwork has been Found....");
    Serial.println("Attempting Connection to Network..");
    Do_Connect();

    if (Fl_NetworkUP)
    {
      // Connection success
      Serial.println("Connected OK");
      delay(4000);
      IPAddress ip = WiFi.localIP(); // // Convert IP Here
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    }
    else
    {
      // Connection failure
      Serial.println("Not Connected");
      delay(3000);
    }
    else
    {
      // Nope my network not identified in Scan
      Serial.println("Not Connected");
      delay(3000);
    }
}

void loop()
{
  if (!Fl_NetworkUP)
  {
    Connect()
  }
  else
  {
    
  }
  delay(5000);    // Wait a little before trying again
}


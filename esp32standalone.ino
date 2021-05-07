
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid     = "belzebub";
const char* password = "LHL55JHL4PUP";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool block = false;
int timeRead;
int LED_BUILTIN = 2;
int OUT_RELAY = 15;

int PIN_BUTTON = 23;
int button = 0;

int PIN_LED = 22;

void setup() {
  Serial.begin(115200);

  // ------ OUTPUTS -----------
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (OUT_RELAY, OUTPUT);
  pinMode (PIN_LED, OUTPUT);

  // ------ INPUTS ------------
  pinMode (PIN_BUTTON, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  timeClient.begin();
  timeClient.setTimeOffset(7200);

  digitalWrite(OUT_RELAY, HIGH);

}

void loop() {  
  button = digitalRead(PIN_BUTTON);
  Serial.println(button);//prints out liquid level sensor reading

  if(button){
    water(5000);
  }

 
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  
  timeRead = timeClient.getHours();

  // Reset blocking every day at 1AM
  if(timeRead == 1 && block == true){
    block = false;
  }

  if(block == false && timeRead == 12) {
    Serial.println("Time to water");
    block = true;
    water(10000);
  }
    
  delay(500);
}

void water(int duration){
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(OUT_RELAY, LOW);
  delay(duration);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(OUT_RELAY, HIGH);
}

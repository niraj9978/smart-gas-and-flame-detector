// Master side code
#include <WiFiS3.h>
#include <ArduinoHttpClient.h>

//Wifi defineation
const char* ssid = "VodafoneF2CB76";
const char* password = "chdeg2XhyfPkNyct";

//Thingspeak api key details
const char* server = "api.thingspeak.com";
const char* writeAPIKey = "ODQWBWGHAQ9I2450";

//Pin definations
#define FLAME_PIN 3
#define GAS_PIN A0
#define SIGNAL_PIN 4

//Gas threshold
int gasThreshold = 450;


WiFiClient wifi;
HttpClient client(wifi, server, 80);

void setup() {
  pinMode(FLAME_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);

  Serial.begin(115200);

  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void loop() {
  
  int flame = digitalRead(FLAME_PIN);     
  int gasValue = analogRead(GAS_PIN);      

  bool flameDetected = (flame == LOW);
  bool gasDetected   = (gasValue > gasThreshold);

  
  if (flameDetected || gasDetected) {
    digitalWrite(SIGNAL_PIN, HIGH); 
    Serial.println("🔥 ALARM: Gas or Flame Detected");
  } else {
    digitalWrite(SIGNAL_PIN, LOW);
    Serial.println("✅ Safe Condition");
  }


  int flameStatus = flameDetected ? 1 : 0;
  int gasStatus   = gasDetected ? 1 : 0;

//Sending data to thingspeak
  if (WiFi.status() == WL_CONNECTED) {
    String url = "/update?api_key=" + String(writeAPIKey) +
                 "&field1=" + String(gasValue) +
                 "&field2=" + String(flameStatus) +
                 "&field3=" + String(gasStatus);

    client.get(url);

    int statusCode = client.responseStatusCode();
    client.responseBody();  

    Serial.print("ThingSpeak Status Code: ");
    Serial.println(statusCode); 
  } else {
    Serial.println("WiFi Disconnected!");
  }

  
  delay(15000);
}

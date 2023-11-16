#include <ArduinoJson.h>
#include <esp8266wifi.h>
#include <esp8266httpclient.h>
#include <WiFiClient.h>
WiFiClient wifiClient;
// put function declarations here:
HTTPClient http;

const char *ssid = "MujO2Internet_33D5C2";
const char *password = "";

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {

    delay(1000);
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(wifiClient, "http://worldtimeapi.org/api/timezone/Europe/Prague");
    int timeCode = http.GET();
    if (timeCode > 0)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();

    http.begin(wifiClient, "weatherApi");
    int weatherCode = http.GET();
    if (weatherCode > 0)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();
  }

  if (Serial.available())
  {
    // Read the data from the Arduino via Serial
    String data = Serial.readStringUntil('\n'); // Read data until newline character
    StaticJsonDocument<200> jsonDoc;
    deserializeJson(jsonDoc, data);

    // Extract data from the JSON object
    float temperature = jsonDoc["temperature"];
    float humidity = jsonDoc["humidity"];
    int lightLevel = jsonDoc["lightLevel"];
    int co2Level = jsonDoc["co2Level"];
    http.begin(wifiClient, "mojeSite");
    int graphCode = http.GET();
    if (graphCode > 0)
    {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "sem dej params";
      http.POST(httpRequestData);
    }
    http.end();
  }
  delay(200);
}
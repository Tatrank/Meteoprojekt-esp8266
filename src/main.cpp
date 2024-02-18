#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EasyTransfer.h>

#include <esp8266wifi.h>
#include <esp8266httpclient.h>
#include <WiFiClient.h>
WiFiClient wifiClient;
// put function declarations here:
HTTPClient http;
const char *ssid = "MujO2Internet_33D5C2";
const char *password = "FAakfTjY";
const long interval = 3000;
unsigned long previousMillis = 0;
struct SEND_DATA_STRUCTURE {
  String timeJSON; // Adjust the size based on your needs
  String weather;  // Adjust the size based on your needs
} sendData;

struct RECEIVE_DATA_STRUCTURE {
  int temperature;
  int humidity;
  int lightLevel;
  int co2Level;
} receivedData;


EasyTransfer ETin, ETout;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  ETin.begin(details(receivedData), &Serial);
  ETout.begin(details(sendData), &Serial);
}



void mainLogic() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(wifiClient, "http://worldtimeapi.org/api/timezone/Europe/Prague");
    int timeCode = http.GET();

    if (timeCode > 0) {
      String timeJSON = http.getString();
      sendData.timeJSON = timeJSON;
    }

    http.end();



      http.begin(wifiClient, "http://api.openweathermap.org/data/2.5/forecast?lat=49.3039&lon=14.1457&appid=c7751024e682e3d3ed614bf31ae6925f&units=metric");
      int weatherCode = http.GET();

      if (weatherCode > 0) {
        String weather = http.getString();
        sendData.weather = weather;
      }

      http.end();
    }
    ETout.sendData();


  // You can use sendData.temperature, sendData.humidity, etc., for further processing
}
void loop() {
  //make code so the main logic need to be executed every 2 minutes
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

  mainLogic();
  }
  if (ETin.receiveData()) {
    Serial.print(receivedData.co2Level);
  }
}
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SimpleRotary.h>

#include "conf.h"; 
#include "countries.h"; 

LiquidCrystal_I2C lcd(0x3F,16,2);
SimpleRotary rotary(14,12,13);

String prevCases;
String prevDeaths;
String prevRecovered;
String selectedCountry = "ukraine";
int lastUpdate = 0;
int dataIndex = 0;
int lastRotation = 0;
bool needToUpdate = false;


void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  
  lcd.init();
  lcd.backlight();

  connectToWifi(ssid, password);
  getUpdatesFromAPI(selectedCountry);
}



void loop() {
  
  int delta = millis() - lastUpdate;
  int rotationDelta = millis() - lastRotation;
  
  if (needToUpdate == false && delta >= updateInterval) {
    Serial.println("regular update");
    getUpdatesFromAPI(selectedCountry);
  }

  if (needToUpdate == true && rotationDelta >= debounce) {
    Serial.println("rotary update");
    getUpdatesFromAPI(selectedCountry);
    needToUpdate = false;
  }

  byte i, j;
  
  i = rotary.push();
  
  if ( i == 1 ) {
    dataIndex += 1;
    selectedCountry = countryList[dataIndex];
    getUpdatesFromAPI(selectedCountry);
  }

  j = rotary.rotate();
  
  if ( j == 1 ) {
    Serial.println("CW");
    dataIndex = countryIndexCounter(dataIndex, true);
    selectedCountry = countryList[dataIndex];
    updateCountryScreen(selectedCountry);
    lastRotation = millis();
    needToUpdate = true;
  }

  if ( j == 2 ) {
    Serial.println("CW");
    dataIndex = countryIndexCounter(dataIndex, false);
    selectedCountry = countryList[dataIndex];
    updateCountryScreen(selectedCountry);
    lastRotation = millis();
    needToUpdate = true;
  }
}




void getUpdatesFromAPI (String cname) {
  
  String data = getData(cname);
  
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, data);
  JsonObject dataJson = doc.as<JsonObject>();
  
  String country = dataJson["country"];
  String cases = dataJson["cases"];
  String deaths = dataJson["deaths"];
  String recovered = dataJson["recovered"];
  String message = dataJson["message"];

  Serial.println(message);

  while (message != "null") {

    Serial.println("null data received");
    
    dataIndex ++; 
    selectedCountry = countryList[dataIndex];
    
    data = getData(selectedCountry);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, data);
    JsonObject newDataJson = doc.as<JsonObject>();
  
    String newCountry = newDataJson["country"];
    String newCases = newDataJson["cases"];
    String newDeaths = newDataJson["deaths"];
    String newRecovered = newDataJson["recovered"];
    String newMessage = newDataJson["message"];
    
    country = newCountry;
    cases = newCases;
    deaths = newDeaths;
    recovered = newRecovered;
    message = newMessage;
  }

  
  if ((prevRecovered == NULL || prevRecovered != recovered) || (prevDeaths == NULL && prevDeaths != deaths) || (prevCases == NULL || prevCases != cases)) {
    Serial.println("Updating screen");
    
    updateScreen(country, cases, deaths, recovered);

    prevCases = String(cases);
    prevDeaths = String(deaths);
    prevRecovered = String(recovered);
  }  
    
  Serial.println("end of the iteration");
  lastUpdate = millis();
}

#include "ForecastManager.h"
#include "DataManager.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <string.h>
#include "time.h"

ForecastManager::ForecastManager() 
{
  id = 301;
  temperature_outside = 0.0;
  strcpy(city_name, "Oshkosh");
  strcpy(date_and_time, "2024/01/28-18:58:13");
}

bool ForecastManager::begin(char *ssid, char *pass)
{
  // (1) Initiating wifi, like in BasicHttpClient example
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  int cnt = 0;
  Serial.print(F("Waiting for WiFi to connect"));
  while ((WiFi.status() != WL_CONNECTED) && (cnt < 16)) {
    Serial.print(F("."));
    delay(1000);
    cnt++;
  }
  if (cnt == 16) {
    Serial.println("\nERROR: Could not connect to WiFi!");
    // if WiFi is down or for some reason connection does not go through,
    // I do not want to keep restarting - it will drain battery; just move on;
    // it will go to sleep and wake up in 1h;
    //ESP.restart(); // could try to restart ESP once more?
    return false; // just exit function; the main program will continue w/o connection to Internet;
  } else {
    Serial.println(F(" Connected to WiFi!"));
    // init time zone;
    init_time_zone( "CST6CDT,M3.2.0,M11.1.0");
    return true;
  }
}

bool ForecastManager::getForecast(char *id, char *apiKey) 
{
  HTTPClient http;
  http.getStream().setNoDelay(true);
  http.getStream().setTimeout(1);

  char url[256];
  sprintf(url, "https://api.openweathermap.org/data/2.5/forecast?id=%s&units=imperial&cnt=1&APPID=%s", id, apiKey);

  http.begin(url);

  // Dynamic Json from ArduinoJson library
  DynamicJsonDocument doc(30000);

  int httpCode = http.GET();
  if (httpCode == 200) {
    int32_t len = http.getSize();
    if (len > 0) {
      // Try parsing JSON object
      DeserializationError error = deserializeJson(doc, http.getStream());
      // here is is how this API 2.5 version looks like:
      // https://openweathermap.org/current#example_JSON
      // https://openweathermap.org/current#fields_json
      // NOTE: If you want to see quickly how the data from openweathermap.org looks like go to:
      // https://api.openweathermap.org/data/2.5/forecast?id=5263045&units=imperial&cnt=1&APPID=COPY-HERE-YOUR-KEY
      // and you will get something like this:
      // {"cod":"200","message":0,"cnt":1,"list":[{"dt":1705989600,"main":{"temp":31.66,"feels_like":27.63,"temp_min":31.66,"temp_max":32.07,
      //"pressure":1025,"sea_level":1025,"grnd_level":1003,"humidity":92,"temp_kf":-0.23},"weather":[{"id":600,"main":"Snow","description":
      //"light snow","icon":"13n"},{"id":511,"main":"Rain","description":"freezing rain","icon":"13n"}],"clouds":{"all":100},"wind":{"speed":
      //4.05,"deg":180,"gust":7.63},"visibility":7723,"pop":0.38,"snow":{"3h":0.22},"sys":{"pod":"n"},"dt_txt":"2024-01-23 06:00:00"}],

      // If an error happens print it to Serial monitor
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return 0;
      } else {
        // (a) get the forecast id; it will be used to know what icon to draw;
        String forecastString = doc["list"][0]["weather"][0]["id"];
        int forecastInt = forecastString.toInt();
        // assign the converted value to the class id variable
        this->id = forecastInt;
        // (b) get the temperature outside;
        String temperatureString = doc["list"][0]["main"]["temp"];
        float temperatureFloat = temperatureString.toFloat();
        // place it in the local class variable;
        this->temperature_outside = temperatureFloat;
        // (c) get city name
        String citynameString = doc["city"]["name"];
        citynameString.toCharArray(this->city_name, 10); // hardcoded to maximum 10 characters;
      }
    }
  }            
  
  doc.clear();
  http.end();
  return 1;  
}

void ForecastManager::copy_forecast_to_data_manager(DataManager &data_manager)
{
  // place latest readings from Internet of 
  // forecast weather id, temperature outside, and city name 
  // into overall data manager;
  data_manager.updateTempOutside(temperature_outside);
  data_manager.updateWeatherId(id);
  data_manager.updateCityName(city_name); // city name is restricted to 10 characters;
  data_manager.updateDateAndTime(date_and_time); // date and time is restricted to 20 characters;
}

///////////////////////////////////////////////////////////////////////////////
//
// date and time
//
///////////////////////////////////////////////////////////////////////////////

void ForecastManager::set_time_zone(String timezone)
{
  // function to set timezone
  Serial.printf("Setting Timezone to %s...\n", timezone.c_str());
  // now, adjust the TZ; clock settings are adjusted to show the new local time;
  setenv("TZ", timezone.c_str(), 1);
  tzset();
}

void ForecastManager::init_time_zone(String timezone)
{
  // connect to NTP server and adjust timezone;
  struct tm timeinfo;
  Serial.println("Setting up time...");
  configTime(0, 0, "pool.ntp.org"); // first, connect to NTP server, with 0 TZ offset;
  if ( !getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time. Check the WiFi connection...");
  } 
  Serial.println("Got the time from NTP");
  // now, we can set the real timezone;
  set_time_zone(timezone);
}

void ForecastManager::get_date_and_time()
{
  // get date and time; 
  struct tm timeinfo;
  if ( !getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time. Check the WiFi connection...");
  } 
  strftime( date_and_time, sizeof(date_and_time), "%Y/%m/%d-%H:%M:%S", &timeinfo);
  //Serial.println(date_and_time);
}

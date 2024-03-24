#include "DataManager.h"
#include "Arduino.h"
//#include <string.h>

DataManager::DataManager() 
{
  // this is the ctor; clear all entries in the storage arrays;
  for (int i = 0; i < 24; ++i) {
    temperature_outside[i] = 0.0;
    temperature[i] = 0.0; 
    humidity[i] = 0.0;    
    pressure[i] = 0.0;
  }
  id = 301; // by deafault I want it LightRain... :)
  strcpy(city_name, "Oshkosh");
  strcpy(date_and_time, "2024/01/28-18:58:13");
}

void DataManager::setCityName(char a_city_name[11]) 
{ 
  strcpy(city_name, a_city_name); 
}

void DataManager::setDateAndTime(char a_date_and_time[21]) 
{ 
  strcpy(date_and_time, a_date_and_time); 
}

void DataManager::updateIndoorSensorsData(float temp, float hum, float press) 
{
  // Shift the existing data to make room for the new current values
  for (int i = 0; i < 23; ++i) {
    temperature[i] = temperature[i + 1];
    humidity[i] = humidity[i + 1];
    pressure[i] = pressure[i + 1];
  }
  // Update the last elements with the new values
  temperature[23] = temp;
  humidity[23] = hum;
  pressure[23] = press;
}

void DataManager::updateTempOutside(float temp_outside) 
{
  for (int i = 0; i < 23; ++i) {
    temperature_outside[i] = temperature_outside[i + 1];
  }
  // Update the last element with the new value
  temperature_outside[23] = temp_outside;
}

void DataManager::updateWeatherId(int new_id) 
{ 
  id = new_id; 
}

void DataManager::updateCityName(char *cityname)
{
  strcpy(city_name, cityname);
}

void DataManager::updateDateAndTime(char *dateandtime)
{
  strcpy(date_and_time, dateandtime);
}

void DataManager::printAllData() 
{
  // debug purposes;
  Serial.println("Sensor Data:");
  for (int i = 0; i < 24; ++i) {
    Serial.print("Index ");
    Serial.print(i);
    Serial.print(": Temp.Out=");
    Serial.print(temperature_outside[i], 1);
    Serial.print(", Temp=");
    Serial.print(temperature[i], 1);
    Serial.print(", Humidity=");
    Serial.print(humidity[i], 1);
    Serial.print(", Pressure=");
    Serial.println(pressure[i], 1);
  }
  Serial.print("Weather Id: ");
  Serial.println(id);
  Serial.print("City name: ");
  Serial.println(city_name);
  Serial.print("Date and time: ");
  Serial.println(date_and_time);
}
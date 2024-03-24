#pragma once

class DataManager 
{
public:
  DataManager();
  
  float temperature_outside[24]; // outdoor temperature;
  float temperature[24]; // inside
  float humidity[24]; // inside
  float pressure[24]; // inside
  int id; 
  char city_name[11]; // 10 chars + '\0'
  char date_and_time[21]; // 20 chars + '\0'
  
  int get_weather_id() { return id; }
  void set_weather_id(int a_id) { id = a_id; }
  char *getCityName() { return city_name; }
  void setCityName(char a_city_name[11]);
  char *getDateAndTime() { return date_and_time; }
  void setDateAndTime(char a_date_and_time[21]);
  float get_latest_temperature_outside() { return temperature_outside[23]; }
  float get_latest_temperature() { return temperature[23]; }
  float get_latest_humidity() { return humidity[23]; }
  float get_latest_pressure() { return pressure[23]; }

  // functions to update data;
  void updateIndoorSensorsData(float temp, float hum, float press); 
  void updateTempOutside(float temp_outside);
  void updateWeatherId(int new_id);
  void updateCityName(char *cityname);
  void updateDateAndTime(char *dateandtime);
  // debug;
  void printAllData();

private:

};
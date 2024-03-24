#include "Arduino.h"
#include "DataManager.h"

class ForecastManager 
{
public:
  ForecastManager();
  
  // forecast weather id (will determine which icon will be drawn 
  // on display, clouds, sunny, etc.), temperature outside, and city name;
  int id; 
  float temperature_outside; // outside temperature, in F;    
  char city_name[10]; // should be enough to store most city names;
  char date_and_time[20];

  bool begin(char *ssid, char *pass);
  bool getForecast(char *id, char *apiKey);
  void copy_forecast_to_data_manager(DataManager &data_manager);
  // time related;
  void set_time_zone(String timezone);
  void init_time_zone(String timezone);
  void get_date_and_time();

private:

};
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include "DataManager.h"

// NOTE: that this class works with and stores information
// from Indoor sensors, attached to our design!
// Outside temperature, which we also want to show on the
// Inkplate screen is retrieved from the openweathermap.org;

class Sensors {
public:
  Sensors(int dht22Pin, uint8_t type);

  DHT dht22;
  Adafruit_BMP085 bmp; 
  float temperature; // in Fahrenheit;
  float humidity;
  float pressure;
  
  void begin(); 
  void copy_current_readings_to_data_manager(DataManager &data_manager);
  void readIndoorSensors(bool print_to_serial = false);
  void print_indoor_sensors_data();
  float getTemperature();
  float getHumidity();
  float getPressure();

private:

};
#include "sensors.h"
#include "DataManager.h"

Sensors::Sensors(int dht22Pin, uint8_t type) : dht22(dht22Pin, type) 
{
  temperature = 0.0;
  humidity = 0.0;
  pressure = 0.0;
}

void Sensors::begin()
{
  dht22.begin();
  bmp.begin();
}

float Sensors::getTemperature() { return temperature; }
float Sensors::getHumidity() { return humidity; }
float Sensors::getPressure() { return pressure; }

void Sensors::copy_current_readings_to_data_manager(DataManager &data_manager)
{
  data_manager.updateIndoorSensorsData(temperature, humidity, pressure); 
}

void Sensors::readIndoorSensors(bool print_to_serial) 
{
  // (1) Read temperature and humidity; 
  humidity = dht22.readHumidity();
  // Read temperature in Fahrenheit (isFahrenheit = true)
  temperature = dht22.readTemperature(true);

  // (2) Read pressure;
  pressure = bmp.readPressure()/97.5;

  // (3) if debug true, print to serial terminal too;
  if (print_to_serial == true) {
    // (a) check if any reads from DHT22 sensor failed and exit early.
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("ERROR: Failed to read from DHT sensor!"));
    }
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°F"));

    // (b) check reading from pressure sensor BMP085 too;
    if (isnan(pressure)) {
      Serial.println(F("ERROR: Failed to read from BMP085 sensor!"));
    }
    Serial.print(F("Pressure: "));
    Serial.print(pressure);
    Serial.println(F(" mb"));
  }
}

void Sensors::print_indoor_sensors_data()
{
  // for debug purposes;
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.println(F(" °F"));
  Serial.print(F("Pressure: "));
  Serial.print(pressure);
  Serial.println(F(" mb"));
}

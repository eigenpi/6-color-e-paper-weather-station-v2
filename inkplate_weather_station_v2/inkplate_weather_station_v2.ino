#include "sensors.h"
#include "display.h"
#include "SDCardManager.h"
#include "DataManager.h"
#include "ForecastManager.h"
#include "driver/rtc_io.h" // ESP32 library used for deep sleep and RTC wake up pins

// NOTE: do not use any of the pins 12,13,14,15 of the Inkplate display to
// connect to DHT22 sensor because those pins are used by spi2.begin(14, 12, 13, 15)
// to talk to the SD Card!

#define uS_TO_S_FACTOR 1000000     // Conversion factor from micro seconds to seconds
unsigned int TIME_TO_SLEEP = 3600; // How long ESP32 will be in deep sleep (in seconds)
#define DHTPIN 25                  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22              // Type of temperature sensor: DHT 22 (AM2302), AM2321;

char *ssid     = "Your-WiFi-Router-Name"; 
char *password = "Your-Password"; 
char *APIKEY   = "Key-You-Get-From-openweathermap";
char *cityID   = "Your-City-Number-ID"; // This is 5263045 for Milwaukee, USA

// sensors is basically a wrapper/host of the two main
// objects that deal with DHT22 and BMP085 sensors;
Sensors sensors(DHTPIN, DHT22);

// basically a wrapper around Inplate display object;
// note that a reference to that display object will
// need to be passed to the SD Card manager class instance next;
Display display; 

// handling writing and reading from the sd card; is
// maybe (unintuitively) done through the Inkplate display instance;
// argument is reference/adress of Inkplate display object;
SDCardManager sd_card( display.get_inkplate_display()); 

// an all-in-one-place container to store 24 values of each 
// displayed variable; will be shifted left every hour; used
// also to display bar-graph plots and other plots on the display;
// also used for writing and reading from sd card;
DataManager all_data_manager; 

// to connect to the WiFi and openweathermap.org
ForecastManager forecast;

///////////////////////////////////////////////////////////////////////////////
//
// setup
//
///////////////////////////////////////////////////////////////////////////////

void setup() 
{
  bool connection_to_wifi_ok = false;
  Serial.begin(115200);
  Serial.println("\n---- Welcome to Weather Station V2.0 ----");

  display.begin();
  Serial.println("Initialized Inkplate display.");

  // read all data from SD Card;
  Serial.println("Reading all data from SD Card...");
  sd_card.loadAllData( all_data_manager); // all indoor and outdoor information;
  sd_card.read_bootupcount_from_sd_card(); // read also separately bootupcount;
  // at this time data manager should have all Old information loaded from sd_card;
  //all_data_manager.printAllData(); // debug;

  // every 4h, get the Weather Forecast Id, outside temperature, and city name;
  // we do it only every 4h to minimize the amount of times we need
  // to connect to the Internet - the WiFi tranceiver consumes power!
  if ( sd_card.get_bootupcount() %4 == 0) {
    Serial.println("Connecting to WiFi...");
    connection_to_wifi_ok = forecast.begin(ssid, password);
    if (connection_to_wifi_ok == true) {
      forecast.getForecast(cityID, APIKEY);
      forecast.get_date_and_time();
      forecast.copy_forecast_to_data_manager( all_data_manager); // incuding dateandtime;
    }
  }

  // every 1h, update the graphs, but only the Indoor sensors
  // will be updated with current sensor readings; the forecast 
  // and outdoor temperature remain as of last connection to wifi,
  // which is done every 4h;
  sensors.begin(); 
  Serial.println(F("Initialized indoor sensors."));
  // read current Indoor sensors: temperature, humidity, pressure;
  Serial.println("Reading indoor sensors...");
  sensors.readIndoorSensors(); 
  // copy new sensor readings to data manager;
  Serial.println("Copy indoor sensor readings to data manager...");
  sensors.copy_current_readings_to_data_manager( all_data_manager);
  // write all data to SD Card;
  // NOTE: important for this to be done after also reading
  // weather forecast from Internet, so that updated information 
  // every 4h is included too;
  Serial.println("Writing all data to SD Card...");
  sd_card.saveAllData( all_data_manager); // write to sd card all information;
  sd_card.incr_and_write_bootupcount_to_sd_card();
  
  // at this time data manager should have all information (arrays
  // of 24 values) updated; some updates happen every 4h, all others
  // are updated every 1h; also, every 1h these data was written into 
  // sd_card too;
  //sensors.print_indoor_sensors_data(); // debug;
  all_data_manager.printAllData(); // debug;

  // update Inkplate display screen;
  display.printUI();
  display.printCityName( all_data_manager.getCityName());
  display.printLastUpdate( all_data_manager.getDateAndTime());
  display.printTemperature( all_data_manager.get_latest_temperature(), all_data_manager.get_latest_temperature_outside());
  display.printHumidity( all_data_manager.get_latest_humidity());
  display.printPressure( all_data_manager.get_latest_pressure());
  display.printTemperatureGraph( all_data_manager.temperature, all_data_manager.temperature_outside);
  display.printPressureGraph( all_data_manager.pressure);
  display.printHumidityGraph( all_data_manager.humidity);
  display.printWeatherIcon( all_data_manager.id);
  display.update();

  // go to sleep every 1h;
  Serial.println("Going to deep sleep now...");
  sleep();

  Serial.println("Setup done!");
}

///////////////////////////////////////////////////////////////////////////////
//
// loop
//
///////////////////////////////////////////////////////////////////////////////

void loop() 
{
  //delay(2000);
  //sensors.readIndoorSensors(true);
}

void sleep() 
{
  // put sd card to sleep too (turns off a transistor?);
  sd_card.place_sd_card_to_sleep();
  // Activate wake-up timer -- wake up after 3600s here
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Put ESP32 into deep sleep. Program stops here.
  esp_deep_sleep_start();
}

#include "Inkplate.h"

class Display 
{
public:
  Display();

  Inkplate display;
  // will need this to pass pointer to SDCardManager; for 
  // the purpose of writing/reading to SD Card - via 
  // Inkplate display object;
  Inkplate *get_inkplate_display() { return &display; } 

  void begin();
  void update();
  void printUI();
  void printCityName(char city_name[11]);
  void printLastUpdate(char last_update[11]);
  void printTemperature(float temp_in, float temp_out);
  void printHumidity(float humidity);
  void printPressure(float pressure);
  void printTemperatureGraph(float temp_in[], float temp_out[]);
  void printHumidityGraph(float data[]);
  void printPressureGraph(float data[]);
  void printWeatherIcon(int id);

private:
  void printCityNameFrame(int x, int y);
  void printLastUpdateFrame(int x, int y);
  void printFrame();
  void printSmallFrame(int x, int y);
  void printTitle(int x, int y, String text);
  void printDoubleTitle(int x1, int y1, String text1, int x2, int y2, String text2);
  void drawClearWeather();
  void drawFewClouds();
  void drawCloud();
  void drawThunderstorm();
  void drawLightRain();
  void drawHeavyRain();
  void drawLightSnowfall();
  void drawHeavySnowfall();
  void drawFog();
  void drawLightRainWithSun();
};
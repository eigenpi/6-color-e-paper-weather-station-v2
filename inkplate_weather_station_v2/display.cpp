#include "display.h"
#include "icons.h"

Display::Display() 
{
  
}

void Display::begin() 
{
  display.begin();
  display.clearDisplay();  
}

void Display::update() 
{
  display.display();
}

void Display::printUI() 
{
  printFrame();
  printCityNameFrame(6, 6);
  printLastUpdateFrame(6, 416);
  printSmallFrame(38, 34);
  printSmallFrame(319, 34);
  printSmallFrame(38, 241);
  printSmallFrame(319, 241);
  printDoubleTitle(334, 50, "TEMP.IN: ", 334, 76, "TEMP.OUT: "); // 344,50
  printTitle(88, 50, "FORECAST");
  printTitle(60, 257, "PRESSURE.IN");
  printTitle(345, 257, "HUMIDITY.IN");
}

void Display::printFrame() 
{
  // four black rectangles on periphery of screen;
  display.drawRect(0, 0, 600, 448, INKPLATE_BLACK);
  display.drawRect(1, 1, 598, 446, INKPLATE_BLACK);
  display.drawRect(2, 2, 596, 444, INKPLATE_BLACK);
  display.drawRect(3, 3, 594, 442, INKPLATE_BLACK);
}

void Display::printCityNameFrame(int x, int y)
{
  // at the top of the screen we display name of city;
  display.fillRect(x, y, 588, 26, INKPLATE_RED);
  display.setTextColor(INKPLATE_WHITE);
  display.setTextSize(2);
  display.setCursor(200, 12);
  display.print("City: ");
}
void Display::printCityName(char city_name[11]) 
{
  display.setTextColor(INKPLATE_WHITE);
  display.setTextSize(2);
  display.setCursor(264, 12);
  display.print(city_name);
}

void Display::printLastUpdateFrame(int x, int y)
{
  // at the bottom of the screen we display time of last forecast update;
  display.fillRect(x, y, 588, 26, INKPLATE_RED);
  display.setTextColor(INKPLATE_WHITE);
  display.setTextSize(2);
  display.setCursor(100, 422);
  display.print("Last update: ");
}
void Display::printLastUpdate(char last_update[21])
{
  display.setTextColor(INKPLATE_WHITE);
  display.setTextSize(2);
  display.setCursor(248, 422);
  display.print(last_update);
}

void Display::printSmallFrame(int x, int y)
{
   display.drawRoundRect(x, y, 243, 173, 20, INKPLATE_BLACK);
   display.drawRoundRect(x + 1, y + 1, 241, 171, 20, INKPLATE_BLACK);
}

void Display::printTitle(int x, int y, String text)
{
  display.setTextColor(INKPLATE_BLACK);
  display.setTextSize(3);
  display.setCursor(x, y);
  display.print(text);
}
void Display::printDoubleTitle(int x1, int y1, String text1, int x2, int y2, String text2)
{
  display.setTextColor(INKPLATE_BLACK);
  display.setTextSize(2);
  display.setCursor(x1, y1);
  display.print(text1);
  display.setCursor(x2, y2);
  display.print(text2);
}

void Display::printTemperature(float temp_in, float temp_out) 
{
  char buffer[6];
  display.setTextColor(INKPLATE_RED);
  display.setTextSize(2);
  display.setCursor(454, 50);
  dtostrf(temp_in, 4, 1, buffer);
  display.print(buffer);
  display.print(" ");
  display.print((char)247);
  display.print("F");
  display.setTextColor(INKPLATE_BLUE);
  display.setCursor(454, 76);
  dtostrf(temp_out, 4, 1, buffer);
  display.print(buffer);
  display.print(" ");
  display.print((char)247);
  display.print("F");
}

void Display::printTemperatureGraph(float temp_in[], float temp_out[])
{
  int x = 340;
  int y = 190;
  int i = 0;
  int x1=0, x2=0, y1=0, y2=0;

  // (1) do the graph for temp inside;
  float minValue = 0.0;
  float maxValue = 0.0;
  for (i = 0; i < 24; ++i) {
    if (temp_in[i] !=0) {
      minValue = min(minValue, temp_in[i]);
      maxValue = max(maxValue, temp_in[i]);
    }
  }
  // Calculate the width of each data point on the x-axis
  float xStep = 230 / 24;
  float yScale = 80.0 / (maxValue - minValue);
  for (i = 0; i < 23; ++i) {
    x1 = x + i * xStep;
    y1 = y - int((temp_in[i] - minValue) * yScale);
    x2 = x + (i + 1) * xStep;
    y2 = y - int((temp_in[i + 1] - minValue) * yScale);
    display.drawLine(x1, y1, x2, y2, INKPLATE_RED);
    display.drawLine(x1, y1 + 1, x2, y2 + 1, INKPLATE_RED);
    display.drawLine(x1, y1 + 2, x2, y2 + 2, INKPLATE_RED);
  }

  // (2) do the graph for temp outside;
  minValue = 0.0;
  maxValue = 0.0;
  for (i = 0; i < 24; ++i) {
    if (temp_out[i] !=0) {
      minValue = min(minValue, temp_out[i]);
      maxValue = max(maxValue, temp_out[i]);
    }
  }
  // Calculate the width of each data point on the x-axis
  // use xStep and yScale from temp_in;
  for (i = 0; i < 23; ++i) {
    x1 = x + i * xStep;
    y1 = y - int((temp_out[i] - minValue) * yScale);
    x2 = x + (i + 1) * xStep;
    y2 = y - int((temp_out[i + 1] - minValue) * yScale);
    display.drawLine(x1, y1, x2, y2, INKPLATE_BLUE);
    display.drawLine(x1, y1 + 1, x2, y2 + 1, INKPLATE_BLUE);
    display.drawLine(x1, y1 + 2, x2, y2 + 2, INKPLATE_BLUE);
  }
}

void Display::printHumidity(float humidity) 
{
  char buffer[6];
  dtostrf(humidity, 4, 1, buffer);
  display.setTextColor(INKPLATE_BLUE);
  display.setTextSize(4);
  display.setCursor(350, 302);
  display.print(buffer);
  display.print(" ");
  display.print("%");
}

void Display::printPressure(float pressure) 
{
  char buffer[6];
  dtostrf(pressure, 5, 1, buffer);
  display.setTextColor(INKPLATE_GREEN);
  display.setTextSize(4);
  display.setCursor(55, 302);
  display.print(buffer);
  display.print(" mb");
}

void Display::printHumidityGraph(float *data) 
{
  int i = 0;
  int x = 340;
  int y = 410;
  int barWidth = int(180 / 24.0);
  int barHeight = 0;
  int rectX = 0, rectY = 0;
  float maxValue = 1.0;
  for (i = 0; i < 24; ++i) {
    if (data[i] != 0) {
      maxValue = max(maxValue, data[i]);
    }
  }
  for (i = 0; i < 24; ++i) {
    if (data[i] != 0) {
      barHeight = int( 70 * (data[i]/maxValue) ); // scale the height to fit within 70 pixels;
      rectX = x + i * (barWidth + 1); // add 1 pixel for spacing;
      rectY = y - barHeight;
      display.fillRect(rectX, rectY, barWidth, barHeight, INKPLATE_BLUE);
    }
  }
}

void Display::printPressureGraph(float *data) 
{
  int i = 0;
  int x = 60;
  int y = 410;
  int barWidth = int(180 / 24.0);
  int barHeight = 0;
  int rectX = 0, rectY = 0;
  float maxValue = 1.0;
  for (i = 0; i < 24; ++i) {
    if (data[i] != 0) {
      maxValue = max(maxValue, data[i]);
    }
  }
  for (i = 0; i < 24; ++i) {
    if (data[i] != 0) {
      barHeight = int( 70 * (data[i]/maxValue) ); // scale the height to fit within 70 pixels;
      rectX = x + i * (barWidth + 1); // add 1 pixel for spacing;
      rectY = y - barHeight;
      display.fillRect(rectX, rectY, barWidth, barHeight, INKPLATE_GREEN);
    }
  }
}

void Display::printWeatherIcon(int id)
{
  switch(id)
  {
    case 800: drawClearWeather(); break;
    case 801: drawFewClouds(); break;
    case 802: drawFewClouds(); break;
    case 803: drawCloud(); break;
    case 804: drawCloud(); break;

    case 200: drawThunderstorm(); break;
    case 201: drawThunderstorm(); break;
    case 202: drawThunderstorm(); break;
    case 210: drawThunderstorm(); break;
    case 211: drawThunderstorm(); break;
    case 212: drawThunderstorm(); break;
    case 221: drawThunderstorm(); break;
    case 230: drawThunderstorm(); break;
    case 231: drawThunderstorm(); break;
    case 232: drawThunderstorm(); break;

    case 300: drawLightRain(); break;
    case 301: drawLightRain(); break;
    case 302: drawLightRain(); break;
    case 310: drawLightRain(); break;
    case 311: drawLightRain(); break;
    case 312: drawLightRain(); break;
    case 313: drawLightRain(); break;
    case 314: drawLightRain(); break;
    case 321: drawLightRain(); break;

    case 500: drawLightRainWithSun(); break;
    case 501: drawLightRainWithSun(); break;
    case 502: drawLightRainWithSun(); break;
    case 503: drawLightRainWithSun(); break;
    case 504: drawLightRainWithSun(); break;
    case 511: drawLightRain(); break;
    case 520: drawHeavyRain(); break;
    case 521: drawHeavyRain(); break;
    case 522: drawHeavyRain(); break;
    case 531: drawHeavyRain(); break;

    case 600: drawLightSnowfall(); break;
    case 601: drawHeavySnowfall(); break;
    case 602: drawHeavySnowfall(); break;
    case 611: drawLightSnowfall(); break;
    case 612: drawLightSnowfall(); break;
    case 615: drawLightSnowfall(); break;
    case 616: drawLightSnowfall(); break;
    case 620: drawLightSnowfall(); break;
    case 621: drawHeavySnowfall(); break;
    case 622: drawHeavySnowfall(); break;

    case 701: drawFog(); break;
    case 711: drawFog(); break;
    case 721: drawFog(); break;
    case 731: drawFog(); break;
    case 741: drawFog(); break;
    case 751: drawFog(); break;
    case 761: drawFog(); break;
    case 762: drawFog(); break;
    case 771: drawFog(); break;
    case 781: drawFog(); break;

    default:break; 
  }
}

void Display::drawClearWeather(){
  display.fillCircle(155, 140, 45, INKPLATE_ORANGE);
}

void Display::drawFewClouds()
{
  display.drawBitmap(100, 90, cloud, 110, 110, INKPLATE_BLUE);
  display.drawBitmap(100, 90, sun, 110, 110, INKPLATE_ORANGE);
}

void Display::drawCloud() 
{
  display.drawBitmap(100, 70, cloud, 110, 110, INKPLATE_BLUE);
}

void Display::drawThunderstorm() 
{
  display.drawBitmap(100, 80, thunderstorm, 110, 110, INKPLATE_BLUE);
  display.drawBitmap(100, 80, thunder, 110, 110, INKPLATE_ORANGE);
}

void Display::drawLightRain() 
{
  display.drawBitmap(100, 90, light_rain, 110, 110, INKPLATE_BLUE);
}

void Display::drawHeavyRain() 
{
  display.drawBitmap(100, 84, rain, 110, 110, INKPLATE_BLUE);
}

void Display::drawLightSnowfall() 
{
  display.drawBitmap(100, 84, snowfall, 110, 110, INKPLATE_BLUE);
}

void Display::drawHeavySnowfall() 
{
  display.drawBitmap(100, 80, heavy_snowfall, 110, 110, INKPLATE_BLUE);
}

void Display::drawLightRainWithSun() 
{
  display.drawBitmap(100, 84, sun_light_rain, 110, 110, INKPLATE_ORANGE);
  display.drawBitmap(100, 84, light_rain_with_sun, 110, 110, INKPLATE_BLUE);
}

void Display::drawFog() 
{
  display.drawBitmap(100, 84, fog, 110, 110, INKPLATE_BLUE);
}
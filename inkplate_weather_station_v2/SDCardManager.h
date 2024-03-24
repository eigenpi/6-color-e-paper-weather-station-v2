#include "Inkplate.h" 
#include "DataManager.h"
#include "SdFat.h" 

class SDCardManager {
public:
  SDCardManager(Inkplate *display);
  
  // pointer to an object of Inkplate; the object will be created
  // externally in the main sketch, but passed here because it will
  // be used to write and read from SD Card;
  Inkplate *_display; 

  // we will place in one long char array: 
  //   24 float values of Temperature outside
  //   24 float values of Temperature inside 
  //   24 float values of Humidity inside
  //   24 float values of Pressure inside
  // each float number will be translated into 10 chars and concatenated into
  // an overall data_buffer; then, we write this buffer to file on sd card;
  char _data_buffer[961]; // 24 values x 10 chars x 4 variables + '\0'

  int _boot_up_count;
  
  SdFile _sd_file; // SdFile object used for accessing files on SD card;
  char _data_file_name[10]; // "/data.txt"
  char _weather_id_file_name[15]; // "/weatherid.txt"
  char _city_file_name[10]; // "/city.txt"
  char _dateandtime_file_name[17]; // "/dateandtime.txt"
  char _bootupcount_file_name[17]; // "/bootupcount.txt"

  void initialize(); 
  int get_bootupcount() { return _boot_up_count; }
  void set_bootupcount(int new_bootupcount) { _boot_up_count = new_bootupcount; } 
  // reads
  void loadAllData(DataManager &data_manager);
  void read_sensors_data_from_sd_card();
  int read_weather_id_from_sd_card();
  void read_city_name_from_sd_card(char *cityname);
  void read_dateandtime_from_sd_card(char *dateandtime);
  void read_bootupcount_from_sd_card(); 
  // writes
  void saveAllData(DataManager &data_manager); 
  void write_sensors_data_to_sd_card();
  void write_weather_id_to_sd_card(int id);
  void write_city_name_to_sd_card(char *cityname);
  void write_dateandtime_to_sd_card(char *dateandtime);
  void incr_and_write_bootupcount_to_sd_card();
  // deep sleep;
  void place_sd_card_to_sleep() { _display->sdCardSleep(); }
  
private:

};
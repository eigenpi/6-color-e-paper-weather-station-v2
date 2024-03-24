#include "SDCardManager.h"
#include "Arduino.h"

SDCardManager::SDCardManager(Inkplate *display)
{
  _display = display;
  _boot_up_count = 0;
  // hard coded file names to keep things simple;
  strcpy(_data_file_name, "/data.txt");
  strcpy(_weather_id_file_name, "/weatherid.txt");
  strcpy(_city_file_name, "/city.txt");
  strcpy(_dateandtime_file_name, "/dateandtime.txt");
  strcpy(_bootupcount_file_name, "/bootupcount.txt");
}

void SDCardManager::initialize() 
{
  // initialize SD Card; similarly to how things are done with Inkplate code examples:
  // \InkplateLibrary\examples\Inkplate6COLOR\Advanced\SD\Inkplate6COLOR_SD_TXT_Write
  // NOTE: display.sdCardInit depends on having display.begin() already called, and, 
  // it should have been called once only!

  // NOTE: I moved display.sdCardInit into each of the read/write functions below...
}

///////////////////////////////////////////////////////////////////////////////
//
// read from Sd Card functions;
//
///////////////////////////////////////////////////////////////////////////////

void SDCardManager::loadAllData(DataManager &data_manager) 
{
  // NOTE: see the "saveAllData()" for info on how files were written!
  int head_index = 0;
  // read 10 chars at a time for each float number saved into file during writing;
  char temp_buff[11];
  int i = 0, j = 0, k = 0;
  char old_city_name[11];
  char old_dateandtime[21];
  int old_weather_id = 301;

  // (1) read 960 chars from "/data.txt"
  read_sensors_data_from_sd_card();
  head_index = 0;
  for (i = 0; i < 24; ++i) {
    for (j = 0; j < 10; ++j) {
      temp_buff[j] = _data_buffer[head_index];
      head_index++;
    }
    temp_buff[10] = '\0';
    data_manager.temperature_outside[i] = atof(temp_buff); 
  }
  for (i = 0; i < 24; ++i) {
    for (j = 0; j < 10; ++j) {
      temp_buff[j] = _data_buffer[head_index];
      head_index++;
    }
    temp_buff[10] = '\0';
    data_manager.temperature[i] = atof(temp_buff); 
  }
  for (i = 0; i < 24; ++i) {
    for (j = 0; j < 10; ++j) {
      temp_buff[j] = _data_buffer[head_index];
      head_index++;
    }
    temp_buff[10] = '\0';
    data_manager.humidity[i] = atof(temp_buff); 
  }
  for (i = 0; i < 24; ++i) {
    for (j = 0; j < 10; ++j) {
      temp_buff[j] = _data_buffer[head_index];
      head_index++;
    }
    temp_buff[10] = '\0';
    data_manager.pressure[i] = atof(temp_buff); 
  }

  // (2) 
  old_weather_id = read_weather_id_from_sd_card();
  data_manager.set_weather_id( old_weather_id);

  // (3)
  read_city_name_from_sd_card( old_city_name); 
  data_manager.setCityName( old_city_name);

  // (4)
  read_dateandtime_from_sd_card( old_dateandtime);
  data_manager.setDateAndTime( old_dateandtime);
}

void SDCardManager::read_sensors_data_from_sd_card()
{
  int len = 0; // length of opened file in chars/bytes;
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (1)!");
    // open file for reading;
    if (!_sd_file.open( _data_file_name, O_RDONLY)) { // "/data.txt" file
      Serial.printf("ERROR: Cannot open file %s to read from SD Card!\n", _data_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Reading from %s on SD Card...\n", _data_file_name);
      len = _sd_file.fileSize(); // Read how big is file that we are opening
      if (len > 960) { len = 960; } // If it's more than 960 chars, limit to max 960 bytes
      _sd_file.read( _data_buffer, len); // Read data from file and save it in char array
      _data_buffer[len] = '\0'; // Put null terminating char at the and of data
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (1)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

int SDCardManager::read_weather_id_from_sd_card()
{
  char buffer_weatherid[9]; // 8 chars is sufficient to store an int as a string;
  int old_id = 300;
  int len = 0; // length of opened file in chars/bytes;
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (2)!");
    // open file for reading;
    if (!_sd_file.open( _weather_id_file_name, O_RDONLY)) { // "/weatherid.txt" file
      Serial.printf("ERROR: Cannot open file %s to read from SD Card!\n", _weather_id_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Reading from %s on SD Card...\n", _weather_id_file_name);
      len = _sd_file.fileSize(); // Read how big is file that we are opening
      if (len > 8) { len = 8; } // If it's more than 960 chars, limit to max 8 bytes
      _sd_file.read( buffer_weatherid, len); // Read data from file and save it in char array
      buffer_weatherid[len] = '\0'; // Put null terminating char at the and of data
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (2)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
  old_id = atoi(buffer_weatherid); // convert character string to Int;
  Serial.print("Old weather id: ");
  Serial.println(old_id);
  return old_id;
}

void SDCardManager::read_city_name_from_sd_card(char *cityname)
{
  int len = 0; // length of opened file in chars/bytes;
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (3)!");
    // open file for reading;
    if (!_sd_file.open( _city_file_name, O_RDONLY)) { // "/city.txt" file
      Serial.printf("ERROR: Cannot open file %s to read from SD Card!\n", _city_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Reading from %s on SD Card...\n", _city_file_name);
      len = _sd_file.fileSize(); // Read how big is file that we are opening
      if (len > 10) { len = 10; } // If it's more than 10 chars, limit to max 10 bytes
      _sd_file.read( cityname, len); // Read data from file and save it in char array
      cityname[len] = '\0'; // Put null terminating char at the and of data
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (3)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

void SDCardManager::read_dateandtime_from_sd_card(char *dateandtime)
{
  int len = 0; // length of opened file in chars/bytes;
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (4)!");
    // open file for reading;
    if (!_sd_file.open( _dateandtime_file_name, O_RDONLY)) { // "/dateandtime.txt" file
      Serial.printf("ERROR: Cannot open file %s to read from SD Card!\n", _dateandtime_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Reading from %s on SD Card...\n", _dateandtime_file_name);
      len = _sd_file.fileSize(); // Read how big is file that we are opening
      if (len > 20) { len = 20; } // If it's more than 20 chars, limit to max 20 bytes
      _sd_file.read( dateandtime, len); // Read data from file and save it in char array
      dateandtime[len] = '\0'; // Put null terminating char at the and of data
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (4)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

void SDCardManager::read_bootupcount_from_sd_card() 
{
  char buffer_count[9]; // 8 chars is sufficient to store an int as a string;
  int len = 0; // length of opened file in chars/bytes;
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (5)!");
    // open file for reading;
    if (!_sd_file.open( _bootupcount_file_name, O_RDONLY)) { // "/weatherid.txt" file
      Serial.printf("ERROR: Cannot open file %s to read from SD Card!\n", _bootupcount_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Reading from %s on SD Card...\n", _bootupcount_file_name);
      len = _sd_file.fileSize(); // Read how big is file that we are opening
      if (len > 8) { len = 8; } // If it's more than 960 chars, limit to max 8 bytes
      _sd_file.read( buffer_count, len); // Read data from file and save it in char array
      buffer_count[len] = '\0'; // Put null terminating char at the and of data
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (5)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);

  _boot_up_count = atoi(buffer_count); // convert character string to Int;
  Serial.print("Old bootup count: ");
  Serial.println(_boot_up_count);
}

///////////////////////////////////////////////////////////////////////////////
//
// write to Sd Card functions;
//
///////////////////////////////////////////////////////////////////////////////

void SDCardManager::saveAllData(DataManager &data_manager) 
{
  // (1) write sensors data to sd card: "data.txt"
  int head_index = 0; // used to iterate the _data_buffer sequentially;
  // we will use dtostrf, to convert aeach float to a string terminated with '\0';
  // will place the string into a temp buff; size of this buffer must consider:
  //   What the biggest number might ever be 
  //   Size must include space for the “.” and the possible “-” sign
  //   Add 1 for the null-terminating character “\0”
  // so, 10 chars should be enough for actual number, plus one for '\0', which
  // will not be actually included into the overall data buffer;
  char temp_buff[11];
  int i = 0, j = 0, k = 0;

  head_index = 0;
  for (i = 0; i < 24; ++i) {
    dtostrf(data_manager.temperature_outside[i], 10, 2, temp_buff); // sprintf alternative?
    for (j = 0; j < 10; ++j) {
      _data_buffer[head_index] = temp_buff[j];
      head_index++;
    }
  }
  for (i = 0; i < 24; ++i) {
    dtostrf(data_manager.temperature[i], 10, 2, temp_buff);
    for (j = 0; j < 10; ++j) {
      _data_buffer[head_index] = temp_buff[j];
      head_index++;
    }
  }
  for (i = 0; i < 24; ++i) {
    dtostrf(data_manager.humidity[i], 10, 2, temp_buff);
    for (j = 0; j < 10; ++j) {
      _data_buffer[head_index] = temp_buff[j];
      head_index++;
    }
  }
  for (i = 0; i < 24; ++i) {
    dtostrf(data_manager.pressure[i], 10, 2, temp_buff);
    for (j = 0; j < 10; ++j) {
      _data_buffer[head_index] = temp_buff[j];
      head_index++;
    }
  }
  _data_buffer[960] = '\0';
  // debug; 
  //for (k = 0; k < 961; ++k) {
  //  Serial.print(_data_buffer[k]);
  //}

  write_sensors_data_to_sd_card();

  // (2) write weather id to sd card: "/weatherid.txt"
  write_weather_id_to_sd_card( data_manager.get_weather_id());

  // (3) write city name to sd card: "/city.txt"
  write_city_name_to_sd_card( data_manager.getCityName());

  // (4) write date and time to sd card: "/dateandtime.txt"
  write_dateandtime_to_sd_card( data_manager.getDateAndTime());
}

void SDCardManager::write_sensors_data_to_sd_card() 
{ 
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (6)!");
    // open file for writing, creating it if doesn't exist, and deleting any data in the file if it does;
    if (!_sd_file.open( _data_file_name, O_WRITE | O_CREAT | O_TRUNC)) { // create "/data.txt" file
      Serial.printf("ERROR: Cannot open file %s to write on SD Card!\n", _data_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Writing in file %s on SD Card...\n", _data_file_name);
      _sd_file.write( _data_buffer); 
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (6)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

void SDCardManager::write_weather_id_to_sd_card(int id) 
{
  char buffer_weatherid[9]; // 8 chars is sufficient to store an int as a string;
  dtostrf(id, 8, 0, buffer_weatherid);

  // debug; 
  //Serial.println("Buffer weather id:");
  //Serial.print(buffer_weatherid);
  //Serial.println(" ");

  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (7)!");
    if (!_sd_file.open( _weather_id_file_name, O_WRITE | O_CREAT | O_TRUNC)) { // create "/weatherid.txt" file
      Serial.printf("ERROR: Cannot open file %s to write on SD Card!\n", _weather_id_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Writing in file %s on SD Card...\n", _weather_id_file_name);
      _sd_file.write( buffer_weatherid); 
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (7)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

void SDCardManager::write_city_name_to_sd_card(char *cityname) 
{
  // debug; 
  //Serial.println("City name:");
  //Serial.print(cityname);
  //Serial.println(" ");

  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (8)!");
    if (!_sd_file.open( _city_file_name, O_WRITE | O_CREAT | O_TRUNC)) { // create "/city.txt" file
      Serial.printf("ERROR: Cannot open file %s to write on SD Card!\n", _city_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Writing in file %s on SD Card...\n", _city_file_name);
      _sd_file.write( cityname); 
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (8)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

void SDCardManager::write_dateandtime_to_sd_card(char *dateandtime)
{
  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (9)!");
    if (!_sd_file.open( _dateandtime_file_name, O_WRITE | O_CREAT | O_TRUNC)) { // create "/dateandtime.txt" file
      Serial.printf("ERROR: Cannot open file %s to write on SD Card!\n", _dateandtime_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Writing in file %s on SD Card...\n", _dateandtime_file_name);
      _sd_file.write( dateandtime); 
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program; 
    Serial.println("ERROR: SD Card initialization error (9)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

void SDCardManager::incr_and_write_bootupcount_to_sd_card() 
{
  char buffer_count[9]; // 8 chars is sufficient to store an int as a string;

  // increment _boot_up_count and place into char buffer that will be written to file;
  _boot_up_count++;
  if ( _boot_up_count >= 240000) {
    _boot_up_count = 0; // reset bootupcount when it reached too high a value;
  }

  // debug; 
  Serial.print("New bootup count: ");
  Serial.println(_boot_up_count);

  dtostrf( _boot_up_count, 8, 0, buffer_count);

  if ( _display->sdCardInit()) {
    Serial.println("SD Card ok (10)!");   
    if (!_sd_file.open( _bootupcount_file_name, O_WRITE | O_CREAT | O_TRUNC)) { // create "/bootupcount.txt" file
      Serial.printf("ERROR: Cannot open file %s to write on SD Card!\n", _bootupcount_file_name);
      //_display->sdCardSleep();
    } else {
      Serial.printf("Writing in file %s on SD Card...\n", _bootupcount_file_name);
      _sd_file.write( buffer_count); 
      _sd_file.close();
    }
  } else { 
    // card init not successful, put sd card in sleep mode, and stop program;  
    Serial.println("ERROR: SD Card initialization error (10)!");
    //_display->sdCardSleep();
    while (true);
  }
  delay(500);
}

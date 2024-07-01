/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * ALGORITHM HEADER PROGRAM
 * AUTHOR : hanifanhilmana
 * PURPOSE :
 * =>
 * TARGET :
 * =>
 * HISTORY :
 * =>
 * NOTE AND REMARKS :
 * => still in development!!!
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include <stdint.h>
#include "DHT.h"

#define DHT_TYPE DHT11
const byte DHT_PIN = 13;   
const byte MICROPHONE_PIN = 34;

float humidity_data, temperature_data, heat_index_c_data;
int16_t mic_data, mic_data_value;
char tempString[8], humString[8], heatString[8];
String temperatureString, humidityString, heatindexString;
String micDataValue_Str; 
bool hatched_egg;
DHT dht(DHT_PIN, DHT_TYPE);

void sensor_setup(){
  dht.begin();
  pinMode(MICROPHONE_PIN, INPUT);
}

void dht_read() {
  humidity_data = dht.readHumidity();
  temperature_data = dht.readTemperature(); // Read temperature as Celsius (the default)

  if (isnan(humidity_data) || isnan(temperature_data)) { // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  heat_index_c_data = dht.computeHeatIndex(temperature_data, humidity_data, false);// Compute heat index in Celsius (isFahreheit = false)

  dtostrf(temperature_data, 1, 2, tempString); 
  dtostrf(humidity_data, 1, 2, humString); 
  dtostrf(heat_index_c_data, 1, 2, heatString); 

  temperatureString = String(tempString);
  humidityString = String(humString);
  heatindexString = String(heatString);
}

void mic_read(){
  mic_data=analogRead(MICROPHONE_PIN);   
  // Serial.println(mic_data,DEC); 
  mic_data_value = map(mic_data, 0, 4096, -100, 100);
  micDataValue_Str = String(mic_data_value);
  
  if(mic_data_value > 47){
    hatched_egg = true;
  } else {
    hatched_egg = false;
  }
  delay(100);

}
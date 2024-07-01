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
#include "esp32-hal-gpio.h"
#define THINGER_SERIAL_DEBUG
#include <ThingerESP32.h>

#define USERNAME "hanifanhilmana"
#define DEVICE_ID "SmartLivestock_EggIncubator"
#define DEVICE_CREDENTIAL "zgNMKHZpV2iNr7nR"

const char* SSID = "xxxx";
const char* SSID_PASSWORD = "xxxx";

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void ThingerIO_setup(){
  thing.add_wifi(SSID, SSID_PASSWORD);
}

void SendData_Thinger(){
  thing["dht11"] >> [] (pson & out){
    out["temperature"]  = temperature_data;
    out["humidity"]     = humidity_data;
    out["heat index"]   = heat_index_c_data;
  };
  thing["mic"] >> [] (pson& out){
      out = mic_data_value;
  };
  // thing["relay"] >> [] (pson & out){
  //   out["fan"]     = relay_fan_data;
  //   out["heater"]  = relay_heater_data;
  //   out["servo"]   = relay_servo_data;
  // };
  thing["relay_fan"] << (digitalPin(RELAY_FAN_PIN));
  thing["relay_heater"] << (digitalPin(RELAY_HEATER_PIN));
  thing["relay_servo"] << (digitalPin(RELAY_SERVO_PIN));


}
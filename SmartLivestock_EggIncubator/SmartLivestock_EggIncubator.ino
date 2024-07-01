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
#include "Menu.h"

void setup() {
  Serial.begin(115200);
  menu_setup();
  digitalWrite(RELAY_FAN_PIN, LOW);
  digitalWrite(RELAY_HEATER_PIN, HIGH);
  digitalWrite(RELAY_SERVO_PIN, HIGH);
  speedControl();
  SendData_Thinger();
}

void loop() {
  // start_menu();
  //=======================================================================================
  if (!client.connected())
  {
    reconnect();
  }
  dht_read();
  mic_read();
  // relay_read();
  client.loop();

  Serial.print("Publish message");
  Serial.print(", suhu = "); Serial.print(temperature_data);
  client.publish("SmartLivestock/Egg_Incubator/Temperature", temperatureString.c_str());
  Serial.print("°C, kelembaban = "); Serial.print(humidity_data);
  client.publish("SmartLivestock/Egg_Incubator/Humidity", humidityString.c_str());
  Serial.print("%, heat index = "); Serial.print(heat_index_c_data);
  client.publish("SmartLivestock/Egg_Incubator/HeatIndex", heatindexString.c_str());
  Serial.print("°C, mic data = "); Serial.println(micDataValue_Str);
  client.publish("SmartLivestock/Egg_Incubator/MicData", micDataValue_Str.c_str());
  
  thing.handle();
  //=======================================================================================
  // IPAddress ip = WiFi.localIP();
  // String ipStr = ip.toString();
  // Serial.print("IP Address = "); Serial.println(WiFi.localIP());
  // client.publish("iot21222/kelompokg/ipaddress", ipStr.c_str());
  // Serial.print("CHIP ID = "); Serial.println(String(ESP.getFlashChipId()).c_str());
  // client.publish("iot21222/kelompokg/chipid", message.c_str());
}

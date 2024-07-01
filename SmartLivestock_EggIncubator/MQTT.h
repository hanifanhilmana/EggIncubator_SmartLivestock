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
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "xxxx"; // Enter your WiFi name
const char *password = "xxxxxx";  // Enter WiFi password
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT Broker
const char *mqtt_server = "ee.unsoed.ac.id";
// const char *topic = "emqx/esp32";
// const char *mqtt_username = "emqx";
// const char *mqtt_password = "public";
const int mqtt_port = 1883;

void callback(char *topic, unsigned char *payload, int length);

void wifi_mqtt_setup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Connecting to a WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //connecting to a mqtt broker
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");       // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void callback(char *topic, unsigned char *payload, int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  String data = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(RELAY_FAN_PIN, LOW);
    client.publish("SmartLivestock/Egg_Incubator/FanON", "FAN INCUBATOR ON");
    Serial.print("UV LIGHT NYALA\n");
  } else if ((char)payload[0] == '2'){
    digitalWrite(RELAY_FAN_PIN, HIGH);
    client.publish("SmartLivestock/Egg_Incubator/FanOFF", "FAN INCUBATOR OFF");
    Serial.print("UV LIGHT MATI\n");
  } else if ((char)payload[0] == '3'){
    digitalWrite(RELAY_HEATER_PIN, HIGH);
    client.publish("SmartLivestock/Egg_Incubator/HeaterON", "HEATER ON");
    Serial.print("POMPA NYALA\n");
  } else if ((char)payload[0] == '4'){
    digitalWrite(RELAY_HEATER_PIN, LOW);
    client.publish("SmartLivestock/Egg_Incubator/FanOFF", "HEATER OFF");
    Serial.print("POMPA MATI\n");
  } 
  
}

void reconnect(){
   while (!client.connected()){
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("SmartLivestock/Egg_Incubator/Button");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
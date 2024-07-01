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
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AnalogMultiButton.h>
#include "Sensor.h"
#include "Actuator.h"
#include "MQTT.h"
#include "ThingerIO.h"

void main_menu();
void sensor_menu();
void show_dht();
void show_mic();
void control_menu();

const String StrText[] = {"SMART LIVESTOCK", "EGG INCUBATOR", "TEAM:", "REZA", "BAGJA", "SAHILA"};
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte MENU_PIN   = 39;
const byte BUTTONS_TOTAL = 5;
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {50, 400, 1300, 2300, 3950};
const byte BUTTON_NONE = 0;
const byte BUTTON_UP = 1;
const byte BUTTON_DOWN = 2;
const byte BUTTON_LEFT = 3;
const byte BUTTON_SELECT = 4;

byte menu_page_curr, menu_page_prev, menu_page_total;
byte sub_menu_page_curr, sub_menu_page_prev, sub_menu_page_total;
bool status_sub_menu = 0, back = 0, select_menu = 0;

AnalogMultiButton buttons(MENU_PIN, BUTTONS_TOTAL, BUTTONS_VALUES);
// enum BUTTON_MODE {BUTTON_RIGHT_MODE, BUTTON_UP_MODE, BUTTON_DOWN_MODE, BUTTON_LEFT_MODE, BUTTON_SELECT_MODE}

void menu_setup(){
  lcd.begin();
	lcd.backlight();
  pinMode(MENU_PIN, INPUT_PULLDOWN);
  sensor_setup();
  actuator_setup();
  wifi_mqtt_setup();
  ThingerIO_setup();
}

void printStr(String text, int index, int line) {
	if(text.length() > 16) {
		lcd.setCursor(0, 0);
		lcd.print(text);
		lcd.setCursor(0, 1);
		lcd.print(text.substring(17, text.length()));
	}
	else {
		lcd.setCursor(index, line);
		lcd.print(text);
	}
}

void clearLine(int line) {
	lcd.setCursor(0, line);
	lcd.print("                ");
}

void start_menu(){
  byte i;
  String StrTemp, Str;
  Serial.println("Start Menu");
  while(1) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(StrText[0]);
    lcd.setCursor(2, 1);
    lcd.print(StrText[1]);
    for(i = 0; i < StrText[0].length(); i++) {
      for(int x = 0; x < 20; x++) {
        buttons.update();
        if(buttons.onPressAfter(BUTTON_NONE, 5, 250)) {
          main_menu();
        }
        delay(20);
      }
      lcd.clear();
      lcd.setCursor(1, 0);
      StrTemp    = StrText[0];
      StrTemp[i] = '*';
      lcd.print(StrTemp);
      lcd.setCursor(2, 1);
      lcd.print(StrText[1]);
      delay(3);
    }

    for(i = 0; i < StrText[1].length(); i++) {
      for(int x = 0; x < 20; x++) {
        buttons.update();
        if(buttons.onPressAfter(BUTTON_NONE, 5, 250)) {
          main_menu();
        }
        delay(20);
      }
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(StrText[0]);
      lcd.setCursor(2, 1);
      StrTemp    = StrText[1];
      StrTemp[i] = '*';
      lcd.print(StrTemp);
      delay(3);
    }

    for(i = 2; i < 6; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(StrText[i]);
      i++;
      lcd.setCursor(0, 1);
      lcd.print(StrText[i]);
      i++;
      lcd.setCursor(16 - (StrText[i].length()), 0);
      lcd.print(StrText[i]);
      i++;
      lcd.setCursor(16 - (StrText[i].length()), 1);
      lcd.print(StrText[i]);
      for(int x = 0; x < 20; x++) {
        buttons.update();
        if(buttons.onPressAfter(BUTTON_NONE, 5, 250)){ 
          main_menu();
        }
        delay(75);
      }
    }
  }
}

void main_menu() {
  menu_page_curr  = 1; 
  menu_page_prev  = 1;
  menu_page_total = 2;
  select_menu = 0;
  lcd.clear();
  while(1){
    Serial.print("menu page curr: "); Serial.println(menu_page_curr);
    Serial.print("select: "); Serial.println(select_menu);
    printStr(String("Main Menu"), 0, 0);

    // MAIN MENU OPTION +++++++++++++++++++++++++++++++++++++++++++++++++++++
    switch(menu_page_curr) {
      case 1:
        printStr(String("1.Sensor Menu"), 0, 1);
        break;
      case 2:
        printStr(String("2.Control Menu"), 0, 1);
        break;
    }

    // MAIN MENU LOGIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    buttons.update();
    if (buttons.onPressAfter(BUTTON_UP, 25, 200)) {
      clearLine(1);
      menu_page_curr++;
      if(menu_page_curr > menu_page_total) menu_page_curr = 1;
    } 
    else if (buttons.onPressAfter(BUTTON_DOWN, 25, 200)) {
      clearLine(1);
      menu_page_curr--;
      if(menu_page_curr == 0) menu_page_curr = menu_page_total;
    }
    else if (buttons.onPressAfter(BUTTON_LEFT, 25, 200)) {
      start_menu();
    } 
    else if (buttons.onPressAfter(BUTTON_NONE, 25, 200)) {
      select_menu = 1;
    }
    
    // MAIN MENU FEATURE ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(select_menu == 1 && menu_page_curr == 1){
      sensor_menu();
    }else if(select_menu == 1 && menu_page_curr == 2){
      control_menu();
    }
  }
}

void sensor_menu() {
  sub_menu_page_curr = 1;
  sub_menu_page_prev = 1;
  sub_menu_page_total = 2;
  select_menu = 0;
  lcd.clear();
  while(1){
    Serial.print("sub menu page curr: "); Serial.println(sub_menu_page_curr);
    Serial.print("select: "); Serial.println(select_menu);
    printStr(String("Sensor Menu"), 0, 0);

    // SENSOR MENU OPTION +++++++++++++++++++++++++++++++++++++++++++++++++++++
    switch(sub_menu_page_curr){
      case 1:
        printStr(String("1.DHT Sensor"), 0, 1);
        break;
      case 2:
        printStr(String("2.MIC Sensor"), 0, 1);
        break;
    }

    // SENSOR MENU LOGIC +++++++++++++++++++++++++++++++++++++++++++++++++++++
    buttons.update();
    if (buttons.onPressAfter(BUTTON_UP, 25, 200)) {
      clearLine(1);
      sub_menu_page_curr++;
      if(sub_menu_page_curr > sub_menu_page_total) sub_menu_page_curr = 1;
    } 
    else if (buttons.onPressAfter(BUTTON_DOWN, 25, 200)) {
      clearLine(1);
      sub_menu_page_curr--;
      if(sub_menu_page_curr == 0) sub_menu_page_curr = sub_menu_page_total;
    }
    else if (buttons.onPressAfter(BUTTON_LEFT, 25, 200)) {
      main_menu();
    } 
    else if (buttons.onPressAfter(BUTTON_NONE, 25, 200)) {
      select_menu = 1;
    }

    // SENSOR MENU FEATURE +++++++++++++++++++++++++++++++++++++++++++++++++++
    if(select_menu == 1 && sub_menu_page_curr == 1){
      show_dht();
    }else if(select_menu == 1 && sub_menu_page_curr == 2){
      show_mic();
    }
  }
}

void show_dht() {
  lcd.clear();
  while(1){
    buttons.update();
    if(buttons.onPressAfter(BUTTON_LEFT, 5, 25)) sensor_menu();
    lcd.clear();

    dht_read();

    printStr(String("Temperature:"), 0, 0);
    printStr(temperatureString, 12, 0);

    printStr(String("Humidity:"), 0, 1);
    printStr(humidityString, 9, 1);

    delay(250);
  } 
}

void show_mic() {
  lcd.clear();
  while(1){
    buttons.update();
    if(buttons.onPressAfter(BUTTON_LEFT, 5, 25)) sensor_menu();
    
    mic_read();
    printStr(String("Sound Detect:"), 0, 0);

    clearLine(1);
    if(hatched_egg == true){
      printStr(String("Hatched Egg"), 0, 1);
    }else{
      printStr(String("Unhatched Egg"), 0, 1);
    }
    

    delay(150);
  }
}

void control_menu(){

}
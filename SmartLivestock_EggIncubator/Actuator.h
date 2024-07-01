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
#include <ESP32Servo.h>

static const int  SERVO_PIN   = 25;
const byte RELAY_SERVO_PIN    = 14;
const byte RELAY_FAN_PIN      = 26;
const byte RELAY_HEATER_PIN   = 27;

int8_t relay_fan_data, relay_heater_data, relay_servo_data;
// Motor A connections
int16_t enA = 33;
int16_t in1 = 12;
int16_t in2 = 5;
// Motor B connections
int16_t enB = 32;
int16_t in3 = 2;
int16_t in4 = 15;

int16_t motion                = 0;
int16_t FLAT_ANGLE            = 90;
int16_t TILTED_FORWARD_ANGLE  = 120;
int16_t TILTED_BACK_ANGLE     = 70;
// setting PWM properties
const int freq1 = 8000;    // set the frequency for 8kHz
const int resolution1 = 9; // set PWM resolution
const int freq2 = 1000;    // set the frequency for 8kHz
const int resolution2 = 8; // set PWM resolution

Servo servo;  
enum position {tilted_forward, tilted_back, forward_to_flat, back_to_flat};

 
void actuator_setup() {
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo.setPeriodHertz(50);    // standard 50 hz servo
	servo.attach(SERVO_PIN); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
  pinMode(RELAY_SERVO_PIN,  OUTPUT);
  pinMode(RELAY_FAN_PIN,    OUTPUT);
  pinMode(RELAY_HEATER_PIN, OUTPUT);
  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  ledcAttach(enA, freq1, resolution1);
  ledcAttach(enB, freq2, resolution2);
}
void relay_read(){
  // relay_fan_data = digitalRead(RELAY_FAN_PIN);
  // relay_heater_data = digitalRead(RELAY_HEATER_PIN);
  // relay_servo_data = digitalRead(RELAY_SERVO_PIN);
}
void servo_position(position mode) {
  if(mode == tilted_forward){
    for(motion = FLAT_ANGLE; motion <= TILTED_FORWARD_ANGLE; motion += 5) {
      servo.write(motion);
      delay(25);
    }
  }else if(mode == tilted_back){
    for(motion = FLAT_ANGLE; motion >= TILTED_BACK_ANGLE; motion -= 5) {
      servo.write(motion);
      delay(25);
    }
  }else if(mode == forward_to_flat){
    for(motion = TILTED_FORWARD_ANGLE; motion >= FLAT_ANGLE; motion -= 5) {
      servo.write(motion);
      delay(25);
    }
  }else if(mode == back_to_flat){
    for(motion = TILTED_BACK_ANGLE; motion <= FLAT_ANGLE; motion += 5) {
      servo.write(motion);
      delay(25);
    }
  }
}

// This function lets you control spinning direction of motors
void directionControl() {
	// Turn on motor 
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	
	// Now change motor directions
	// digitalWrite(in1, LOW);
	// digitalWrite(in2, HIGH);
	// digitalWrite(in3, LOW);
	// digitalWrite(in4, HIGH);
	
	// Turn off motors
	// digitalWrite(in1, LOW);
	// digitalWrite(in2, LOW);
	// digitalWrite(in3, LOW);
	// digitalWrite(in4, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH); 
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	
	// Accelerate from zero to maximum speed
	for (int i = 0; i < 512; i++) {
		ledcWrite(enA, i);
		ledcWrite(enB, i);
		delay(5);
	}
  // while(1){
  //   ledcWrite(enA, 511);
	//   ledcWrite(enB, 511);
  // }
}
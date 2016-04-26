
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

// create servo object to control a servo
Servo myservo;  

// Create the motor shield object with 
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


// Create the bluefruit object.
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);


void setup() {
  delay(500);

  // set serial speed
  Serial.begin(115200);

  // connect to bluetooth shield
  if ( ! ble.begin() ) {
    Serial.println(F("Couldn't find Bluefruit, make sure it's in CommanD mode & check wiring"));
  }
 
  // wait for connection from iphone
  while (! ble.isConnected() ) {
      delay(500);
  }

  // allow data transmission
  ble.setMode(BLUEFRUIT_MODE_DATA);

  // start motor controller
  AFMS.begin();  
  myMotor->setSpeed(150);

  myservo.attach(10);  // attaches the servo on pin 9 to the servo object
}


void loop() {  
  ble.readline(100);
  
  char val[10];
  if (! strcmp(ble.buffer, "START") ) {
    Serial.println("Starting");
    myMotor->run(FORWARD);
  } else if ( ! strcmp(ble.buffer, "STOP") ) {
    Serial.println("Stopping"); 
    myMotor->run(RELEASE);
  } else if ( ! strcmp(ble.buffer, "REVERSE") ) {
    Serial.println("Reversing"); 
    myMotor->run(BACKWARD);  
  } else if ( sscanf(ble.buffer, "TURN:%s", val ) == 1 ) { 
    Serial.print("Turning: ");
    myservo.write(atoi(val));
  } else {
    Serial.println("Wrong command");
  }
  
}




  


  

    

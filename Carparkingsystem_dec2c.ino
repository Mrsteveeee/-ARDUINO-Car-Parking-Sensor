#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo gate;


const int sensorLeftPin   = A0;  
const int sensorCenterPin = A1;  
const int sensorRightPin  = A2;  
const int servoPin = 9;
const int analogMin = 0;
const int analogMax = 800;
const int distMin   = 0;
const int distMax   = 100;
const int nearThreshold = 30;
const int servoClosed = 0;    
const int servoOpen   = 90;   

bool gateIsUp = false;

void setup() {
  lcd.init();          
  lcd.backlight();     
  lcd.clear();

  gate.attach(servoPin);
  gate.write(servoClosed);

  lcd.setCursor(0,0);
  lcd.print("Smart Parking System");
  delay(1500);
  lcd.clear();
}

int readSensorMapped(int pin) {
  int raw = analogRead(pin);
  raw = constrain(raw, analogMin, analogMax);
  int mappedValue = map(raw, analogMin, analogMax, distMin, distMax);
  // Uncomment if your sensor gives higher values when closer:
  // mappedValue = distMax - mappedValue;
  return mappedValue;
}

void loop() {
  int leftVal   = readSensorMapped(sensorLeftPin);
  int centerVal = readSensorMapped(sensorCenterPin);
  int rightVal  = readSensorMapped(sensorRightPin);


  if (leftVal <= nearThreshold && !gateIsUp) {
    gate.write(servoOpen);  
    gateIsUp = true;
  }

  if (centerVal <= nearThreshold && gateIsUp) {
    gate.write(servoClosed);
    gateIsUp = false;
  }


  lcd.clear();
  lcd.setCursor(0,1);
  if (rightVal <= nearThreshold) {
    lcd.print("Lot Occupied");
  } else {
    lcd.print("Lot Available");
  }

  delay(300);
}

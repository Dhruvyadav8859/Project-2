#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD: typical I2C address is 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int soilMoisturePin = A0;
const int gasSensorPin = A1;
const int pumpPin = 7;
const int trigPin = 8;
const int echoPin = 9;

// Thresholds
const int soilThreshold = 400;  // adjust based on calibration
const int gasThreshold = 300;   // adjust for your sensor

long duration;
int distance;

void setup() {
  pinMode(pumpPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Agri-Tech System");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {
  // Read soil moisture
  int soilMoisture = analogRead(soilMoisturePin);
  
  // Read gas sensor
  int gasLevel = analogRead(gasSensorPin);
  
  // Measure distance with ultrasonic
  distance = getUltrasonicDistance();

  // Water pump control
  if (soilMoisture < soilThreshold) {
    digitalWrite(pumpPin, HIGH);  // Turn pump ON
  } else {
    digitalWrite(pumpPin, LOW);   // Turn pump OFF
  }

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  lcd.print(soilMoisture);
  lcd.setCursor(9, 0);
  lcd.print("G:");
  lcd.print(gasLevel);

  lcd.setCursor(0, 1);
  lcd.print("Dist:");
  lcd.print(distance);
  lcd.print("cm");

  // Fire detection warning
  if (gasLevel > gasThreshold) {
    Serial.println("FIRE DETECTED!");
  }

  delay(1000);
}

int getUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;
  return dist;
}


#include <Servo.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DHT setup
#define DHTPIN 2
#define DHTTYPE DHT11

// Pins
#define RAIN_PIN 3
#define SERVO_PIN 9

// Initialize
DHT dht(DHTPIN, DHTTYPE);
Servo windowServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Settings
int openAngle = 90;
int closeAngle = 0;
float tempThreshold = 30.0;

void setup() {
  Serial.begin(9600);

  dht.begin();
  windowServo.attach(SERVO_PIN);
  pinMode(RAIN_PIN, INPUT);

  lcd.init();
  lcd.backlight();

  Serial.println("===== SMART WINDOW SYSTEM =====");
  Serial.println("System Initializing...");

  lcd.setCursor(0, 0);
  lcd.print("Smart Window");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);

  windowServo.write(closeAngle);

  Serial.println("System Ready!");
  Serial.println("--------------------------------");
}

void loop() {
  float temperature = dht.readTemperature();
  int rainStatus = digitalRead(RAIN_PIN);

  // Check DHT
  if (isnan(temperature)) {
    Serial.println("ERROR: Failed to read DHT sensor!");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    delay(2000);
    return;
  }

  // Print sensor data
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Rain Sensor: ");
  Serial.print(rainStatus == LOW ? "RAIN DETECTED" : "NO RAIN");

  lcd.clear();

  // Display temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C");

  // Decision logic
  if (rainStatus == LOW) {
    windowServo.write(closeAngle);

    Serial.println(" --> ACTION: WINDOW CLOSED (RAIN)");

    lcd.setCursor(0, 1);
    lcd.print("Rain! CLOSED");
  }
  else if (temperature > tempThreshold) {
    windowServo.write(openAngle);

    Serial.println(" --> ACTION: WINDOW OPEN (HOT)");

    lcd.setCursor(0, 1);
    lcd.print("Hot! OPEN");
  }
  else {
    windowServo.write(closeAngle);

    Serial.println(" --> ACTION: WINDOW CLOSED (NORMAL)");

    lcd.setCursor(0, 1);
    lcd.print("Normal CLOSED");
  }

  Serial.println("--------------------------------");

  delay(2000);
}
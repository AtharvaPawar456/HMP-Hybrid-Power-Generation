#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int voltagePin1 = A0;  // First voltage divider connected to A0
const int voltagePin2 = A1;  // Second voltage divider connected to A1
const int buzzerPin = 7;     // Buzzer pin
const int ledPin = 5;        // LED pin

const float referenceVoltage = 5.0;   // Reference voltage for analog pins
const float maxVoltage = 12.0;        // Max voltage after scaling
const float thresholdVoltage = 2.2;   // Threshold for the first voltage (in 0-12V scale)
const unsigned long blinkInterval = 500; // LED blink interval (milliseconds)

unsigned long previousMillis = 0;     // Last time LED was toggled
bool ledState = false;                // LED state

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);   // Buzzer off initially
  digitalWrite(ledPin, LOW);      // LED off initially
  
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  Serial.begin(9600);  // Initialize serial monitor
}

void loop() {
  // Read the voltage from the first and second analog pins
  int sensorValue1 = analogRead(voltagePin1);
  int sensorValue2 = analogRead(voltagePin2);

  // Map the values from 0-1023 (0-5V) to 0-12V for both inputs
  float voltage1 = map(sensorValue1, 0, 1023, 0, maxVoltage * 100) / 100.0;
  float voltage2 = map(sensorValue2, 0, 1023, 0, maxVoltage * 100) / 100.0;

  // Display both voltages on the OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Hybrid Power Generation System");

  display.setCursor(0, 30);
  display.print("Voltage 1: ");

  display.print(voltage1);
  display.println(" V");
  
  display.setCursor(0, 45);  // Move cursor to second line
  display.print("Voltage 2: ");
  display.print(voltage2);
  display.println(" V");

  display.display();

  // Check the first voltage and control the buzzer and LED
  if (voltage1 < thresholdVoltage) {
    digitalWrite(buzzerPin, HIGH);  // Buzzer on
    digitalWrite(ledPin, LOW);      // LED off
  } else {
    digitalWrite(buzzerPin, LOW);   // Buzzer off
    ledBlink();                     // LED blinking
  }

  delay(100);  // Short delay to stabilize readings
}

void ledBlink() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;  // Save the last time LED toggled
    ledState = !ledState;            // Toggle LED state
    digitalWrite(ledPin, ledState);
  }
}

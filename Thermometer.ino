#include <Wire.h>          // Library for I2C communication
#include <RTClib.h>        // Library for DS3231 RTC module
#include <SD.h>            // Library for SD card module

RTC_DS3231 rtc;            // Create an instance of the RTC module
File myFile;               // File object to handle SD card operations

const int lm35Pin = A0;    // LM35 temperature sensor connected to analog pin A0
float temperature;         // Variable to store temperature value

void setup() {
  Serial.begin(9600);      // Initialize serial communication
  Wire.begin();            // Begin I2C communication
  rtc.begin();             // Initialize RTC module

  if (!SD.begin(10)) {     // Initialize SD card module
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
}

void loop() {
  DateTime now = rtc.now();    // Get current date and time from RTC module

  temperature = getTemperature();  // Read temperature from LM35 sensor
  logTemperature(now, temperature); // Log temperature along with date and time
  delay(5000);                // Wait for 5 seconds before logging next reading
}

float getTemperature() {
  int rawValue = analogRead(lm35Pin);  // Read analog value from LM35 sensor
  float millivolts = (rawValue / 1023.0) * 5000;  // Convert to millivolts
  temperature = millivolts / 10;  // Convert millivolts to temperature in Celsius
  return temperature;
}

void logTemperature(DateTime now, float temp) {
  myFile = SD.open("temperature.txt", FILE_WRITE);  // Open file for writing

  if (myFile) {
    myFile.print(now.year(), DEC);      // Log year
    myFile.print('/');
    myFile.print(now.month(), DEC);     // Log month
    myFile.print('/');
    myFile.print(now.day(), DEC);       // Log day
    myFile.print(" ");
    myFile.print(now.hour(), DEC);      // Log hour
    myFile.print(':');
    myFile.print(now.minute(), DEC);    // Log minute
    myFile.print(':');
    myFile.print(now.second(), DEC);    // Log second
    myFile.print(" - ");
    myFile.print(temp);                 // Log temperature
    myFile.println(" °C");

    myFile.close();    // Close the file
    Serial.println("Temperature logged.");
  } else {
    Serial.println("Error opening file for writing.");
  }
}

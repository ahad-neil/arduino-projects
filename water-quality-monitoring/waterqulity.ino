#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define sensor pins and reference voltage
const int TURBIDITY_PIN = A0;
const int TDS_PIN = A1;
const int PH_PIN = A2;
const float REFERENCE_VOLTAGE = 5.0;

// Define sensor value ranges
const float MIN_PH = 6.5;
const float MAX_PH = 8.5;
const float MIN_TURBIDITY = 0; // Adjust as needed
const float MAX_TURBIDITY = 50; // Adjust as needed
const float MIN_TDS = 0; // Adjust as needed
const float MAX_TDS = 500; // Adjust as needed

// Define sensor values
float phValue = 0.0;
float turbidityValue = 0.0;
float tdsValueFinal = 0.0;

// Define LCD parameters
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27, 16 columns, 2 rows

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("pH: -- TDS: --");
  lcd.setCursor(0, 1);
  lcd.print("Turbidity: --");
}

void loop() {
  // Read sensors and update values
  readTurbidity();
  readTDS();
  readPhSensor();

  // Check if any sensor value is out of range
  if (phValue < MIN_PH || phValue > MAX_PH || turbidityValue < MIN_TURBIDITY || turbidityValue > MAX_TURBIDITY || tdsValueFinal < MIN_TDS || tdsValueFinal > MAX_TDS) {
    Serial.println("Water Unsafe!");
  }

  // Print all values in a single line with clear labels
  Serial.print("pH: ");
  Serial.print(phValue);
  Serial.print(" | TDS: ");
  Serial.print(tdsValueFinal);
  Serial.print(" | Turbidity: ");
  Serial.println(turbidityValue);

  // Update LCD display
  lcd.setCursor(4, 0);
  lcd.print(phValue);
  lcd.setCursor(13, 0);
  lcd.print(tdsValueFinal);
  lcd.setCursor(11, 1);
  lcd.print(turbidityValue);

  // Delay between readings
  delay(1000);
}

void readTurbidity() {
  int sensorValue = analogRead(TURBIDITY_PIN);
  turbidityValue = map(sensorValue, 0, 1023, 0, 100); // Assuming sensor range is 0-100 NTU
}

void readTDS() {
  int sensorValue = analogRead(TDS_PIN);
  float voltage = (sensorValue / 1023.0) * REFERENCE_VOLTAGE;
  tdsValueFinal = voltage * 100; // Example conversion formula
}

void readPhSensor() {
  int sensorValue = analogRead(PH_PIN);
  float voltage = (sensorValue / 1023.0) * REFERENCE_VOLTAGE;
  phValue = 2.5 * voltage;
}

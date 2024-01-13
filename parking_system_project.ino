#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);
Servo gateServo;

#define NUM_SPOTS 6
#define ENTER_SENSOR_PIN 8
#define EXIT_SENSOR_PIN 9
#define GATE_SERVO_PIN 10

int irSensors[NUM_SPOTS] = {2, 3, 4, 5, 6, 7};
int gateEnterSensor = ENTER_SENSOR_PIN;
int gateExitSensor = EXIT_SENSOR_PIN;
int spotStatus[NUM_SPOTS] = {0};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Parking status empty");

  pinMode(gateEnterSensor, INPUT);
  pinMode(gateExitSensor, INPUT);

  for (int i = 0; i < NUM_SPOTS; i++) {
    pinMode(irSensors[i], INPUT_PULLUP);
  }

  gateServo.attach(GATE_SERVO_PIN); 
  closeGate();
}

void loop() {

  int enterSensorValue = digitalRead(gateEnterSensor);
  int exitSensorValue = digitalRead(gateExitSensor);

  if (exitSensorValue == HIGH) {
    openGate(); 
  } 
  else {
    closeGate();
    }
  if (!AllSpotsFull() && enterSensorValue == HIGH) {
      openGate();
    }
    
  checkSpotOccupancy();

  delay(200);
}

void checkSpotOccupancy() {
  for (int i = 0; i < NUM_SPOTS; i++) {
    int sensorValue = digitalRead(irSensors[i]);

    if (sensorValue == LOW && spotStatus[i] == 0) {
      spotStatus[i] = 1;
      updateLCD();
    } else if (sensorValue == HIGH && spotStatus[i] == 1) {
      spotStatus[i] = 0;
      updateLCD();
    }
  }
}

void checkGateStatus() {
  
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Parking Status");

 
lcd.setCursor(0, 1);
lcd.print("S1:");
lcd.print(digitalRead(irSensors[0]) == LOW ? "full  " : "empty ");

lcd.setCursor(0, 2);
lcd.print("S2:");
lcd.print(digitalRead(irSensors[1]) == LOW ? "full  " : "empty ");

lcd.setCursor(0, 3);
lcd.print("S3:");
lcd.print(digitalRead(irSensors[2]) == LOW ? "full  " : "empty ");

lcd.setCursor(10, 1);
lcd.print("S4:");
lcd.print(digitalRead(irSensors[3]) == LOW ? "full  " : "empty ");

lcd.setCursor(10, 2);
lcd.print("S5:");
lcd.print(digitalRead(irSensors[4]) == LOW ? "full  " : "empty ");

lcd.setCursor(10, 3);
lcd.print("S6:");
lcd.print(digitalRead(irSensors[5]) == LOW ? "full  " : "empty ");

  if (AllSpotsFull()) {
    lcd.setCursor(0, 0);
    lcd.print("Parking Status: Full");
  }
}

bool AllSpotsFull() {
  for (int i = 0; i < NUM_SPOTS; i++) {
    if (spotStatus[i] == 0) {
      return false;  
    }
  }
  return true;  
}

void openGate() {
  gateServo.write(90);  
}

void closeGate() {
  gateServo.write(0); 
}

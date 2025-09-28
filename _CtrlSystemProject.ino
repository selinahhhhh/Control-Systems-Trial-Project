
#include <Wire.h>
#include <Servo.h>

const int MPU = 0x68;   // MPU6050 I2C address
Servo myServo;
int servoPin = 9;      
int offset = 0;       

void setup() {
  Wire.begin();
  Serial.begin(9600);
  myServo.attach(servoPin);


  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
}

void loop() {
  // --- 1. Read accelerometer data ---
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);        // Starting register for accel data
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  int16_t AcX = (Wire.read() << 8) | Wire.read();
  int16_t AcY = (Wire.read() << 8) | Wire.read();
  int16_t AcZ = (Wire.read() << 8) | Wire.read();

  // --- 2. Calculate tilt angle (in degrees) ---
  float angle = atan2(AcX, AcZ) * 180.0 / PI;

  // --- 3. offset ---
  float correctedAngle = angle + offset;

  // --- 4. Map angle to servo range (0–180) ---
  int servoPos = map(correctedAngle, -90, 90, 0, 180);
  servoPos = constrain(servoPos, 0, 180);

  myServo.write(servoPos);

  
  Serial.print("Angle: "); Serial.print(angle);
  Serial.print(" | Offset: "); Serial.print(offset);
  Serial.print(" | Servo: "); Serial.println(servoPos);

  // --- 6. Check if user typed new offset ---
  if (Serial.available() > 0) {
    offset = Serial.parseInt(); // read number from Serial Monitor
    Serial.print("New offset set to: ");
    Serial.println(offset);
  }

  delay(100); 
}

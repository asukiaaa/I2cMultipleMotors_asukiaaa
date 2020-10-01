#include <I2cMultipleMotors_asukiaaa.h>
#define ADDRESS_MOTORS 0x51
#define NUMBER_MOTORS 3

I2cMultipleMotors_asukiaaa::Driver motors(ADDRESS_MOTORS, NUMBER_MOTORS);
I2cMultipleMotors_asukiaaa::Info motorsInfo(NUMBER_MOTORS);

void setup() {
  motors.begin();
  Serial.begin(9600);
  Serial.println("Start");
  motorsInfo.motors[1].speed = 0xff;
}

void loop() {
  motorsInfo.motors[0].speed = millis() / 1000 / 60;
  motorsInfo.motors[0].byteWritable = millis() / 1000 % 60;

  motors.write(motorsInfo);
  motors.read(&motorsInfo);
  if (motorsInfo.stateRead == 0) {
    Serial.println("index: reverse brake speed byteWritable byteReadOnly");
    for (int i = 0; i < NUMBER_MOTORS; ++i) {
      I2cMultipleMotors_asukiaaa::MotorInfo motor = motorsInfo.motors[i];
      Serial.println(String(i) + ": " +
                     String(motor.brake) + " " +
                     String(motor.speed) + " " +
                     String(motor.byteWritable) + " " +
                     String(motor.byteReadOnly));
    }
  } else {
    Serial.println("Cannot read because error " + String(motorsInfo.stateRead));
  }
  Serial.println("at " + String(millis()));

  delay(1000);
}

#include <I2cMultipleMotors_asukiaaa.h>
#define MOTORS_ADDRESS 0x51
#define NUMBER_MOTORS 3

I2cMultipleMotors_asukiaaa motors(MOTORS_ADDRESS, NUMBER_MOTORS);
I2cMultipleMotors_asukiaaa_info motorsInfo(NUMBER_MOTORS);

void setup() {
  motors.begin();
  Serial.begin(9600);
  Serial.println("Start");
  motorsInfo.motors[1].speed = 0xff;
}

void loop() {
  motorsInfo.motors[0].speed = millis() / 1000 % 0xff;
  motorsInfo.motors[1].reverse = !motorsInfo.motors[1].reverse;

  motors.write(motorsInfo);
  motors.read(&motorsInfo);
  if (motorsInfo.stateRead == 0) {
    Serial.println("index: reverse brake speed");
    for (int i = 0; i < NUMBER_MOTORS; ++i) {
      I2cMultipleMotors_asukiaaa_motor_info motor = motorsInfo.motors[i];
      Serial.println(String(i) + ": " +
                     String(motor.reverse) + " " +
                     String(motor.brake) + " " +
                     String(motor.speed));
    }
  } else {
    Serial.println("Cannot read because error " + String(motorsInfo.stateRead));
  }
  Serial.println("at " + String(millis()));

  delay(1000);
}

#include <I2cMultipleMotors_asukiaaa.h>
#include <utils_asukiaaa.h>
#include <utils_asukiaaa/wire.h>
#define MOTORS_ADDRESS 0x51
#define NUMBER_MOTORS 3

bool prohibitWriting (int index) {
  return index % I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR == 2;
}
I2cMultipleMotors_asukiaaa_info motorsInfo(NUMBER_MOTORS);
const int registerLen = I2cMultipleMotors_asukiaaa::getArrLenFromNumberMotors(NUMBER_MOTORS);
utils_asukiaaa::wire::PeripheralHandler wirePeri(&Wire, registerLen, prohibitWriting);
unsigned long handledReceivedAt = 0;

void setup() {
  Wire.onReceive([](int v) { wirePeri.onReceive(v); });
  Wire.onRequest([]() { wirePeri.onRequest(); });
  Wire.begin(MOTORS_ADDRESS);
  Serial.begin(9600);
  Serial.println("Start");
}

void handleMotor(int index, I2cMultipleMotors_asukiaaa_motor_info* motorInfo) {
  Serial.println(String(index) + ": " +
                 String(motorInfo->reverse) + " " +
                 String(motorInfo->brake) + " " +
                 String(motorInfo->speed) + " " +
                 String(motorInfo->byteWritable) + " " +
                 String(motorInfo->byteReadOnly));
  // Update motor by motorInfo
}

void loop() {
  if (wirePeri.receivedAt != handledReceivedAt) {
    handledReceivedAt = wirePeri.receivedAt;
    for (int i = 0; i < wirePeri.buffLen; ++i) {
      Serial.print(wirePeri.buffs[i]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println("receivedAt: " + String(wirePeri.receivedAt));

    I2cMultipleMotors_asukiaaa::parseArrToInfo(&motorsInfo, wirePeri.buffs, wirePeri.buffLen);
    for (int i = 0; i < NUMBER_MOTORS; ++i) {
      handleMotor(i, &motorsInfo.motors[i]);
    }
  }
  motorsInfo.motors[0].byteReadOnly = millis() / 1000 % 0xff;
  I2cMultipleMotors_asukiaaa::putReadOnlyInfoToArr(motorsInfo, wirePeri.buffs, wirePeri.buffLen);
  delay(1);
}

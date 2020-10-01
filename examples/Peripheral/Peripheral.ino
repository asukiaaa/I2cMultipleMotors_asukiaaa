#include <I2cMultipleMotors_asukiaaa.h>
#define ADDRESS_MOTORS 0x51
#define NUMBER_MOTORS 3

I2cMultipleMotors_asukiaaa::Info motorsInfo(NUMBER_MOTORS);
I2cMultipleMotors_asukiaaa::PeripheralHandler peri(&Wire, NUMBER_MOTORS);
unsigned long handledReceivedAt = 0;

void setup() {
  Wire.onReceive([](int v) { peri.onReceive(v); });
  Wire.onRequest([]() { peri.onRequest(); });
  Wire.begin(ADDRESS_MOTORS);
  Serial.begin(9600);
  Serial.println("Start");
}

void handleMotor(int index, I2cMultipleMotors_asukiaaa::MotorInfo* motorInfo) {
  Serial.println(String(index) + ": " +
                 String(motorInfo->brake) + " " +
                 String(motorInfo->speed) + " " +
                 String(motorInfo->byteWritable) + " " +
                 String(motorInfo->byteReadOnly));
  // Update motor by motorInfo
}

void loop() {
  if (peri.receivedAt != handledReceivedAt) {
    handledReceivedAt = peri.receivedAt;
    for (int i = 0; i < peri.buffLen; ++i) {
      Serial.print(peri.buffs[i]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.println("receivedAt: " + String(peri.receivedAt));

    peri.parseToInfo(&motorsInfo);
    for (int i = 0; i < NUMBER_MOTORS; ++i) {
      handleMotor(i, &motorsInfo.motors[i]);
    }
  }
  motorsInfo.motors[0].byteReadOnly = millis() / 1000 % 0xff;
  peri.putReadOnlyInfo(motorsInfo);
  delay(1);
}

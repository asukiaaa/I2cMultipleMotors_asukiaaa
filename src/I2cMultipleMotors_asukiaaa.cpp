#include "I2cMultipleMotors_asukiaaa.h"

// #define I2C_MULTIPLE_MOTORS_ASUKIAAA_DEBUG

namespace I2cMultipleMotors_asukiaaa {
  PeripheralHandler::PeripheralHandler(TwoWire *wire, int numberMotors): wire_asukiaaa::PeripheralHandler(
    wire,
    getArrLenFromNumberMotors(numberMotors),
    [](int index) {
      return index % I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR == 2;
    }) {}

  void PeripheralHandler::parseToInfo(Info* info) {
    parseArrToInfo(info, buffs, buffLen);
  }

  MotorInfo::MotorInfo() {
    speed = 0;
    reverse = false;
    brake = false;
    stateRead = -1;
    byteReadOnly = 0;
    byteWritable = 0;
  }

  Info::Info(uint16_t numberMotors): numberMotors(numberMotors) {
    motors = new MotorInfo[numberMotors];
    stateRead = -1;
  }

  Info::~Info() {
    delete[] motors;
  }

  int Info::setMotor(uint8_t index, uint8_t speed, bool reverse, bool brake) {
    if (index >= numberMotors) {
      return 1;
    }
    motors[index].speed = speed;
    motors[index].reverse = reverse;
    motors[index].brake = brake;
    return 0;
  }

  Driver::Driver(uint8_t address, uint16_t numberMotors)
    : address(address),
      numberMotors(numberMotors),
      buffLen(getArrLenFromNumberMotors(numberMotors)) {
    this->wire = NULL;
    this->buffs = new uint8_t[buffLen];
  }

  Driver::~Driver() {
    delete[] buffs;
  }

  void Driver::setWire(TwoWire* wire) {
    this->wire = wire;
  }

  void Driver::begin() {
    if (wire == NULL) {
      Wire.begin();
      this->wire = &Wire;
    }
  }

  int Driver::write(const Info &info) {
    parseInfoToArr(info, buffs, buffLen);
#ifdef I2C_MULTIPLE_MOTORS_ASUKIAAA_DEBUG
    Serial.println("Motors to write:");
    for (int16_t i = 0; i < info.numberMotors; ++i) {
      Serial.println(String(info.motors[i].reverse) + " " + String(info.motors[i].speed));
    }
    Serial.print("Write:");
    for (int16_t i = 0; i < buffLen; ++i) {
      Serial.print(buffs[i]);
      Serial.print(" ");
    }
    Serial.println("");
#endif
    wire->beginTransmission(address);
    wire->write(0);
    wire->write(buffs, buffLen);
    return wire->endTransmission();
  }

  int Driver::read(Info* info) {
    int state = wire_asukiaaa::readBytes(wire, address, 0, buffs, buffLen);
    info->stateRead = state;
    if (state != 0) return state;
    parseArrToInfo(info, buffs, buffLen);
    return state;
  }

  int getArrLenFromNumberMotors(int numberMotors) {
    return numberMotors * I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR;
  }

  void parseInfoToArr(const Info& info, uint8_t* arr, uint16_t arrLen) {
    uint16_t infoNumberMotors = info.numberMotors;
    for (uint16_t i = 0; i < infoNumberMotors; ++i) {
      if (!arrLenAvairableForMotorInfo(i, arrLen)) break;
      parseMotorInfoToArr(info.motors[i],
                          &arr[i * I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR],
                          I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR);
    }
  }

  void parseMotorInfoToArr(const MotorInfo& motorInfo, uint8_t* arr, uint16_t arrLen) {
    if (!arrLenMatchesToMotorInfo(arrLen)) return;
    arr[0] = 0;
    if (motorInfo.reverse) {
      arr[0] |= 0b1;
    }
    if (motorInfo.brake) {
      arr[0] |= 0b10;
    }
    arr[1] = motorInfo.byteWritable;
    arr[2] = motorInfo.byteReadOnly;
    arr[3] = motorInfo.speed;
  }

  void parseArrToInfo(Info* info, uint8_t* arr, uint16_t arrLen) {
    uint16_t num = info->numberMotors;
    for (uint16_t i = 0; i < num; ++i) {
      if (!arrLenAvairableForMotorInfo(i, arrLen)) break;
      parseArrToMotorInfo(&info->motors[i],
                          &arr[i * I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR],
                          I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR);
    }
  }

  void parseArrToMotorInfo(MotorInfo* motorInfo, uint8_t* arr, uint16_t arrLen) {
    if (!arrLenMatchesToMotorInfo(arrLen)) return;
    motorInfo->reverse = ((arr[0] & 0b1) != 0);
    motorInfo->brake = ((arr[0] & 0b10) != 0);
    motorInfo->byteWritable = arr[1];
    motorInfo->byteReadOnly = arr[2];
    motorInfo->speed = arr[3];
  }

  void putReadOnlyInfoToArr(const Info& info, uint8_t* arr, uint16_t arrLen) {
    uint16_t infoNumberMotors = info.numberMotors;
    for (uint16_t i = 0; i < infoNumberMotors; ++i) {
      if (!arrLenAvairableForMotorInfo(i, arrLen)) break;
      putReadOnlyMotorInfoToArr(info.motors[i],
                                &arr[i * I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR],
                                I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR);
    }
  }

  void putReadOnlyMotorInfoToArr(const MotorInfo& motorInfo, uint8_t* arr, uint16_t arrLen) {
    if (!arrLenMatchesToMotorInfo(arrLen)) return;
    arr[2] = motorInfo.byteReadOnly;
  }

  bool arrLenAvairableForMotorInfo(uint16_t index, uint16_t arrLen) {
    return (index + 1) * I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR - 1 < arrLen;
  }

  bool arrLenMatchesToMotorInfo(uint16_t arrLen) {
    return arrLen == I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR;
  }
}

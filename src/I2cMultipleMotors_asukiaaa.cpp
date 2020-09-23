#include "I2cMultipleMotors_asukiaaa.h"
#include <utils_asukiaaa.h>
#include <utils_asukiaaa/wire.h>

I2cMultipleMotors_asukiaaa_motor_info::I2cMultipleMotors_asukiaaa_motor_info() {
  speed = 0;
  reverse = false;
  brake = false;
  stateRead = -1;
}

I2cMultipleMotors_asukiaaa_info::I2cMultipleMotors_asukiaaa_info(uint8_t numberMotors) {
  this->numberMotors = numberMotors;
  motors = new I2cMultipleMotors_asukiaaa_motor_info[numberMotors];
  stateRead = -1;
}

I2cMultipleMotors_asukiaaa_info::~I2cMultipleMotors_asukiaaa_info() {
  delete[] motors;
}

int I2cMultipleMotors_asukiaaa_info::setMotor(uint8_t index, uint8_t speed, bool reverse, bool brake) {
  if (index >= numberMotors) {
    return 1;
  }
  motors[index].speed = speed;
  motors[index].reverse = reverse;
  motors[index].brake = brake;
  return 0;
}

uint8_t I2cMultipleMotors_asukiaaa_info::getNumberMotors() {
  return numberMotors;
}

I2cMultipleMotors_asukiaaa::I2cMultipleMotors_asukiaaa(uint8_t address, uint8_t numberMotors) {
  this->wire = NULL;
  this->address = address;
  this->numberMotors = numberMotors;
  this->buffLen = getArrLenFromNumberMotors(numberMotors);
  this->buffs = new uint8_t[buffLen];
}

I2cMultipleMotors_asukiaaa::~I2cMultipleMotors_asukiaaa() {
  delete[] buffs;
}

void I2cMultipleMotors_asukiaaa::setWire(TwoWire* wire) {
  this->wire = wire;
}

void I2cMultipleMotors_asukiaaa::begin() {
  if (wire == NULL) {
    Wire.begin();
    this->wire = &Wire;
  }
}

int I2cMultipleMotors_asukiaaa::write(I2cMultipleMotors_asukiaaa_info &info) {
  parseInfoToArr(info, buffs, buffLen);
  wire->beginTransmission(address);
  wire->write(0);
  wire->write(buffs, buffLen);
  return wire->endTransmission();
}

int I2cMultipleMotors_asukiaaa::read(I2cMultipleMotors_asukiaaa_info* info) {
  int state = utils_asukiaaa::wire::readBytes(wire, address, 0, buffs, buffLen);
  info->stateRead = state;
  if (state != 0) return state;
  return state;
}

int I2cMultipleMotors_asukiaaa::getArrLenFromNumberMotors(int numberMotors) {
  return numberMotors * 2;
}

void I2cMultipleMotors_asukiaaa::parseInfoToArr(I2cMultipleMotors_asukiaaa_info& info, uint8_t* arr, uint8_t arrLen) {
  int infoNumberMotors = info.getNumberMotors();
  for (int i = 0; i < infoNumberMotors; ++i) {
    if (i * 2 + 2 >= arrLen) break;
    parseMotorInfoToArr(info.motors[i], &arr[i*2], 2);
  }
}

void I2cMultipleMotors_asukiaaa::parseMotorInfoToArr(I2cMultipleMotors_asukiaaa_motor_info& motorInfo, uint8_t* arr, uint8_t arrLen) {
  arr[0] = 0;
  if (motorInfo.reverse) {
    arr[0] |= 0b1;
  }
  if (motorInfo.brake) {
    arr[0] |= 0b10;
  }
  arr[1] = motorInfo.speed;
}

void I2cMultipleMotors_asukiaaa::parseArrToInfo(I2cMultipleMotors_asukiaaa_info* info, uint8_t* arr, uint8_t arrLen) {
  int num = info->getNumberMotors();
  for (int i = 0; i < num; ++i) {
    if (i*2 + 2 >= arrLen) break;
    parseArrToMotorInfo(&info->motors[i], &arr[i*2], 2);
  }
}

void I2cMultipleMotors_asukiaaa::parseArrToMotorInfo(I2cMultipleMotors_asukiaaa_motor_info* motorInfo, uint8_t* arr, uint8_t arrLen) {
  motorInfo->reverse = ((arr[0] & 0b1) != 0);
  motorInfo->brake = ((arr[0] & 0b10) != 0);
  motorInfo->speed = arr[1];
}

#ifndef I2C_MULTIPLE_MOTORS_ASUKIAAA_H
#define I2C_MULTIPLE_MOTORS_ASUKIAAA_H

#include <Wire.h>

// #define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <Arduino.h>
#endif

class I2cMultipleMotors_asukiaaa_motor_info {
 public:
  I2cMultipleMotors_asukiaaa_motor_info();
  uint8_t speed;
  bool reverse;
  bool brake;
  int stateRead;
};

class I2cMultipleMotors_asukiaaa_info {
 public:
  I2cMultipleMotors_asukiaaa_info(uint8_t numberMotors);
  ~I2cMultipleMotors_asukiaaa_info();
  int setMotor(uint8_t index, uint8_t speed, bool reverse=false, bool brake=false);
  uint8_t getNumberMotors();

  int stateRead;
  I2cMultipleMotors_asukiaaa_motor_info* motors;

 private:
  uint8_t numberMotors;
};

class I2cMultipleMotors_asukiaaa {
 public:
  I2cMultipleMotors_asukiaaa(uint8_t address, uint8_t numberMotors);
  ~I2cMultipleMotors_asukiaaa();
  void setWire(TwoWire* wire);
  void begin();
  int read(I2cMultipleMotors_asukiaaa_info* info);
  int write(I2cMultipleMotors_asukiaaa_info& info);

  static int getArrLenFromNumberMotors(int numberMotors);
  static void parseInfoToArr(I2cMultipleMotors_asukiaaa_info& info, uint8_t* arr, uint8_t arrLen);
  static void parseMotorInfoToArr(I2cMultipleMotors_asukiaaa_motor_info& motorInfo, uint8_t* arr, uint8_t arrLen);
  static void parseArrToInfo(I2cMultipleMotors_asukiaaa_info* info, uint8_t* arr, uint8_t arrLen);
  static void parseArrToMotorInfo(I2cMultipleMotors_asukiaaa_motor_info* motorInfo, uint8_t* arr, uint8_t arrLen);

 private:
  TwoWire* wire;
  uint8_t address;
  uint8_t numberMotors;
  int buffLen;
  uint8_t* buffs;
};

#endif
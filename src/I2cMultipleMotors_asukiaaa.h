#ifndef I2C_MULTIPLE_MOTORS_ASUKIAAA_H
#define I2C_MULTIPLE_MOTORS_ASUKIAAA_H

#include <Wire.h>
#include <wire_asukiaaa.h>

// #define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <Arduino.h>
#endif

#define I2C_MULTIPLE_MOTORS_ARR_LEN_INFO_MOTOR 4

namespace I2cMultipleMotors_asukiaaa {
  class MotorInfo {
   public:
    MotorInfo();
    uint8_t speed;
    bool reverse;
    bool brake;
    int stateRead;
    uint8_t byteReadOnly;
    uint8_t byteWritable;
  };

  class Info {
   public:
    Info(uint16_t numberMotors);
    ~Info();
    int setMotor(uint8_t index, uint8_t speed, bool reverse=false, bool brake=false);
    int stateRead;
    MotorInfo* motors;
    const uint16_t numberMotors;
  };

  class PeripheralHandler: public wire_asukiaaa::PeripheralHandler {
   public:
    PeripheralHandler(TwoWire* wire, int numberMotors);
    void parseToInfo(Info* info);
  };

  class Driver {
   public:
    Driver(uint8_t address, uint16_t numberMotors);
    ~Driver();
    void setWire(TwoWire* wire);
    void begin();
    int read(Info* info);
    int readMotor(uint16_t index, MotorInfo* motorInfo);
    int readMotor(uint16_t index, int* speed, bool* brake = NULL);
    int write(const Info& info);
    int writeMotor(uint16_t index, const MotorInfo& motorInfo);
    int writeMotor(uint16_t index, int speed, bool brake = false);
    const uint8_t address;
    const uint16_t numberMotors;

   private:
    TwoWire* wire;
    const int buffLen;
    uint8_t* buffs;
  };

  int getArrLenFromNumberMotors(int numberMotors);
  void parseInfoToArr(const Info& info, uint8_t* arr, uint16_t arrLen);
  void parseMotorInfoToArr(const MotorInfo& motorInfo, uint8_t* arr, uint16_t arrLen);
  void parseArrToInfo(Info* info, uint8_t* arr, uint16_t arrLen);
  void parseArrToMotorInfo(MotorInfo* motorInfo, uint8_t* arr, uint16_t arrLen);
  void putReadOnlyInfoToArr(const Info& info, uint8_t* arr, uint16_t arrLen);
  void putReadOnlyMotorInfoToArr(const MotorInfo& motorInfo, uint8_t* arr, uint16_t arrLen);
  bool arrLenAvairableForMotorInfo(uint16_t index, uint16_t arrLen);
  bool arrLenMatchesToMotorInfo(uint16_t arrLen);
}

#endif

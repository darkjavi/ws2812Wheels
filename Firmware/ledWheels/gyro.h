#ifndef GYRO
#define GYRO

#include "gyro_accel.h"

#define dt 20                       // time difference in milli seconds
#define rad2degree 57.3              // Radian to degree conversion
#define Filter_gain 0.95             // e.g.  angle = angle_gyro*Filter_gain + angle_accel*(1-Filter_gain)
// *********************************************************************
//    Global Variables
// *********************************************************************


class MPU6050 {
public:
  MPU6050() {}

  void init()
  {
    on();
  }

  void on()
  {

    Wire.begin(D6,D5);
//    Wire.beginTransmission(MPU);
//    Wire.write(0x6B);  // PWR_MGMT_1 register
//    Wire.write(0);     // set to zero (wakes up the MPU-6050)
//    Wire.endTransmission(true);
    MPU6050_ResetWake();
//    MPU6050_SetGains(0,1);// Setting the lows scale
//    MPU6050_SetDLPF(0); // Setting the DLPF to inf Bandwidth for calibration
    yield();
    //MPU6050_OffsetCal();
    MPU6050_SetDLPF(6); // Setting the DLPF to lowest Bandwidth
    m_activated = true;
  }
  void off()
  {
    Wire.begin(D6,D5);
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(1);     // apagar?
    Wire.endTransmission(true);
    m_activated = false;
  }

  void firstRead()
  {
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Temp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      Temp = Temp/340.00+36.53;
      lastAcX  = AcX;
      lastAcY  = AcY;
      lastAcZ  = AcZ;
      lastGyX  = GyX;
      lastGyX  = GyY;
      lastGyX  = GyZ;
      lastTemp = Temp;
  }

  void fullRead()
  {
      if(!m_activated)
      {
              Serial.println("Request for read the gyro, but is not activated...");
              return;
      }

/*
      MPU6050_ReadData();

      angle_x_gyro = (gyro_x_scalled*((float)dt/1000)+angle_x);
      angle_y_gyro = (gyro_y_scalled*((float)dt/1000)+angle_y);
      angle_z_gyro = (gyro_z_scalled*((float)dt/1000)+angle_z);

      angle_z_accel = atan(accel_z_scalled/(sqrt(accel_y_scalled*accel_y_scalled+accel_x_scalled*accel_x_scalled)))*(float)rad2degree;
      angle_y_accel = -atan(accel_y_scalled/(sqrt(accel_y_scalled*accel_y_scalled+accel_z_scalled*accel_z_scalled)))*(float)rad2degree;
      angle_x_accel = atan(accel_x_scalled/(sqrt(accel_x_scalled*accel_x_scalled+accel_z_scalled*accel_z_scalled)))*(float)rad2degree;

      angle_x = Filter_gain*angle_x_gyro+(1-Filter_gain)*angle_x_accel;
      angle_y = Filter_gain*angle_y_gyro+(1-Filter_gain)*angle_y_accel;
      angle_z = Filter_gain*angle_z_gyro+(1-Filter_gain)*angle_z_accel;

      Temp = temp_scalled;


      Serial.print(gyro_x_scalled);
      Serial.print("\t");
      Serial.print(gyro_y_scalled);
      Serial.print("\t");
      Serial.print(gyro_z_scalled);
      Serial.print("\t");


      Serial.print(accel_x_scalled);
      Serial.print("\t");
      Serial.print(accel_y_scalled);
      Serial.print("\t");
      Serial.print(accel_z_scalled);
      Serial.print("\t");

      Serial.print(angle_x_gyro);
      Serial.print("\t");
      Serial.print(angle_y_gyro);
      Serial.print("\t");
      Serial.print(angle_z_gyro);
      Serial.print("\t");

      Serial.print(angle_x_accel);
      Serial.print("\t");
      Serial.print(angle_y_accel);
      Serial.print("\t");
      Serial.print(angle_z_accel);
      Serial.print("\t");


      Serial.print(angle_x);
      Serial.print("\t");
      Serial.print(angle_y);
      Serial.print("\t");
      Serial.println(angle_z);
      yield();
      return;
*/
    lastAcX  = AcX;
    lastAcY  = AcY;
    lastAcZ  = AcZ;
    lastGyX  = GyX;
    lastGyX  = GyY;
    lastGyX  = GyZ;
    lastTemp = Temp;
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Temp = Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);  // request a total of 14 registers

    int16_t secondRead =  Wire.read()<<8|Wire.read();
    AcX  += (secondRead -  AcX) /2;
    secondRead =  Wire.read()<<8|Wire.read();
    AcY  += (secondRead -  AcY) /2;
    secondRead =  Wire.read()<<8|Wire.read();
    AcZ  += (secondRead -  AcZ) /2;
    secondRead =  Wire.read()<<8|Wire.read();
    Temp += (secondRead -  Temp) /2;
    secondRead =  Wire.read()<<8|Wire.read();
    GyX  += (secondRead -  GyX) /2;
    secondRead =  Wire.read()<<8|Wire.read();
    GyY  += (secondRead -  GyY) /2;
    secondRead =  Wire.read()<<8|Wire.read();
    GyZ  += (secondRead -  GyZ) /2;


    acxRising = AcX > lastAcX;
    acyRising = AcY > lastAcY;
    aczRising = AcZ > lastAcZ;

    Temp = Temp/340.00+36.53;
  }

  //float angle_x_gyro=0,angle_y_gyro=0,angle_z_gyro=0,angle_x_accel=0,angle_y_accel=0,angle_z_accel=0,angle_x=0,angle_y=0,angle_z=0;
  int16_t   AcX,AcY,AcZ,GyX,GyY,GyZ,lastAcX,lastAcY,lastAcZ,lastGyX,lastGyY,lastGyZ;
  bool      acxRising,acyRising,aczRising;
  float     Temp,lastTemp;

protected:
    bool	  m_activated = false;
    const int     MPU=0x68;  // I2C address of the MPU-6050

};

#endif // GYRO

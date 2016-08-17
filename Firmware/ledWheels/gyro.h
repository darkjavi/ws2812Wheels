#ifndef GYRO
#define GYRO

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
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
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
  void fullRead()
  {
        if(!m_activated)
        {
                Serial.println("Request for read the gyro, but is not activated...");
                return;
        }

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

    //Serial.println(Temp/340.00+36.53);// equation for temperature in degrees C from datasheet
    Temp = Temp/340.00+36.53;
  }


  int16_t   AcX,AcY,AcZ,GyX,GyY,GyZ;
  float     Temp;
  protected:
        bool	  m_activated = false;
    const int     MPU=0x68;  // I2C address of the MPU-6050

};

#endif // GYRO

#include <Adafruit_NeoPixel.h>
//#include <avr/power.h>
#include <Wire.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            2
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     83

#ifdef ESP8266
	#define A1 17
#endif

class MPU6050 {
public:
  MPU6050() {}

  void init()
  {
    on();
  }
  void on()
  {
#ifdef ESP8266
    Wire.begin(4,5);
#else
    Wire.begin();
#endif

    Wire.beginTransmission(MPU);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
  }
  void off()
  {
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(1);     // apagar?
    Wire.endTransmission(true);    
  }
  void fullRead()
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    Serial.print("ax:"); Serial.println(AcX);
    Serial.print("ay:"); Serial.println(AcY);
    Serial.print("az:"); Serial.println(AcZ);
//    Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
    Serial.print("gx:"); Serial.println(GyX);
    Serial.print("gy:"); Serial.println(GyY);
    Serial.print("gz:"); Serial.println(GyZ);  
  }
  protected:
    const int MPU=0x68;  // I2C address of the MPU-6050
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
};

class vuMeter{
  public:
    vuMeter(const int pin): m_pin(pin) {}
    void analize()
    {
      int value = 0;
      //for (int i = 0 ; i < 10 ; i++)
      //{
      //  value = ((value*i)+analogRead(m_pin)) / i+1 ;
      //}
      value = analogRead(m_pin);
      Serial.print("v:"); Serial.println(value);
    }
  private:
  int m_pin;

};

class led{
  public:
  led(uint8_t red =0,uint8_t green=0,uint8_t blue=0):
    r(red), g(green), b(blue) {};
  void setColor(uint8_t red =0,uint8_t green=0,uint8_t blue=0){r=red,g=green,b=blue;}
  uint8_t r;
  uint8_t g;
  uint8_t b;
  void off()   {setColor(0,0,0);}
  void white() {setColor(200,200,200);}
};

class ws2812Strip {
  public:
  ws2812Strip() : m_pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800) {m_pixels.begin();}  
  led leds[NUMPIXELS];
  void update(){
    for(int i=0;i<NUMPIXELS;i++){
      m_pixels.setPixelColor(i, leds[i].r,leds[i].g,leds[i].b);
    }
    m_pixels.show(); 
  }
  void off()
  {
    for( uint8_t i = 0 ; i < NUMPIXELS ; i++) 
    {leds[i].off();
    }
    update();
  }
  void test()
  {
    for( uint8_t i = 0 ; i < NUMPIXELS ; i++) 
    {
      leds[i].setColor(10,10,10);
      update();
      delay(50);
    }
  }
  
  private:
    Adafruit_NeoPixel m_pixels;
}; 
 

class ledGadget {
  public:
    ledGadget(int lednr)
    {
      m_leds = new led*[lednr];
      m_ledCount = lednr;
    }
    
    void addLed(led* l)
    {
      if(m_ledsPopulated >= m_ledCount) return;
      m_leds[m_ledsPopulated]= l;
      m_ledsPopulated++;
    }
    void setColor(uint8_t red =0,uint8_t green=0,uint8_t blue=0)
    {
      for( uint8_t i = 0 ; i < m_ledsPopulated ; i++) 
      {m_leds[i]->setColor(red,green,blue);}
    }
    void off()
    {
      for( uint8_t i = 0 ; i < m_ledsPopulated ; i++) 
      {m_leds[i]->off();}
    }
    virtual void animate() {}
    void set_vuMeter(vuMeter* vuMeter)
    {
      m_vuMeter = vuMeter;
    }
    
  protected:
    uint8_t   m_ledCount = 0;
    uint8_t   m_ledsPopulated = 0;
    led**     m_leds;
    vuMeter*  m_vuMeter = 0;
    MPU6050*  m_gyro = 0;
};

class ledBar : public ledGadget {
  public:
  ledBar(uint8_t lednr):ledGadget(lednr) {}
  void animate()
  {
    
  }
  protected:
  uint8_t m_level;
 };

class ledCircle : public ledGadget {
  public:
  ledCircle(uint8_t lednr):ledGadget(lednr)  {}
  void circleAnimationcc()
  {
    animationType = 1;
    animationIndex=0;
    off();
  }
  void circleAnimationccw()
  {
    animationType = 2;
    animationIndex=0;
    off();
  }
  void glowAnimation()
  {
    animationType = 3;
    animationIndex=0;
    off();    
  }

    void doubleCircleAnimation()
  {
    animationType = 4;
    animationIndex=0;
    animationIndex2=0;
    off();
  }
  
  void animate()
  {
    if(animationType == 1)
    {
      if(animationIndex == m_ledCount-1) animationIndex = 0;
      off();
      m_leds[animationIndex]->setColor((animationIndex*3),50,(animationIndex*3));
      m_leds[animationIndex+1]->setColor((animationIndex*3),50,(animationIndex*3));
      animationIndex++;
    } else if(animationType == 2) {
      if(animationIndex == 0) animationIndex = m_ledCount-1;
      off();
      m_leds[animationIndex]->setColor((animationIndex*3),50,(animationIndex*3));
      m_leds[animationIndex-1]->setColor((animationIndex*3),50,(animationIndex*3));
      animationIndex--;
    } else if(animationType == 3) {
      if(rising)
      {
        if(animationIndex == m_ledCount-1) rising = 0;
        setColor(animationIndex*1.5,animationIndex*1.5,animationIndex*1.5);
        animationIndex++;      
      
      }else {
        if(animationIndex == 1) rising = 1;
        setColor(animationIndex*1.5,animationIndex*1.5,animationIndex*1.5);
        animationIndex--;
      
      }
    } else if(animationType == 4) {
        if(animationIndex == m_ledCount-1) animationIndex = 0;
        if(animationIndex2 == 0) animationIndex2 = m_ledCount-1;
        off();

        m_leds[animationIndex]->setColor((animationIndex*3),50,(animationIndex*3));
        m_leds[animationIndex-1]->setColor((animationIndex*3),50,(animationIndex*3));
        animationIndex++;

        m_leds[animationIndex2]->setColor(50, (animationIndex2*3) ,(animationIndex2*3));
        m_leds[animationIndex2-1]->setColor(50, (animationIndex2*3),(animationIndex2*3));
        animationIndex2--;    
    }
  }
private:
  int animationIndex=0;
  int animationIndex2=0;
  uint8_t animationType=0;
  bool rising = 1;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// Empieza el programa////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t delay_time = 30;
uint16_t cicle = 0;
vuMeter myVuMeter(A1);
ws2812Strip myStrip;
ledCircle c1(41);
MPU6050 myGyro;




void setup() {
  Serial.begin(115200);
  
  Serial.println("Init...");
  yield();
  
  myStrip.off();
  myStrip.test();
  myStrip.off();
  c1.addLed(&myStrip.leds[0]);
  c1.addLed(&myStrip.leds[1]);
  c1.addLed(&myStrip.leds[2]);
  c1.addLed(&myStrip.leds[3]);
  c1.addLed(&myStrip.leds[4]);
  c1.addLed(&myStrip.leds[5]);
  c1.addLed(&myStrip.leds[6]);
  c1.addLed(&myStrip.leds[7]);
  c1.addLed(&myStrip.leds[8]);
  c1.addLed(&myStrip.leds[9]);
  c1.addLed(&myStrip.leds[10]);
  c1.addLed(&myStrip.leds[11]);
  c1.addLed(&myStrip.leds[12]);
  c1.addLed(&myStrip.leds[13]);
  c1.addLed(&myStrip.leds[14]);
  c1.addLed(&myStrip.leds[15]);
  c1.addLed(&myStrip.leds[16]);
  c1.addLed(&myStrip.leds[17]);
  c1.addLed(&myStrip.leds[18]);
  c1.addLed(&myStrip.leds[19]);
  c1.addLed(&myStrip.leds[20]);
  c1.addLed(&myStrip.leds[21]);
  c1.addLed(&myStrip.leds[22]);
  c1.addLed(&myStrip.leds[23]);
  c1.addLed(&myStrip.leds[24]);
  c1.addLed(&myStrip.leds[25]);
  c1.addLed(&myStrip.leds[26]);
  c1.addLed(&myStrip.leds[27]);
  c1.addLed(&myStrip.leds[28]);
  c1.addLed(&myStrip.leds[29]);
  c1.addLed(&myStrip.leds[30]);
  c1.addLed(&myStrip.leds[31]);
  c1.addLed(&myStrip.leds[32]);
  c1.addLed(&myStrip.leds[33]);
  c1.addLed(&myStrip.leds[34]);
  c1.addLed(&myStrip.leds[35]);
  c1.addLed(&myStrip.leds[36]);
  c1.addLed(&myStrip.leds[37]);
  c1.addLed(&myStrip.leds[38]);
  c1.addLed(&myStrip.leds[39]);
  c1.addLed(&myStrip.leds[40]);
  c1.doubleCircleAnimation();
//  myStrip.update();
//  myGyro.init();
  Serial.println("Ok");
}




void loop() 
{
//    if(cicle == 1320)
//    {
//      cicle = 0;
//      c1.doubleCircleAnimation();
//    }else if(cicle == 1000)
//    {
//      c1.glowAnimation();
//    }
//  myVuMeter.analize();
  myGyro.fullRead();
  c1.animate();
  myStrip.update();
  delay(delay_time);
  cicle++;
}

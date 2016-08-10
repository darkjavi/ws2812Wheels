#ifndef WS2812B
#define WS2812B

#include "settings.h"

class ws2812Strip {
public:
    class led{ // clase led dentro de la clase Strip
    public:
      led(int r = 0, int g = 0, int b = 0, float bright = 1.0): m_r(r), m_g(r), m_b(b), m_bright(bright) {;}

      void setColor(uint8_t red =0,uint8_t green=0,uint8_t blue=0)
      {
          m_r = red;
          m_g = green;
          m_b = blue;
      }

      void dimm(int power)
      {
          float p = power /100;
          dimm(p);
      }

      void dimm(float factor = 0.75)
      {
          m_r *= factor;
          m_g *= factor;
          m_b *= factor;
      }

      void setBrightness(float b = 1.0)
      {
          m_bright = b;
      }

      void off()   {setColor(0,0,0);}
      void white() {setColor(200,200,200);}
      void red()   {setColor(230,0,0);}
      void green() {setColor(0,230,0);}
      void blue()  {setColor(0,0,230);}

      void setR(uint8_t val) {m_r = val;}
      void setG(uint8_t val) {m_g = val;}
      void setB(uint8_t val) {m_b = val;}

      int r()   {return m_r * m_bright;}
      int g()   {return m_g * m_bright;}
      int b()   {return m_b * m_bright;}

    private:
      uint8_t m_r;
      uint8_t m_g;
      uint8_t m_b;
      float   m_bright;
    };

  ws2812Strip(settingList* settings) : m_pixels(settings->lednr, settings->ledStripPin, NEO_GRB + NEO_KHZ800), m_settings(settings)
  {
    if((isValid()))
    {
      for(int i = 0 ; i < settings->lednr ; i++) m_leds.push_back(led());
    }
  }

  void setBrightness(int b)
  {
      m_settings->ledMaxBright = b/100.0f;
      update();
  }

  void setBrightness(float b = 1.0)
  {
      m_settings->ledMaxBright = b;
      update();
  }

  float brightness()
  {
      return  m_settings->ledMaxBright;
  }

  bool isValid() {return  (m_settings->ledStripPin != -1);}

  std::vector<led> leds()&   {return m_leds;}

  led* getLed(int pos)
  {
    return &m_leds[pos];
  }

  std::vector<led>* getLeds()
  {
          return &m_leds;
  }

  void setup()
  {
    if(!isValid()) return;
    Serial.print("Setting up ws2812 strip on pin: "); Serial.print(m_settings->ledStripPin);
    Serial.print(" Total leds: "); Serial.print(m_settings->lednr);
    Serial.print(" max_bright: "); Serial.println(m_settings->ledMaxBright);
    m_pixels.begin();
    yield();
    test();
    off();
  }

  void update(){
    if(!isValid()) return;
    for(int i=0;i<m_leds.size();i++)
    {
        m_pixels.setPixelColor(i, (m_leds[i].r() * m_settings->ledMaxBright) ,
                                  (m_leds[i].g() * m_settings->ledMaxBright) ,
                                  (m_leds[i].b() * m_settings->ledMaxBright) );
    }

    yield();
    m_pixels.show();
  }

  void off()
  {
    if(!isValid()) return;
    for( uint16_t i = 0 ; i < m_leds.size() ; i++)
    {
        m_leds[i].off();
    }
    update();
  }

  void setColor(uint8_t r,uint8_t g, uint8_t b){
    if(!isValid()) return;
    for(int i=0;i<m_leds.size();i++)
    {
      m_leds[i].setColor(r,g,b);
    }
    update();
  }

  void test()
  {
    if(!isValid()) return;
    for( uint16_t i = 0 ; i < m_leds.size() ; i++)
    {
      m_leds[i].setColor(20,20,20);
      if(i%2 == 0)update();
      delay(10);
    }
  }

  void animate()
  {
     update();
  }

private:
    std::vector<led>    m_leds;
    Adafruit_NeoPixel   m_pixels;
    settingList*        m_settings;
};

#endif // WS2812B


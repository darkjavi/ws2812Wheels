#ifndef LEDWHEEL
#define LEDWHEEL

#include "ws2812b.h"
#include "gyro.h"

class ledWheel{
public:
    ledWheel(std::vector<ws2812Strip::led>* leds, MPU6050* gyro)
    {
        m_rawLeds = leds;
        m_gyro = gyro;
        for(int i = 0 ; i < m_rawLeds->size() ; i++)
        {
                ws2812Strip::led* l = &m_rawLeds->at(i);
                m_correctedLedArray.push_back(l);
        }
    }

    void off()
    {
        for(int i = 0 ; i < m_correctedLedArray.size() ; i++)
            m_correctedLedArray[i]->off();
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        resetEffect();
        for(int i = 0 ; i < m_correctedLedArray.size() ; i++)
            m_correctedLedArray[i]->setColor(r,g,b);
    }

    void setBrightness(float b)
    {
        //Esto esta en la tira!
    }

    void resetEffect()
    {
        off();
        m_currentEffect = effectNone;
        m_counter1 = 0;
        m_counter2 = 0;
        m_counter3 = 0;
    }

    void circleEffect()
    {
        resetEffect();
        m_currentEffect = effectCircle;
    }

    void doubleCircleEffect()
    {
        resetEffect();
        m_currentEffect = effectDoubleCircle;
    }

    void flashEffect()
    {
        resetEffect();
        m_currentEffect = effectFlash;
    }

    void rainbowEffect()
    {
        resetEffect();
        m_currentEffect = effectRainbow;
    }

    void animate()
    {
        if      (m_currentEffect == effectCircle)
        {
            //recalculateArray();
            animateCircle();
        }
        else if (m_currentEffect == effectDoubleCircle)
        {
            animateDoubleCircle();
        }
        else if (m_currentEffect == effectFlash)
        {
            animateFlash();
        }
        else if (m_currentEffect == effectRainbow)
        {
            //recalculateArray();
            animateRainbow();
        }
    }

    void parseClientData()
    {
        //check clients for data
        for(uint8_t i = 0; i < MAX_SRV_CLIENTS; i++)
        {
          if (serverClients[i] && serverClients[i].connected())
          {
            if(serverClients[i].available()){
              String data;
              while(serverClients[i].available())
              {
                  data += (char)serverClients[i].read();
              }
              parseCommand(data);
            }
          }
        }
    }

private:
    enum effectType{
        effectNone,
        effectCircle,
        effectDoubleCircle,
        effectFlash,
        effectRainbow
    };

    MPU6050*                            m_gyro;
    std::vector<ws2812Strip::led>*	m_rawLeds;
    std::vector<ws2812Strip::led*>	m_correctedLedArray;

    effectType  m_currentEffect;
    effectType  m_nextEffect;
    uint8_t     m_counter1;
    uint8_t     m_counter2;
    uint8_t     m_counter3;


    void parseCommand(String& data)
    {
        Serial.println(data);
        std::vector<String> args = splitStr(data,"|");
        if(args.size() == 0)
        {
            Serial.print("Failed to parse msg: ");
            Serial.println(data);
            return;
        }

        if      (args[0] == "off")
        {
            resetEffect();
        }
        else if (args[0] == "setColor")
        {
            if(args.size() == 4)
            setColor(args[1].toInt(),args[2].toInt(),args[3].toInt());
        }
        else if (args[0] == "setBrightness")
        {
            //if(args.size() == 2)
            //set (args[1].toFloat());
        }
        else if (args[0] == "animationCircle")
        {
            circleEffect();
        }
        else if (args[0] == "animationDoubleCircle")
        {
            doubleCircleEffect();
        }
        else if (args[0] == "animationRainbow")
        {
            rainbowEffect();
        }
        else
        {
            Serial.print("Unknown command:");
            Serial.println(args[0]);
        }
    }

    void animateCircle()
    {
        if(m_counter1 == m_correctedLedArray.size()-1) m_counter1 = 0;
        off();
        m_correctedLedArray[m_counter1]->setColor((m_counter1*3),50,(m_counter1*3));
        m_correctedLedArray[m_counter1+1]->setColor((m_counter1*3),50,(m_counter1*3));
        m_counter1++;
    }

    void animateDoubleCircle()
    {
        if(m_counter1 == m_correctedLedArray.size()-1) m_counter1 = 0;
        if(m_counter2 == 0) m_counter2 = m_correctedLedArray.size()-1;
        off();
        m_correctedLedArray[m_counter1]->setColor((m_counter1*3),50,(m_counter1*3));
        if(m_counter1 > 0)
            m_correctedLedArray[m_counter1-1]->setColor((m_counter1*3),50,(m_counter1*3));
        m_counter1++;

        m_correctedLedArray[m_counter2]->setColor(50, (m_counter2*3) ,(m_counter2*3));
        if(m_counter2 > 0)
            m_correctedLedArray[m_counter2-1]->setColor(50, (m_counter2*3),(m_counter2*3));
        m_counter2--;
    }

    void animateFlash()
    {

    }

    void animateRainbow()
    {
        if(m_counter1 > 256*5) m_counter1 = 0;

        for(uint16_t i = 0;  i < m_correctedLedArray.size() ; i++)
        {
            uint16_t WheelPos = 255 - (((i * 256 / m_correctedLedArray.size()) + m_counter1) & 255);
            if(WheelPos < 85)
            {
                m_correctedLedArray[i]->setColor(255 - WheelPos * 3, 0, WheelPos * 3);
            }
            else if(WheelPos < 170)
            {
                WheelPos -= 85;
                m_correctedLedArray[i]->setColor(0, WheelPos * 3, 255 - WheelPos * 3);
            }
            else
            {
                WheelPos -= 170;
                m_correctedLedArray[i]->setColor(WheelPos * 3, 255 - WheelPos * 3, 0);
            }
            yield();
        }
        m_counter1 += 10;
    }

    void recalculateArray()
    {
            m_correctedLedArray.clear();
            for(int i = 0 ; i < m_rawLeds->size() ; i++)
            {
                    ws2812Strip::led* l = &m_rawLeds->at(i);
                    m_correctedLedArray.push_back(l);
            }
    }

};
#endif // LEDWHEEL

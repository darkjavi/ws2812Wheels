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
        }

    virtual void animateRainbow()
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

        void animate()
        {
                recalculateArray();
                animateRainbow();

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

private:
        MPU6050*						m_gyro;
        std::vector<ws2812Strip::led>*	m_rawLeds;
        std::vector<ws2812Strip::led*>	m_correctedLedArray;

        uint8_t m_counter1;
    uint8_t m_counter2;
    uint8_t m_counter3;
};
#endif // LEDWHEEL


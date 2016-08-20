#ifndef LEDWHEEL
#define LEDWHEEL

#include "ws2812b.h"
#include "gyro.h"
#include "vector"
#include "wifi.h"
#include "sound.h"

class ledWheel{
public:
    ledWheel(EEPROMStorage* settings) : m_strip(&settings->settings()) , m_vuMeter(A0)
    {
        m_gyro = new MPU6050();
        m_arrayOffset = 0;
        m_sendingSensorData = false;
    }

    void setup()
    {
        m_strip.setup();
        m_gyro->init();
        recalculateArray();
        yield();
    }

    void update()
    {
        m_gyro->fullRead();
        m_vuMeter.update();
        animate();

        if(m_sendingSensorData)
            sendSensorRead();
        yield();
        parseClientData();
    }

    void off()
    {
        for(int i = 0 ; i < m_correctedLedArray.size() ; i++)
            m_correctedLedArray[i]->off();
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        for(int i = 0 ; i < m_correctedLedArray.size() ; i++)
            m_correctedLedArray[i]->setColor(r,g,b);
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
        if(m_currentEffect == effectFlash)
        {
            m_counter1 = 250;
            return;
        }

        m_nextEffect = m_currentEffect; //salvamos los parametros para retomar la animacion despues del flash
        m_counter1save = m_counter1;
        m_counter2save = m_counter2;
        m_counter3save = m_counter3;

        resetEffect();
        m_currentEffect = effectFlash;
        m_counter1 = 200;
    }

    void nextEffect()
    {
        m_currentEffect = (effectType)((int)m_currentEffect+1);
    }

    void prevEffect()
    {
        m_currentEffect = (effectType)((int)m_currentEffect-1);
    }


    void rainbowEffect()
    {
        resetEffect();
        m_currentEffect = effectRainbow;
    }

    void setPositionLights()
    {
        m_currentEffect = effectPositionLigths;
        std::vector<ws2812Strip::led*> rside = getRightSide();
        std::vector<ws2812Strip::led*> lside = getLeftSide();
        for(int i = 0 ; i < rside.size() ; i++)
        {
            rside.at(i)->setColor(150,150,150);
        }
        for(int i = 0 ; i < lside.size() ; i++)
        {
            lside.at(i)->setColor(200,0,0);
        }
    }

    void setPercentage(int percentage = 50)
    {
        off();
        m_currentEffect = effectPercentage;
        if(percentage > 100) percentage = 100;
        std::vector<ws2812Strip::led*> rside = getRightSide();
        std::vector<ws2812Strip::led*> lside = getLeftSide();
        int i;
        for(i = 0 ; i < (percentage /100.0) * rside.size() ; i++)
        {
            rside.at(i)->setColor(00,100,80);
        }

        for(i = 0 ; i < (percentage /100.0) * lside.size() ; i++)
        {
            lside.at(i)->setColor(00,100,80);
        }
    }

    void setVUMeter(int percentage = 50, bool decayMode = true)
    {
        if(!decayMode)
            resetEffect();
        m_currentEffect = effectVUMeter;

        std::vector<ws2812Strip::led*> rside = invertLedOrder(getRightSide());
        std::vector<ws2812Strip::led*> lside = getLeftSide();
        paintVuMeterBar(rside,percentage,0.75);
        paintVuMeterBar(lside,percentage,0.75);
    }

    void setLigthOfSpeed(int value = 0)
    {
        m_currentEffect = effectSpeedLight;
        uint8_t r,g,b;
        r=0,g=0,b=0;
        if(value > -200)//compensar que el valor estatico del gx es ~ -250
        {
            b = value*255/32768.0f;
            if(value >50)
            {
                g = b * 0.6;
            }
        }
        if(value < -300)
        {
            r = -value*255/32768.0f;
            if(value < -50)
            {
                g = r * 0.3;
            }
        }
        setColor(r,g,b);
    }

    void animate()
    {
        if      (m_currentEffect == effectCircle)
        {
            recalculateArray();
            animateCircle();
        }
        else if (m_currentEffect == effectDoubleCircle)
        {
            recalculateArray();
            animateDoubleCircle();
        }
        else if (m_currentEffect == effectFlash)
        {
            animateFlash();
        }
        else if (m_currentEffect == effectCross)
        {
            recalculateArray();
            animateCross();
        }
        else if (m_currentEffect == effectRainbow)
        {
            recalculateArray();
            animateRainbow();
        }
        else if (m_currentEffect == effectStationaryRainbow)
        {
            recalculateArray();
            animateStationaryRainbow();
        }
        else if (m_currentEffect == effectPositionLigths)
        {
            recalculateArray();
            setPositionLights();
        }
        else if (m_currentEffect == effectPercentage)
        {
            recalculateArray();
            setPercentage(m_counter1++);
        }
        else if (m_currentEffect == effectVUMeter)
        {
            recalculateArray();
            setVUMeter(m_vuMeter.value()*100/700);
        }
        else if (m_currentEffect == effectSpeedLight)
        {
            setLigthOfSpeed(m_gyro->GyX);
        }

        m_strip.update();
    }

    void parseClientData()
    {
        //check clients for data
        for(uint8_t i = 0; i < MAX_SRV_CLIENTS; i++)
        {
          if (serverClients[i] && serverClients[i].connected())
          {
            while(serverClients[i].available()){
              String data;
              data = serverClients[i].readStringUntil('\n');
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
        effectCross,
        effectRainbow,
        effectStationaryRainbow,
        effectPositionLigths,
        effectPercentage,
        effectVUMeter,
        effectSpeedLight,
        effectFlash
    };

    ws2812Strip                         m_strip;
    EEPROMStorage*                      m_settingsStorage;
    MPU6050*                            m_gyro;
    vuMeter                             m_vuMeter;
    std::vector<ws2812Strip::led>*	m_rawLeds;
    std::vector<ws2812Strip::led*>	m_correctedLedArray;
    uint16_t                            m_arrayOffset;
    uint16_t                            m_lastArrayOffset;
    uint8_t                             m_dropCount;
    bool                                m_sendingSensorData;

    effectType  m_currentEffect;
    effectType  m_nextEffect;
    uint16_t     m_counter1;
    uint16_t     m_counter2;
    uint16_t     m_counter3;
    uint16_t     m_counter1save;
    uint16_t     m_counter2save;
    uint16_t     m_counter3save;


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
            {
              resetEffect();
              setColor(args[1].toInt(),args[2].toInt(),args[3].toInt());
            }
        }
        else if (args[0] == "setBrightness")
        {
            if(args.size() == 2)
                m_strip.setBrightness(args[1].toFloat());
        }
        else if (args[0] == "nextEffect")
        {
            nextEffect();
        }
        else if (args[0] == "prevEffect")
        {
            prevEffect();
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
        else if (args[0] == "animationFlash")
        {
            flashEffect();
        }
        else if (args[0] == "setOffset")
        {
            if(args.size() == 2)
            {
                m_arrayOffset = args[1].toInt();
            }
        }
        else if (args[0] == "sendSensorsData")
        {
            m_sendingSensorData = true;
        }
        else if (args[0] == "dontSendSensorsData")
        {
            m_sendingSensorData = false;
        }
        else
        {
            Serial.print("Unknown command:");
            Serial.println(args[0]);
        }
    }

    void sendSensorRead()
    {
        MPU6050* g = m_gyro;
        String str;
        str += "AcX:";
        str += g->AcX;
        str += "|";
        str += "AcY:";
        str += g->AcY;
        str += "|";
        str += "AcZ:";
        str += g->AcZ;
        str += "|";
        str += "GyX:";
        str += g->GyX;
        str += "|";
        str += "GyY:";
        str += g->GyY;
        str += "|";
        str += "GyZ:";
        str += g->GyZ;
        str += "|";
        str += "Temp:";
        str += g->Temp;
        str += "|";
        str += "vuMeter:";
        str += m_vuMeter.value();
        str += "\n";

        for(int i = 0; i < MAX_SRV_CLIENTS; i++){
          if (serverClients[i] && serverClients[i].connected())
          {
            serverClients[i].print(str);
          }
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

    void animateCross()
    {
        off();
        std::vector<ws2812Strip::led*> rside = getRightSide();
        std::vector<ws2812Strip::led*> lside = getLeftSide();

        rside.at(8)->setColor (150,150,0);
        rside.at(9)->setColor (150,150,0);
        rside.at(10)->setColor(150,150,0);
        rside.at(11)->setColor(150,150,0);
        rside.at(12)->setColor(150,150,0);
        rside.at(13)->setColor(150,150,0);
        rside.at(14)->setColor(150,150,0);
        rside.at(15)->setColor(150,150,0);

        rside.at(30)->setColor(120,120,120);
        rside.at(31)->setColor(120,120,120);
        rside.at(32)->setColor(120,120,120);
        rside.at(33)->setColor(120,120,120);
        rside.at(34)->setColor(120,120,120);
        rside.at(35)->setColor(120,120,120);
        rside.at(36)->setColor(120,120,120);
        rside.at(37)->setColor(120,120,120);

        lside.at(8)->setColor (10,10,200);
        lside.at(9)->setColor (10,10,200);
        lside.at(10)->setColor(10,10,200);
        lside.at(11)->setColor(10,10,200);
        lside.at(12)->setColor(10,10,200);
        lside.at(13)->setColor(10,10,200);
        lside.at(14)->setColor(10,10,200);
        lside.at(15)->setColor(10,10,200);

        lside.at(30)->setColor(120,120,120);
        lside.at(31)->setColor(120,120,120);
        lside.at(32)->setColor(120,120,120);
        lside.at(33)->setColor(120,120,120);
        lside.at(34)->setColor(120,120,120);
        lside.at(35)->setColor(120,120,120);
        lside.at(36)->setColor(120,120,120);
        lside.at(37)->setColor(120,120,120);
    }

    void animateFlash()
    {
        setColor(m_counter1,m_counter1,m_counter1);
        if(m_counter1 > 0)
            m_counter1 -= 25;
        else
        {
            resetEffect(); // al vaciar el contador se retorna al efecto anterior
            m_currentEffect = m_nextEffect;
            m_counter1 = m_counter1save;
            m_counter2 = m_counter2save;
            m_counter3 = m_counter3save;
        }
    }

    void animateStationaryRainbow()
    {
        resetEffect();
        m_currentEffect = effectStationaryRainbow;
        animateRainbow();
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
            m_lastArrayOffset = m_arrayOffset;

            m_correctedLedArray.clear();
            uint8_t offsetleds = m_arrayOffset*m_strip.getLeds()->size()/360;
            offsetleds += m_gyro->angleY*m_strip.getLeds()->size()/360;
            if(offsetleds > 360)
                offsetleds -= 360;

            for(int i = offsetleds ; i < m_strip.getLeds()->size() ; i++)
            {
                    ws2812Strip::led* l = &m_strip.getLeds()->at(i);
                    m_correctedLedArray.push_back(l);
            }

            for(int i = 0 ; i < offsetleds ; i++)
            {
                    ws2812Strip::led* l = &m_strip.getLeds()->at(i);
                    m_correctedLedArray.push_back(l);
            }
    }

    std::vector<ws2812Strip::led*> invertLedOrder(const std::vector<ws2812Strip::led*> array)
    {
        std::vector<ws2812Strip::led*> result;
        for(int i = array.size()-1 ; i>= 0  ; i-- )
        {
            result.push_back(array[i]);
        }
        return result;
    }

    std::vector<ws2812Strip::led*> getRightSide()
    {
        std::vector<ws2812Strip::led*> result;
        for(int i = m_correctedLedArray.size()-1 ; i > m_correctedLedArray.size()/2 ; i --)
        {
            result.push_back(m_correctedLedArray.at(i));
        }
        return result;
    }

    std::vector<ws2812Strip::led*> getLeftSide()
    {
        std::vector<ws2812Strip::led*> result;
        for(int i = 0 ; i < m_correctedLedArray.size()/2 ; i ++)
        {
            result.push_back(m_correctedLedArray.at(i));
        }
        return result;
    }

    void paintVuMeterBar(std::vector<ws2812Strip::led*> array, uint8_t percentage, float decay = 0.3)
    {
        if(percentage > 100) percentage = 100;
        int i;
        int max = (percentage * array.size())/100;
        for(i = 0 ; i < max ; i++)
        {
            uint8_t r,g,b,level;
            level = (i+1)*percentage/max;

            if(level <= 0)
            {
                r = 0, g = 0, b = 0;
            }
            else if(level < 30)
            {
                r = 0, g = 0, b = 34+(level*4);
            }
            else if(level < 55)
            {
                r = 0,  g = 116 +(level*2), b = 0;
            }
            else if(level < 70)
            {
                r = 100+level, g = 100+level, b = 0;
            }
            else if(level <=100)
            {
                r = 220, g = 0, b = 0;
            }
            array.at(i)->setColor(r,g,b);
        }

        for(int c = i ; c < array.size() ; c++)
        {
            array.at(c)->dimm(decay);
        }
    }
};
#endif // LEDWHEEL

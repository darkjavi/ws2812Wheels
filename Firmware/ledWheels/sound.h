#ifndef SOUND
#define SOUND

class vuMeter{
  public:
    vuMeter(const int pin): m_pin(pin) , m_lastValue(666) {}
    void update()
    {
      m_lastValue = (analogRead(m_pin)+analogRead(m_pin)+analogRead(m_pin))/3;
    }

    int value()
    {
        return m_lastValue;
    }

    int read()
    {
        return (analogRead(m_pin)+analogRead(m_pin))/2;
    }

  private:
  int m_pin;
  int m_lastValue;

};

#endif // SOUND


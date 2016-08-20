#ifndef SOUND
#define SOUND

class vuMeter{
  public:
    vuMeter(const int pin): m_pin(pin) , m_lastValue(666) {}
    void update()
    {
      int value = 0;
      //for (int i = 0 ; i < 10 ; i++)
      //{
      //  value = ((value*i)+analogRead(m_pin)) / i+1 ;
      //}
      m_lastValue = analogRead(m_pin);
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


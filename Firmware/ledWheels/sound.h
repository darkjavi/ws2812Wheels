#ifndef SOUND
#define SOUND

class vuMeter{
  public:
    vuMeter(const int pin): m_pin(pin) {}
    void update()
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

#endif // SOUND


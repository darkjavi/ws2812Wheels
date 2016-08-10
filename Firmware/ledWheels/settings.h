#ifndef SETTINGS
#define SETTINGS

#ifndef ESP8266 //hack para autocompletar los tipos de dato de arduino en QT
    typedef quint16 uint16_t;
    typedef quint8 uint8_t;
#endif


struct settingList{
    uint8_t micPin         = A0;
    uint8_t ledStripPin    = D8;
    uint8_t lednr          = 83;
    float   ledMaxBright   = 0.3;
    char id[20]            = "LedWheel";
    char wifiESSID[20]     = "LedWheel";
    char wifiPasswd[20]    = "clubmate";
    bool wifiAPMode        = true;
    uint16_t delayTime     = 5;
    uint16_t magicNumber   = 31415;//numero de comprobacion para saber que los settings se han leido/transmitido bien.
};



class EEPROMStorage
{
public:
  EEPROMStorage()
  {
      if(hasSettings())
          Serial.println("Reading EEPROM settings");
      else
          Serial.println("Using hardcoded settings");
      reload();
  }

  settingList& settings()
  {
    return m_settings;
  }

  void reload()
  {
    m_settings = readSettings();
  }

  bool hasSettings()
  {
    if(sizeof(settingList) > m_flashSize) Serial.println("WARNING, Shit may happen!! El tamaño de los settings es mayor que el tamaño de la eeprom!");
    settingList settings;
    settings.magicNumber = 0;
    EEPROM.begin(m_flashSize);
    EEPROM.get(0,settings);
    EEPROM.end();
    Serial.print(settings.magicNumber);
    return settings.magicNumber == 31415;
  }

  settingList readSettings()
  {
    settingList settings;
    settings.magicNumber = 0;
    EEPROM.begin(m_flashSize);
    EEPROM.get(0,settings);
    EEPROM.end();

    if(settings.magicNumber == 31415)
        return settings;
    else
        return settingList(); // Si falla la comprobacion se devuelven los settings por defecto.
  }

  bool setSettings(uint8_t * bytes)
  {
      settingList s;
      s.magicNumber = 0 ;
      for(int i = 0 ; i < sizeof(s.id) ; i++)
          s.id[i] = 0;
      for(int i = 0 ; i < sizeof(s.wifiESSID) ; i++)
          s.wifiESSID[i] = 0;
      for(int i = 0 ; i < sizeof(s.wifiPasswd) ; i++)
          s.wifiPasswd[i] = 0;

      char * byteStorage = (char *)&s;
      for (size_t i = 0; i < sizeof(settingList) ; i++)
        byteStorage[i] = bytes[i];

      if(s.magicNumber == 31415)
      {
          m_settings = s;
          return true;
      }
      else
          return false;
  }

  void setWifiConfig(String essid, String pass, bool AP = false)
  {
     essid.toCharArray(m_settings.wifiESSID,sizeof(m_settings.wifiESSID));
     pass.toCharArray (m_settings.wifiPasswd,sizeof(m_settings.wifiPasswd)) ;
     m_settings.wifiAPMode = AP;
  }

  void getSettingsBytes(uint8_t * result)
  {
      uint8_t * byteStorage = (uint8_t *)&m_settings;
      for (size_t i = 0; i < sizeof(settingList) ; i++) {
        result[i] = byteStorage[i];
      }
  }

  void storeSettings()
  {
    if(sizeof(settingList) > m_flashSize) Serial.println("WARNING, Shit may happen!! El tamaño de los settings es mayor que el tamaño de la eeprom!");
    clearEEPROM();
    EEPROM.begin(m_flashSize);
    Serial.print("Saving..");
    Serial.println(m_settings.wifiESSID);

    EEPROM.put(0,m_settings);
    EEPROM.commit();
    EEPROM.end();
  }

  void clearEEPROM()
  {
    EEPROM.begin(m_flashSize);
    for ( int i = 0 ; i < m_flashSize ; i++ )
      EEPROM.write(i, 0);
    EEPROM.end();
  }

private:
  uint16_t    m_flashSize   = 1024;
  settingList m_settings;
};

#endif // SETTINGS


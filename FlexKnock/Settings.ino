///Handles Settings and Profiles

/*
 * Memory Table:
 * 
 * Base Settings Pointers: 0 - 99
    CHANNELS                       0
    QUITEBOOT                      1
 * 
 * Profile 1
 * [Start Location:] 100
 * [End Location:] 199
 * 
 *  Profile 2
 * [Start Location:] 200
 * [End Location:] 299
 * 
 *  Profile 3
 * [Start Location:] 300
 * [End Location:] 399
 * 
 *  Byte MAP: 
    SPU_SET_BAND_PASS_FREQUENCY    10  
    SPU_SET_PROGRAMMABLE_GAIN      20
    SPU_SET_INTEGRATOR_TIME        30  
    MEASUREMENT_WINDOW_TIME        40 
    DigitalSpeed                   50
    KnockMaxLED                    60      
    EthanolMax                     70 
    FuelTempMax                    80
    TriggerMode                    90
 *
 * 
*/

void RAWREAD(int RAWIN)
{
  Serial.println(EEPROM.read(RAWIN));
}

void RAWWRITE(int ADDR, int VALUE)
{
  EEPROM.update(ADDR, VALUE);  
}

//Write byte of data to location
void WRITEBYTE(int ADDR, int VALUE)
{
  ADDR = PROFILE + 1;
  ADDR = ADDR * 100;
  EEPROM.update(ADDR, VALUE);
}

//Reads byte at this location 
int READBYTE(int ADDR)
{
  //Applys profile offsets.
  ADDR = PROFILE + 1;
  ADDR = ADDR * 100;
  
  return EEPROM.read(ADDR );
}

//Checks that settings are with in normal ranges.
void CheckLimits()
{
  bool OR;
    if (SPU_SET_BAND_PASS_FREQUENCY    <   0x00 || SPU_SET_BAND_PASS_FREQUENCY > 0x3F) { SPU_SET_BAND_PASS_FREQUENCY = 0X00; OR = true;}
    if (SPU_SET_PROGRAMMABLE_GAIN    <   0x80 || SPU_SET_PROGRAMMABLE_GAIN > 0xBF) { SPU_SET_PROGRAMMABLE_GAIN = 0X80; OR = true;}
    if (SPU_SET_INTEGRATOR_TIME    <   0xC0 || SPU_SET_INTEGRATOR_TIME > 0xDF) { SPU_SET_INTEGRATOR_TIME = 0X00; OR = true;}
    if (MEASUREMENT_WINDOW_TIME    <   1000 || MEASUREMENT_WINDOW_TIME > 0x4000) { MEASUREMENT_WINDOW_TIME = 3000; OR = true;}
    if (KnockMaxLED    <   0 || DigitalSpeed > 100) { DigitalSpeed = 80; OR = true;}
    if (EthanolMax    <   0 || EthanolMax > 99) { EthanolMax = 80; OR = true;}
    if (FuelTempMax    <   0 || FuelTempMax > 99) { FuelTempMax = 60; OR = true;}
    if (TriggerMode    <   0 || TriggerMode > 1) { TriggerMode = 0; OR = true;}
    if (DigitalSpeed    <   30 || DigitalSpeed > 3000) { DigitalSpeed = 500; OR = true;}
    if (OR){
    Serial.println(F("One or more settings were out of range!"));
    }
    else
    {
    Serial.println(F("Settings Valid!"));
    }
}


//Loads Settings from eeprom memory or outputs current loaded settings over com port.
void LoadSettings(bool START)
{
  if (START) //Loads from Internal memory.
  {
  if (PROFILE == 0) {}
  else  
  { 
    //Loads Settings From Internal memory
    SPU_SET_BAND_PASS_FREQUENCY    =  READBYTE(10);  
    SPU_SET_PROGRAMMABLE_GAIN    =  READBYTE(20);   
    SPU_SET_INTEGRATOR_TIME     =   READBYTE(30);  
    MEASUREMENT_WINDOW_TIME     =   (READBYTE(40) << 8 ) | READBYTE(40+1); 
    DigitalSpeed =  (READBYTE(50) << 8 ) | READBYTE(50+1);
    KnockMaxLED = READBYTE(60);       
    EthanolMax = READBYTE(70);   
    FuelTempMax = READBYTE(80);
    TriggerMode = READBYTE(90);
  }
    CheckLimits();
    Serial.println(String("Loaded Profile") + PROFILE + String("!"));
  }
  else //Outputs Current loaded settings
  {
        Serial.println(String("SETTINGS:") + SPU_SET_BAND_PASS_FREQUENCY + String("|") + SPU_SET_PROGRAMMABLE_GAIN + String("|") + SPU_SET_INTEGRATOR_TIME + String("|") + MEASUREMENT_WINDOW_TIME
        + String("|") + DigitalSpeed + String("|") + KnockMaxLED + String("|") + EthanolMax + String("|") + FuelTempMax + String("|") + TriggerMode + String("|") + PROFILE);
  }
}

//Saves settings to internal memory
void SaveSettings()
{
CheckLimits();
    WRITEBYTE(10,SPU_SET_BAND_PASS_FREQUENCY);  
    WRITEBYTE(20, SPU_SET_PROGRAMMABLE_GAIN); 
    WRITEBYTE(30, SPU_SET_INTEGRATOR_TIME);  
    WRITEBYTE(40, MEASUREMENT_WINDOW_TIME); 
    WRITEBYTE(41, MEASUREMENT_WINDOW_TIME >> 8);     
    WRITEBYTE(50, DigitalSpeed); 
    WRITEBYTE(51, DigitalSpeed >> 8); 
    WRITEBYTE(60, KnockMaxLED);  
    WRITEBYTE(70, EthanolMax); 
    WRITEBYTE(80, FuelTempMax); 
    WRITEBYTE(90, TriggerMode); 
Serial.println(String("Saved Settings to Profile") + PROFILE + String("!"));
}

//Splits string into parts
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
//Returns string thats requested.
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

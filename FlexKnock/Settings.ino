///Handles Settings and Profiles

/*
 * Memory Table:
 * 
 * Base Settings Pointers:
 * 
 * 
 * Profile 1
 * [Start Location:]
 * [End Location:]
 * 
 *  Profile 2
 * [Start Location:]
 * [End Location:]
 * 
 *  Profile 3
 * [Start Location:]
 * [End Location:]
 * 
 * 
 * 
*/


void LoadSettings()
{
  //To Do:
  //Load settings from eeprom if have been set
  //        SPU_SET_BAND_PASS_FREQUENCY    0b00101010    /* Setting band pass frequency to 7.27kHz */
  //        SPU_SET_PROGRAMMABLE_GAIN      0b10100010    /* Setting programmable gain to 0.381 */
  //        SPU_SET_INTEGRATOR_TIME        0b11001010    /* Setting programmable integrator time constant to 100µs */
  //        MEASUREMENT_WINDOW_TIME        3000 
  //        DigitalSpeed
  // KnockMaxLED = 80;         
  // EthanolMax = 80;     
  // FuelTempMax = 60;
  // TriggerMode
}


void SaveSettings(                                                                                                                                                                                                                                            )
{
  
}


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

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

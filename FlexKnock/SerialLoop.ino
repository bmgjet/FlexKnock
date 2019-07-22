//Handles serial comunications
void SerialMain()
{
 if (time_now >= SerialTimer + 100)//DigitalSpeed) //Check if enough time passed to push Serial Information
  {
  SerialTimer = time_now;                     //Reset Serial timer
  //Prints info over Serial Port.
  if (!QUITE) //Stops comport output.
  {
  Serial.println(F("FLEX-KNOCK:V1.0"));
  Serial.println(String("Ethanol:") + ETHANOL + String("%"));
  Serial.println(String("(D11)Output:") + float(EOUT * 5.0 / 255) + String("V"));
  Serial.println(String("Tempture:") + TEMPERATURE  + String("C"));
  Serial.println(String("(D3)Output:") + float(TOUT * 5.0 / 255) + String("V"));
  Serial.println(String("Knock CH1:") + KNOCKP1 + String("%"));
  Serial.println(String("(D12)Output:") + float(KOUT1 * 5.0 / 255) + String("V"));
    if (CHANNELS)                               //Checks if in single channel mode.
  {
  Serial.println(String("Knock CH2:") + KNOCKP2 + String("%"));
  Serial.println(String("(D2)Output:") + float(KOUT2 * 5.0 / 255) + String("V"));
  }
  }
 if (Serial.available() > 0) {ProgramSettings(Serial.readString());} // read the serial port for Commands
}
}
//Protocol List:
  //SET-BPF XX      SPU_SET_BAND_PASS_FREQUENCY          
  //SET-PG XX       SPU_SET_PROGRAMMABLE_GAIN 
  //SET-IT XX       SPU_SET_INTEGRATOR_TIME 
  //SET-WT XX       MEASUREMENT_WINDOW_TIME 
  //SET-DS XXX      DigitalSpeed
  //SET-KM XX       KnockMaxLED        
  //SET-EM XX       EthanolMax            0 - 99 range
  //SET-FM XX       FuelTempMax           0 - 99 range
  //SET-TM X        TriggerMode           0 off / 1 on
  //SET-SAVE        Saves to selected profile
  //SET-LOAD        Shows Currently loaded settings.
  //SET-QUITE       Stops Comport Traffic.

//Handles settings from Serial port
void ProgramSettings(String Command)
{
  if(Command.indexOf("SET-") >= 0) //Find Command by SET-
  {
    String SET = getValue(Command, ' ', 0); //Splits 2 Parts of command
    SET.replace("SET-", ""); // Remove start
    String GET = "";
    GET = getValue(Command, ' ', 1); //Loads second part of command

//Changes settings based on command input.
if (SET == "BPF"){SPU_SET_BAND_PASS_FREQUENCY = GET.toInt(); TEMPSET();}
else if  (SET == "PG"){SPU_SET_PROGRAMMABLE_GAIN = GET.toInt(); TEMPSET();}
else if  (SET == "IT"){SPU_SET_INTEGRATOR_TIME = GET.toInt(); TEMPSET();}
else if  (SET == "WT"){MEASUREMENT_WINDOW_TIME = GET.toInt(); TEMPSET();}
else if  (SET == "DS"){DigitalSpeed = GET.toInt(); TEMPSET();}
else if  (SET == "KM"){KnockMaxLED = GET.toInt(); TEMPSET();}
else if  (SET == "EM"){EthanolMax = GET.toInt(); TEMPSET();}
else if  (SET == "FM"){FuelTempMax = GET.toInt(); TEMPSET();}
else if  (SET == "TM"){TriggerMode = GET.toInt(); TEMPSET();}
else if  (SET == "SAVE") {SaveSettings();}
else if  (SET == "LOAD") {LoadSettings(0);}
else if  (SET == "QUITE") {if (QUITE){QUITE = false;} else { QUITE = true;}}
else if  (SET == "HELP")
{
  Serial.println(F("Protocol List:"));
  Serial.println(F("SET-BPF XX      SPU_SET_BAND_PASS_FREQUENCY      "));    
  Serial.println(F("SET-PG XX       SPU_SET_PROGRAMMABLE_GAIN "));
  Serial.println(F("SET-IT XX       SPU_SET_INTEGRATOR_TIME "));
  Serial.println(F("SET-WT XX       MEASUREMENT_WINDOW_TIME "));
  Serial.println(F("SET-DS XXX      DigitalSpeed"));
  Serial.println(F("SET-KM XX       KnockMaxLED    "));    
  Serial.println(F("SET-EM XX       EthanolMax            0 - 99 range"));
  Serial.println(F("SET-FM XX       FuelTempMax           0 - 99 range"));
  Serial.println(F("SET-TM X        TriggerMode           0 off / 1 on"));
  Serial.println(F("SET-SAVE        Saves to selected profile"));
  Serial.println(F("SET-LOAD        Shows Currently loaded settings."));
  Serial.println(F("SET-QUITE       Stop/Start Comport Traffic."));
}
//Raw read 
//Raw write to internal memory over comport
else if  (SET == "RAWR"){ RAWREAD(GET.toInt());}
else if  (SET == "RAWW")
{
  RAWWRITE(getValue(GET, ':', 0).toInt(), getValue(GET, ':', 1).toInt());
}
else{Serial.println(F("Unknown Command!"));}
}
else{Serial.println(F("FLEX-KNOCK by BMGJET"));}
}

//Temp Set reply
void TEMPSET()
{
Serial.println(F("Setting Changed But Not Saved!"));
}

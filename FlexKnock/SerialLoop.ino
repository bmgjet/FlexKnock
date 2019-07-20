//Handles serial comunications
void SerialMain()
{
  //Prints info over Serial Port.
 /* Serial.println(String(" "));
  Serial.println(String("FLEX-KNOCK:V1.0"));
  Serial.println(String("Ethanol:") + ETHANOL + String("%"));
  Serial.println(String("Output:") + float(EOUT * 5.0 / 255) + String("V"));
  Serial.println(String("Tempture:") + TEMPERATURE  + String("°C"));
  Serial.println(String("Output:") + float(TOUT * 5.0 / 255) + String("V"));
  Serial.println(String("Knock CH1:") + KNOCKP1 + String("%"));
  Serial.println(String("Output:") + float(KNOCKP1 * 5.0 / 255) + String("V"));
  Serial.println(String("Knock CH2:") + KNOCKP2 + String("%"));
  Serial.println(String("Output:") + float(KNOCKP2 * 5.0 / 255) + String("V"));
  Serial.println(String(" "));
  */
 if (Serial.available() > 0) {ProgramSettings(Serial.readString());} // read the serial port for Commands
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
if (SET == "BPF"){SPU_SET_BAND_PASS_FREQUENCY = GET.toInt();}
else if  (SET == "PG"){SPU_SET_PROGRAMMABLE_GAIN = GET.toInt();}
else if  (SET == "IT"){SPU_SET_INTEGRATOR_TIME = GET.toInt();}
else if  (SET == "WT"){MEASUREMENT_WINDOW_TIME = GET.toInt();}
else if  (SET == "DS"){DigitalSpeed = GET.toInt();}
else if  (SET == "KM"){KnockMaxLED = GET.toInt();}
else if  (SET == "EM"){EthanolMax = GET.toInt();}
else if  (SET == "FM"){FuelTempMax = GET.toInt();}
else if  (SET == "TM"){TriggerMode = GET.toInt();}
else if  (SET == "SAVE") {SaveSettings();}
else if  (SET == "LOAD") {LoadSettings(0);}
else{Serial.print("Unknown Command!");}
}
else{Serial.print("FLEX-KNOCK by BMGJET");}
}

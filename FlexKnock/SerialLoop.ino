//Handles serial comunications
void SerialMain()
{
  //Prints info over Serial Port.
  Serial.println(String(" "));
  Serial.println(String("FLEX-KNOCK:V1.0"));
  Serial.println(String("Ethanol:") + ETHANOL + String("%"));
  Serial.println(String("Output:") + float(EOUT * 5.0 / 255) + String("V"));
  Serial.println(String("Tempture:") + TEMPERATURE  + String("°C"));
  Serial.println(String("Output:") + float(TOUT * 5.0 / 255) + String("V"));
  Serial.println(String("Knock CH1:") + KNOCKP1 + String("%"));
  Serial.println(String("Output:") + float(KNOCKP1 * 5.0 / 255) + String("V"));
  Serial.println(String("Knock CH2:") + KNOCKP2 + String("%"));
  Serial.println(String("Output:") + float(KNOCKP2 * 5.0 / 255) + String("V"));
  Serial.println(String("SETTINGS:") + SPU_SET_BAND_PASS_FREQUENCY + String("|") + SPU_SET_PROGRAMMABLE_GAIN + String("|") + SPU_SET_INTEGRATOR_TIME + String("|") + MEASUREMENT_WINDOW_TIME);
  Serial.println(String(" "));
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

//Handles changing of settings over serial.
//Need to do constant settings using internal eeprom.


//Handles settings from Serial port
void ProgramSettings(String Command)
{
  if(Command.indexOf("SET-") > 0)
  {
    String SET = getValue(Command, ' ', 0);
    SET.replace("SET-", "");
    String GET = getValue(Command, ' ', 1);

if (SET == "BPF"){}
else if  (SET == "PG"){}
else if  (SET == "IT"){}
else if  (SET == "WT"){}
else if  (SET == "DS"){}
else if  (SET == "KM"){}
else if  (SET == "EM"){}
else if  (SET == "FM"){}
else if  (SET == "TM"){}
else{Serial.print("Unknown Command!");}

}
Serial.print("FLEX-Knock by bmgjet.");
}

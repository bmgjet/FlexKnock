//Handles serial comunications
void SerialMain()
{
  //Prints info over Serial Port.
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
 if (Serial.available() > 0) {ProgramSettings(Serial.readString());} // read the serial port for Commands
}

//Handles changing of settings over serial.
//Need to do constant settings using internal eeprom.
void ProgramSettings(String Command)
{
Serial.print("I received:");
Serial.println(Command);
}

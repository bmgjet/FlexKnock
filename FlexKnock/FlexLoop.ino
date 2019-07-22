void FlexMain()
{
  if (!FLEXOFF){  
  getfueltemp(FLEXIN);          //read fuel temp from input duty cycle
  if (REVTICK > 0)              // Avoid dividing by zero, sample in the HZ
    {HZ = 62200 / REVTICK;}     // 3456000ticks per minute, 57600 per second 
    else                     
    {HZ = 0;}                   //needs real sensor test to determine correct tickrate

  //calculate ethanol percentage
    if (HZ > 50)                // Avoid dividing by zero
    {ETHANOL = (HZ-50);}        
    else
    {ETHANOL = 0;}
    if (ETHANOL > 99)           // Avoid overflow in PWM
    {ETHANOL = 99;}

  //MAP Values to sane range.
  //Outputs to analog pin
  //Ethanol 0-5V
  EOUT = map(ETHANOL, 0, 99, 0, 255);      
  analogWrite(FLEXOUTE, EOUT);

  //Fuel Temp 0-5V
  TOUT = map(TEMPERATURE, -40, 125, 0, 255);
  analogWrite(FLEXOUTT, TOUT);
}
}

//Function for getting fuel temp from FlexSensor.
void getfueltemp(int inpPin) {
  HIGHTIME = 0;
  LOWTIME = 0;

//Count length of the Pulse in the PWM signal
  TEMPPULSE = pulseIn(FLEXIN, HIGH);
  if (TEMPPULSE > HIGHTIME) {
    HIGHTIME = TEMPPULSE;
  }

  TEMPPULSE = pulseIn(inpPin, LOW);
  if (TEMPPULSE > LOWTIME) {
    LOWTIME = TEMPPULSE;
  }


  DUTY = ((100 * (HIGHTIME / (double (LOWTIME + HIGHTIME))))); //Calculate duty cycle (integer extra decimal)
  float T = (float(1.0 / float(HZ)));                    //Calculate total period time
  float PERIOD = float(100 - DUTY) * T;                  //Calculate the active period time (100-duty)*T
  float TEMP2 = float(10) * float(PERIOD);               //Convert ms to whole number
  TEMPERATURE = ((40.25 * TEMP2) - 81.25);               //Calculate temperature for display (1ms = -40, 5ms = 80)
  if (TEMPERATURE > 125) {
    TEMPERATURE = 125;                                   //Prevent temperature overflow
  }
  if (TEMPERATURE < -40) {
    TEMPERATURE = -40;                                   //Prevent temperature underflow
  }
}

void FlexMain()
{
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

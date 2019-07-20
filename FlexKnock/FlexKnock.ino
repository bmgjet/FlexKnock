/*
  ______ _           _  __                 _
  |  ____| |         | |/ /                | |
  | |__  | | _____  _| ' / _ __   ___   ___| | __
  |  __| | |/ _ \ \/ /  < | '_ \ / _ \ / __| |/ /
  | |    | |  __/>  <| . \| | | | (_) | (__|   <
  |_|    |_|\___/_/\_\_|\_\_| |_|\___/ \___|_|\_\
  FlexKnock 1.0 (Working Prototype)
  2019 bmgjet.
  Device that uses arduino Nano to convert Knock and Flexfuel signals to 0-5V as well as provide a digital interface.


  PinOut:
  Arduino Nano
  D12 Knock %Out CH1 0-5V
  D11 Flex Ethanol Out 0-5V
  D10 Knock SPI
  D8 Flex Sensor Input
  D7 Knock LED
  D6 Knock Limit LED
  A5 Knock Signal In
  D4 Knock SPI
  D3 Flex Fuel Temp Out.
  D2 Knock %Out CH2 0-5V

*/

//Define included headers.
#include <SPI.h>

//Define registers used.
#define           SPU_SET_PRESCALAR_6MHz         0b01000100    /* 6MHz prescalar with SDO active */
#define           SPU_SET_CHANNEL_1              0b11100000    /* Setting active channel to 1 */
#define           SPU_SET_CHANNEL_2              0b11100001    /* Setting active channel to 1 */
#define           SPU_SET_BAND_PASS_FREQUENCY    0b00101010    /* Setting band pass frequency to 7.27kHz */
#define           SPU_SET_PROGRAMMABLE_GAIN      0b10100010    /* Setting programmable gain to 0.381 */
#define           SPU_SET_INTEGRATOR_TIME        0b11001010    /* Setting programmable integrator time constant to 100µs */
#define           MEASUREMENT_WINDOW_TIME        3000          /* Defining the time window of measurement to 3ms. */

//Define pin assignments.
#define           SPU_NSS_PIN                    10            /* Pin used for chip select in SPI communication. */
#define           SPU_TEST_PIN                   5             /* Pin used for chip select in SPI communication. */
#define           SPU_HOLD_PIN                   4             /* Pin used for chip select in SPI communication. */
#define           LED_STATUS                     7             /* Pin used for power the status LED, indicating we have power. */
#define           LED_LIMIT                      6             /* Pin used for the limit LED. */
#define           UA_ANALOG_INPUT_PIN            5             /* Analog input for knock. */
#define           FLEXIN                         8             /* Define input pin.        */
#define           FLEXOUTE                       11            /* Define Ethanol output pin. */
#define           FLEXOUTT                       3             /* Define Temperture output pin. */
#define           KNOCKCH1                       12            /* Define CH1 Knock output pin. */
#define           KNOCKCH2                       2             /* Define CH2 Knock output pin. */

//Define global variables
volatile uint16_t REVTICK;    /* Ticks per revolution. */
unsigned long time_now = 0;   /* For delay repacement. */
int HZ;                       /* Unsigned 16bit integer for storing HZ input. */
int ETHANOL = 0;              /* Store ethanol percentage here. */
int EOUT = 0;                 /* Store ethanol output voltage 0-5v. */
float TEMPERATURE = 0;        /* Store fuel temperature here. */
float KNOCKP1 = 0;            /* Store knock percentage CH1. */
float KNOCKP2 = 0;            /* Store knock percentage CH1. */
int TOUT = 0;                 /* Store tempture output voltage 0-5v. */
int DUTY;                     /* Duty cycle (0.0-100.0). */
float PERIOD;                 /* Store period time here (eg.0.0025 s). */
static long HIGHTIME = 0;     /* Counter for Flex. */
static long LOWTIME = 0;      /* Counter for Flex. */
static long TEMPPULSE;        /* Counter For Flex. */
bool CHSelect;                /* Switches between Channels. */
unsigned long SerialTimer = 0;/* Value from last time serial updated. */
int DigitalSpeed = 100;       /* User set speed for Serial update. */
int KnockMaxLED = 80;         /* Percentage to trigger Max Knock LED on Board. */

//Function for transfering SPI data to the SPU of Knock Board
byte COM_SPI(byte TX_data) {

  //Set chip select pin low, chip not in use.
  digitalWrite(SPU_NSS_PIN, LOW);

  //Transmit and receive SPI data.
  byte Response = SPI.transfer(TX_data);

  //Set chip select pin high, chip in use.
  digitalWrite(SPU_NSS_PIN, HIGH);

  //Print SPI debug response.
  //Serial.print("SPU_SPI: 0x");
  //Serial.print(Response, HEX);
  //Serial.print("\n\r");

  //Return SPI response.
  return Response;
}

//Function for setting up timer for FlexSensor
void setupTimer()   // setup timer1
{
  TCCR1A = 0;      // normal mode
  TCCR1B = 132;    // (10000100) Falling edge trigger, Timer = CPU Clock/256, noise cancellation on
  TCCR1C = 0;      // normal mode
  TIMSK1 = 33;     // (00100001) Input capture and overflow interupts enabled
  TCNT1 = 0;       // start from 0
}

ISR(TIMER1_CAPT_vect)    // PULSE DETECTED!  (interrupt automatically triggered, not called by main program)
{
  REVTICK = ICR1;        // save duration of last revolution
  TCNT1 = 0;             // restart timer for next revolution
}

ISR(TIMER1_OVF_vect)     // counter overflow/timeout
{
  REVTICK = 0;  // Ticks per second = 0
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
    TEMPERATURE = 125; //Prevent temperature overflow
  }
  if (TEMPERATURE < -40) {
    TEMPERATURE = -40; //Prevent temperature underflow
  }
}

//Function to set up device for operation.
void setup() {
  Serial.begin(38400);      //Open Serial Output


  //To Do:
  //Load settings from eeprom if have been set
  //        SPU_SET_BAND_PASS_FREQUENCY    0b00101010    /* Setting band pass frequency to 7.27kHz */
  //        SPU_SET_PROGRAMMABLE_GAIN      0b10100010    /* Setting programmable gain to 0.381 */
  //        SPU_SET_INTEGRATOR_TIME        0b11001010    /* Setting programmable integrator time constant to 100µs */
  //        MEASUREMENT_WINDOW_TIME        3000 
  //        DigitalSpeed

  //Set up SPI.
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  //Set up Flex Counter
  setupTimer();

  //Set up digital output pins.
  pinMode(SPU_NSS_PIN, OUTPUT);
  pinMode(SPU_TEST_PIN, OUTPUT);
  pinMode(SPU_HOLD_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_LIMIT, OUTPUT);
  pinMode(FLEXIN, INPUT);
  pinMode(FLEXOUTE, OUTPUT);
  pinMode(FLEXOUTT, OUTPUT);


  //Set initial values.
  digitalWrite(SPU_NSS_PIN, HIGH);
  digitalWrite(SPU_TEST_PIN, HIGH);
  digitalWrite(SPU_HOLD_PIN, LOW);

  //Start of operation. (Flash LED's).
  digitalWrite(LED_STATUS, HIGH);
  digitalWrite(LED_LIMIT, HIGH);
  delay(200);
  digitalWrite(LED_STATUS, LOW);
  digitalWrite(LED_LIMIT, LOW);
  delay(200);

}

//Setup Knock options
void KnockSETCH(int i)
{
  COM_SPI(SPU_SET_PRESCALAR_6MHz);
  if (i == 0)                     //Channel One
  {
    COM_SPI(SPU_SET_CHANNEL_1);
    CHSelect = false;
  }
  else                            //Channel Two
  {
    COM_SPI(SPU_SET_CHANNEL_2);
    CHSelect = true;
  }
  //Resets the monitoring perimeters
  COM_SPI(SPU_SET_BAND_PASS_FREQUENCY);
  COM_SPI(SPU_SET_PROGRAMMABLE_GAIN);
  COM_SPI(SPU_SET_INTEGRATOR_TIME);
}


//Main operation function.
void loop() {
  time_now = millis();                        //Timer for delays with out thread freeze.
  KnockSETCH(0);                              //Switches to Channel 1
  KnockMain();                                //Reads Buffer
  KnockSETCH(1);                              //Switches to Channel 2
  KnockMain();                                //Reads Buffer
  FlexMain();                                 //Reads counters from FlexSensor
  if (time_now >= SerialTimer + DigitalSpeed) //Check if enough time passed to push Serial Information
  {
  SerialTimer = time_now;                     //Reset Serial timer
  SerialMain();                               //Do Serial Port work
  }
}

//Function to set up device for operation.
void setup() {
  Serial.begin(38400);      //Open Serial Output

  //Sets up Profile Pins
  pinMode(Profile1, INPUT_PULLUP);
  pinMode(Profile2, INPUT_PULLUP);
  pinMode(Profile3, INPUT_PULLUP);

  //Select Profile
  if (analogRead(A0) < 50)  {    PROFILE = 1;  }
  if (analogRead(A1) < 50)  {    PROFILE = 2;  }
  if (analogRead(A2) < 50)  {    PROFILE = 3;  }

  //Main Settings for boot
  if (EEPROM.read(0) == 1){CHANNELS = 1;} //Channel
  if (EEPROM.read(1) == 1){QUITE = 1;} //Quite Boot
  if (EEPROM.read(2) == 1){FLEXOFF = 1;} //Disable Flex
  if (EEPROM.read(3) == 1){KNOCKOFF = 1;} //Disable Knock
  
  //Loads Settings from internal memory
  LoadSettings(1);

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

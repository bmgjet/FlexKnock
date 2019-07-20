void KnockMain()
{
    //Set status LED on, operation starts.
    digitalWrite(LED_STATUS, HIGH);
    
    //The measurement window starts by driving digital pin 4 high.
    digitalWrite(SPU_HOLD_PIN, HIGH);

    //The SPU performs the integration process and increases the output voltage based on the signal processing result.
    delayMicroseconds(MEASUREMENT_WINDOW_TIME);
    
    //The measurement window ends by driving digital pin 4 low. The SPU stops the integration process and the output voltage is frozen until the window starts again.
    digitalWrite(SPU_HOLD_PIN, LOW);

    //The SPU output voltage is read by the Arduino ADC on analogue input pin 0.
    uint16_t adcValue_UA = analogRead(UA_ANALOG_INPUT_PIN);

    //Convert ADC-value to percentage.
    float knock_percentage = ((float)adcValue_UA / 1023) * 100;

    //Set Limit LED if knock percentage reaches 80%.
    if (knock_percentage >= 80) digitalWrite(LED_LIMIT, HIGH); else digitalWrite(LED_LIMIT, LOW);
if (CHSelect)
{
    KNOCKP2 = knock_percentage;
}
else
{
    KNOCKP1 = knock_percentage;
}
   
    //Set status LED off, operation stops. Inlcuding additional delay time for visibility.
    //delay(100);
    digitalWrite(LED_STATUS, LOW);
    
    //Delay to update serial output every ~500ms. 
    //delay(400);
}

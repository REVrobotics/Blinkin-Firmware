void timerConfiguration(){
  
  // Configure Timer1 interrupt
  //    16,000,000hz/8 = 2,000,000
  //    0.0000005 second = counter tick
  //    1.0 ms pulse = 2000 TCNT1
  TCCR1A = 0;           // set entire TCCR1A register to 0
  TCCR1B = 0;           // set entire TCCR1B register to 0
  TCNT1  = 0;           //initialize counter value to 0
  
  // set compare match register
  OCR1A = 65000; //No signal detected after 0.0325 seconds (max counter of 65536) 
  
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  
  // Set CS11 for 8 prescaler (e.g. 16,000,000/8)
  TCCR1B |= (1 << CS11);
  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
}

ISR(TIMER1_COMPA_vect) { 

  
  patternHistory.unshift(noSignalPatternDisplay); 

  noSignal = true;

  updatedLEDs = false;
  inPulse = false;

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

void ISRrising() {

  detachInterrupt(2);
  inPulse = true;
  noSignal = false;
  updatedLEDs = false;
  
  TCNT1  = 1;//initialize counter value to 1, reset heatbeat, do not set to 0 as this triggers ISR
  prev_time = TCNT1;

  attachInterrupt(digitalPinToInterrupt(2), ISRfalling, FALLING);
}

void ISRfalling() {

  detachInterrupt(2);
  
  pwm_value = TCNT1 - prev_time;

  //TCNT1*0.0000005 = pulse width (seconds)
  if ((pwm_value <= 4000) && (pwm_value >= 1990)) //4400 ~=2.2mS
  {
    if (commandSeq == false)
    {
      //patternHistory.unshift(constrain(map(pwm_value, 2000, 4001, 0, (ARRAY_SIZE(gPatterns))),0,(ARRAY_SIZE(gPatterns)) )); //4400 was 2200ms and 1600 was 800ms
      patternHistory.unshift(constrain(map(pwm_value, 2000, 4000, 0, 100),0,99) ); //4400 was 2200ms and 1600 was 800ms
    }
    else
    {
      gCommands[currCommand](constrain(map(pwm_value, 2000, 4000, 0, 100),0,99));
      
      commandSeq = false;
      setStatusRun();
    }
  }
  else if ((pwm_value > 4000) && (pwm_value <= 4400)) //2.00 ms to 2.20 ms
  {
    if ((inSetup == false))// && (commandSeq == false))
    {
//      if (currCommand != constrain(map(pwm_value, 4000, 4401, 0, 20), 0, 19))
//      {
        commandSeq = true;   
          
        // Indicate Command mode signal detected    
        setStatusCommand();
        
        currCommand = constrain(map(pwm_value, 4000, 4401, 0, 20), 0, 19);
//      }
    }
//    else if ((inSetup == false) && (commandSeq == true))
//    {
//      commandSeq = false;   
//      
//      setStatusRun();
//    }
  }
  else if ((pwm_value < 2000) && (pwm_value >= 1200))
  {
    //patternHistory.unshift(noSignalPatternDisplay);
    //gCommands[map(pwm_value, 4001, 4400, 0, (ARRAY_SIZE(gCommands)))]();
  }

  prev_time = 0;
  inPulse = false;
}

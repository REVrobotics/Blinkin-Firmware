#include "BlinkinFirmware.h"

void setup() {


  //check if defaults have been stored in EEPROM
  if (EEPROM.read(0) != 0xFF) //defaults have been saved, reload variables from them
  {
    addressableStrip = EEPROM.read(0);
    COLOR1 = EEPROM.read(1);
    COLOR2 = EEPROM.read(2);
    //NUM_LEDS = 240;//EEPROM.read(3);
    noSignalPattern = EEPROM.read(4);
  }

  SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);

  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    patternHistory.unshift(0);
  }

  //set timer1 interrupt
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 1, 0 triggers interrupt
  // set compare match register
  OCR1A = 65000; // (must be <65536) 
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  //16,000,000hz/8 = 2,000,000
  //0.0000005 second = counter tick
  //1.0 ms pulse = 2000 TCNT1

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 4500);
  FastLED.setBrightness( BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  pinMode(IND_PIN, OUTPUT);

  pinMode(2, INPUT);
  digitalWrite(2, INPUT_PULLUP);

  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);


  pinMode(sREDPIN,   OUTPUT);
  pinMode(sGREENPIN, OUTPUT);
  pinMode(sBLUEPIN,  OUTPUT);

  digitalWrite(sREDPIN, HIGH);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, LOW);


  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, INPUT_PULLUP);  // set pullup on analog pin 0
  pinMode(SS_PIN, INPUT);
  digitalWrite(SS_PIN, INPUT_PULLUP);  // set pullup on analog pin 0

  //FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1023, 10, 255));

  if (addressableStrip == true) {
    digitalWrite(IND_PIN, LOW);
  }
  else {
    digitalWrite(IND_PIN, HIGH);
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    FastLED.show();
  }

  // when pin D2(Servo input) goes high, call the rising function
  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

//List of patterns.  Each is defined as a separate function in the specified tab.
typedef void (*FunctionList)();
typedef void (*CommandList)(char);

//List of commands.  Each is defined as a separate function in the specified tab.
//typedef void (*SimpleCommandList[])();
CommandList gCommands[] = {
    cmdNoStrip,                   //PWM_0_Command
    cmdNoStrip,                   //PWM_0_Command
    cmdNoStrip,                  //PWM_0_Command
    cmd5VStrip,                  //PWM_0_Command
    cmd12VStrip,                  //PWM_0_Command
    cmdIncreaseBrightness,                  //PWM_0_Command
    cmdDecreaseBrightness,                  //PWM_0_Command
    cmdChangeColor1,                  //PWM_0_Command
    cmdChangeColor2,                  //PWM_0_Command
    cmdChangeDefaultPattern,                  //PWM_0_Command
    cmdChangeDefaultPattern,                  //PWM_0_Command
    cmdChangeDefaultPattern,                  //PWM_0_Command
    cmdChangeColor1,                  //PWM_0_Command
    cmdChangeColor1,                  //PWM_0_Command
    cmdChangeColor1,                  //PWM_0_Command
    cmdChangeColor1,                  //PWM_0_Command
    cmdChangeColor2,                  //PWM_0_Command
    cmdChangeColor2,                  //PWM_0_Command
    cmdChangeColor2,                  //PWM_0_Command
    cmdChangeColor2,                  //PWM_0_Command
};

FunctionList gPatterns[] = {
  rainbow_RGB,                  //PWM_1_Standard
  rainbow_Party,                  //PWM_1_Standard
  rainbow_Ocean,                  //PWM_1_Standard
  rainbow_Lava,                  //PWM_1_Standard
  rainbow_Cloud,                  //PWM_1_Standard
  rainbow_Forest,                  //PWM_1_Standard
  rainbowWithGlitter,           //PWM_1_Standard
  confetti,                     //PWM_1_Standard
  sinelon,                      //PWM_1_Standard
  bpm,                          //PWM_1_Standard
  bpm_RGB,                  //PWM_1_Standard
  bpm_Party,                  //PWM_1_Standard
  bpm_Ocean,                  //PWM_1_Standard
  bpm_Lava,                  //PWM_1_Standard
  rainbow_Cloud,                  //PWM_1_Standard
  bpm_Forest,                  //PWM_1_Standard
  Fire2012,                     //PWM_1_Standard
  EndtoEndStaticBlend_1,        //PWM_2_Color1
  EndtoEndBlend_2,              //PWM_3_Color2
  EndtoEndStaticBlend_2,        //PWM_4_Color2
  teamSparkle,                //PWM_4_Color1_2
  rainbow_Team,                 //PWM_4_Color1_2
  bpm_Team,                     //PWM_4_Color1_2
  EndtoEndBlend,                //PWM_4_Color1_2
  EndtoEndStaticBlend,          //PWM_4_Color1_2
  testPattern,                  //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Blue,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Blue ,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Blue,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Green,                        //PWM_5_Solid
 Red,                          //PWM_5_Solid
 Blue,                         //PWM_5_Solid
 Violet,                        //PWM_5_Solid 
 Violet,                        //PWM_5_Solid 
  HotPink,                      //PWM_5_Solid
  DarkRed,                      //PWM_5_Solid
  Red,                          //PWM_5_Solid
  RedOrange,                    //PWM_5_Solid
  Orange,                       //PWM_5_Solid  
  Gold,                         //PWM_5_Solid
  Yellow,                       //PWM_5_Solid
  LawnGreen,                    //PWM_5_Solid
  Lime,                         //PWM_5_Solid
  DarkGreen,                    //PWM_5_Solid
  Green,                        //PWM_5_Solid
  BlueGreen,                    //PWM_5_Solid
  Aqua,                         //PWM_5_Solid
  SkyBlue,                      //PWM_5_Solid
  DarkBlue,                     //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  BlueViolet,                   //PWM_5_Solid
  Violet,                       //PWM_5_Solid
  White,                        //PWM_5_Solid
  Gray,                         //PWM_5_Solid
  DarkGray,                     //PWM_5_Solid
  Black                        //PWM_5_Solid
};


void loop() {

  buttonHandler();
  if (inSetup) {
    readUserInputs();
  }

  if ((inPulse == false) && (updatedLEDs == false)) {
    detachInterrupt(2);//digitalPinToInterrupt(2));
    ledUpdate();
//    startIndex = startIndex + patternSpeed; /* motion speed */
  
  }

  // do some periodic updates
  EVERY_N_MILLISECONDS( 100 ) {

    // slowly cycle the "base color" through the rainbow, used for some patterns
    gHue++;  

    //Pot 1 - User set pattern density, not used for all patterns 
    patternAdj = constrain(map(analogRead(COLOR1_PIN), 0, 1024, 1, 30), 1, 30);

    //Pot 2 - User set pattern movement speed, not used for all patterns 
    patternSpeed = constrain(map(analogRead(COLOR2_PIN), 0, 1024, 1, 30), 1, 30);
    startIndex = startIndex + patternSpeed; /* motion speed */

    //Pot 3 - user set strip brightness
    FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1024, 10, calculate_max_brightness_for_power_vmA(leds,NUM_LEDS, 255, 5, 4800)));

  }

}

//
ISR(TIMER1_COMPA_vect) { 

  patternHistory.unshift(noSignalPattern); 

  updatedLEDs = false;
  inPulse = false;

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

void ISRrising() {

  detachInterrupt(2);
  inPulse = true;
  updatedLEDs = false;
  
  TCNT1  = 0;//initialize counter value to 1, reset heatbeat, do not set to 0 as this triggers ISR
  prev_time = TCNT1;

  attachInterrupt(digitalPinToInterrupt(2), ISRfalling, FALLING);
}

void ISRfalling() {

  detachInterrupt(2);
  
  pwm_value = TCNT1 - prev_time;

  //TCNT1*0.0000005 = pulse width (seconds)
  if ((pwm_value <= 4010) && (pwm_value >= 1990)) //4400 ~=2.2mS
  {
    if (programSeq == false)
    {
      //patternHistory.unshift(constrain(map(pwm_value, 2000, 4001, 0, (ARRAY_SIZE(gPatterns))),0,(ARRAY_SIZE(gPatterns)) )); //4400 was 2200ms and 1600 was 800ms
      patternHistory.unshift(constrain(map(pwm_value, 2000, 4000, 0, 100),0,99) ); //4400 was 2200ms and 1600 was 800ms
    }
    else
    {
      gCommands[currCommand](constrain(map(pwm_value, 2000, 4000, 0, 100),0,99));
      //gCommands[currCommand](char(88));

    }

  }
  else if ((pwm_value > 4000) && (pwm_value <= 4400)) //2.00 ms to 2.20 ms
  {
    programSeq = true;
    
    digitalWrite(sREDPIN, LOW);
    digitalWrite(sGREENPIN, HIGH);
    digitalWrite(sBLUEPIN, HIGH);
    
    currCommand = constrain(map(pwm_value, 4000, 4401, 0, 20), 0, 19);
    //gCommands[currCommand](0);
  }
  else if ((pwm_value < 2000) && (pwm_value >= 1200))
  {
    //patternHistory.unshift(noSignalPattern);
    //gCommands[map(pwm_value, 4001, 4400, 0, (ARRAY_SIZE(gCommands)))]();
  }

  prev_time = 0;
  inPulse = false;
}

void ledUpdate()
{

  patternStable = true;
        
  //check that the pattern value has been stable. Pattern value is the pattern requested by the user via PWM pulse width measurement.
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    if (patternHistory[0] != patternHistory[i])
      patternStable = false; //if any of the elements of the patternHistory buffer don't match, the pattern is not stable
  }


  if (patternStable) //if the pattern is stable, output and update the current pattern value
  {
    currentPattern = patternHistory[0];
    gPatterns[currentPattern]();
  }
  else //if pattern is not stable, use the previous stable pattern 
  { 
    gPatterns[currentPattern](); 
  }


  // send the 'leds' array out to the actual LED strip
  //30uS per LED for addressable
  if (addressableStrip == true) {
    //update LED display
    FastLED.show();
  }

  updatedLEDs = true; //set status flag to show that LEDs have been updates to latest requested value
  
  //delay(1);
  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
 
}


// This function takes the incoming RGB values and outputs the values
// on three analog PWM output pins to the r, g, and b values respectively.
void showAnalogRGB( const CRGB& rgb)
{

  analogWrite(REDPIN,   rgb.r );
  analogWrite(GREENPIN, rgb.g );
  analogWrite(BLUEPIN,  rgb.b );

//  analogWrite(sREDPIN,   255-rgb.r );
//  analogWrite(sGREENPIN, 255-rgb.g );
//  analogWrite(sBLUEPIN,  255-rgb.b );

}

void displaySolid( CRGB rgb)
{
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, rgb ); 
  }
  else {
     showAnalogRGB( rgb.fadeLightBy(255-map(analogRead(LENGTH_PIN), 0, 1023, 20, 255)) );
  }
}

void toggleStripSelect()
{
    if (addressableStrip) 
    {
      Black();
      FastLED.show();
      digitalWrite(IND_PIN,HIGH);
      addressableStrip = false;
      //delay(20);
      //EEPROM write takes 3.3ms
//      if(writeEEPROM)
//          EEPROM.write(0, addressableStrip);
    }
    else
    {
      Black();
      digitalWrite(IND_PIN,LOW);
      addressableStrip = true;
      //EEPROM write takes 3.3ms
//      if(writeEEPROM)
//        EEPROM.write(0, addressableStrip);
    }
  
}

void setStripSelect(bool newStripState)
{
    if (newStripState != addressableStrip)
       toggleStripSelect();
}









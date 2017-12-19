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
  TCNT1  = 1;//initialize counter value to 1, 0 triggers interrupt
  // set compare match register
  OCR1A = 65000; // (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

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
  digitalWrite(sBLUEPIN, HIGH);


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

//List of commands.  Each is defined as a separate function in the specified tab.
//typedef void (*SimpleCommandList[])();
FunctionList gCommands[] = {
    cmdNoStrip,                   //PWM_0_Command
    cmd5VStrip,                   //PWM_0_Command
    cmd12VStrip,                  //PWM_0_Command
};

//FunctionList gPatterns[] = {
//  rainbow_RGB,                  //PWM_1_Standard
//  rainbow_Party,                  //PWM_1_Standard
//  rainbow_Ocean,                  //PWM_1_Standard
//  rainbow_Lava,                  //PWM_1_Standard
//  rainbow_Cloud,                  //PWM_1_Standard
//  rainbow_Forest,                  //PWM_1_Standard
//  rainbowWithGlitter,           //PWM_1_Standard
//  confetti,                     //PWM_1_Standard
//  sinelon,                      //PWM_1_Standard
//  bpm,                          //PWM_1_Standard
//  bpm_RGB,                  //PWM_1_Standard
//  bpm_Party,                  //PWM_1_Standard
//  bpm_Ocean,                  //PWM_1_Standard
//  bpm_Lava,                  //PWM_1_Standard
//  rainbow_Cloud,                  //PWM_1_Standard
//  bpm_Forest,                  //PWM_1_Standard
//  Fire2012,                     //PWM_1_Standard
//  EndtoEndBlend_1,              //PWM_2_Color1
//  EndtoEndStaticBlend_1,        //PWM_2_Color1
//  EndtoEndBlend_2,              //PWM_3_Color2
//  EndtoEndStaticBlend_2,        //PWM_4_Color2
//  teamSparkle,                //PWM_4_Color1_2
//  rainbow_Team,                 //PWM_4_Color1_2
//  bpm_Team,                     //PWM_4_Color1_2
//  EndtoEndBlend,                //PWM_4_Color1_2
//  EndtoEndStaticBlend,          //PWM_4_Color1_2
//  testPattern,                  //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Green,                        //PWM_5_Solid
// Red,                          //PWM_5_Solid
// Blue,                         //PWM_5_Solid
// Violet,                        //PWM_5_Solid 
// Violet,                        //PWM_5_Solid 
//  HotPink,                      //PWM_5_Solid
//  DarkRed,                      //PWM_5_Solid
//  Red,                          //PWM_5_Solid
//  RedOrange,                    //PWM_5_Solid
//  Orange,                       //PWM_5_Solid  
//  Gold,                         //PWM_5_Solid
//  Yellow,                       //PWM_5_Solid
//  LawnGreen,                    //PWM_5_Solid
//  Lime,                         //PWM_5_Solid
//  DarkGreen,                    //PWM_5_Solid
//  Green,                        //PWM_5_Solid
//  BlueGreen,                    //PWM_5_Solid
//  Aqua,                         //PWM_5_Solid
//  SkyBlue,                      //PWM_5_Solid
//  DarkBlue,                     //PWM_5_Solid
//  Blue,                         //PWM_5_Solid
//  BlueViolet,                   //PWM_5_Solid
//  Violet,                       //PWM_5_Solid
//  White,                        //PWM_5_Solid
//  Gray,                         //PWM_5_Solid
//  DarkGray,                     //PWM_5_Solid
//  Black,                        //PWM_5_Solid
//};

FunctionList gPatterns[] = {
  HotPink,                      //PWM_5_Solid
  rainbow_RGB,                          //PWM_5_Solid
  rainbow_Party,                    //PWM_5_Solid
  Yellow,                       //PWM_5_Solid
  Lime,                         //PWM_5_Solid
  Aqua,                         //PWM_5_Solid
  Blue,                         //PWM_5_Solid
  Violet,                       //PWM_5_Solid
  White,                        //PWM_5_Solid
  Black                         //PWM_5_Solid
};

void loop() {

  buttonHandler();
  if (inSetup) {
    readUserInputs();
  }

  if ((inPulse == false) && (updatedLEDs == false)) {
    detachInterrupt(digitalPinToInterrupt(2));
    ledUpdate();
//    startIndex = startIndex + patternSpeed; /* motion speed */
  
    EVERY_N_MILLISECONDS( 50 ) {
      gHue++;  // slowly cycle the "base color" through the rainbow
      //FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1023, 10, 255));
      FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1023, 10, calculate_max_brightness_for_power_vmA(leds,NUM_LEDS, 255, 5, 4800)));
      
      patternSpeed = map(analogRead(COLOR2_PIN), 0, 1023, 1, 30);
      startIndex = startIndex + patternSpeed; /* motion speed */
  
      patternAdj = map(analogRead(COLOR1_PIN), 0, 1023, 1, 30);
    }
  }

  // do some periodic updates

}

ISR(TIMER1_COMPA_vect) {

  patternHistory.unshift(3);//noSignalPattern);

  updatedLEDs = false;
  inPulse = false;

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

void ISRreset() { //after LEDs have updated and the input PWM line is low set-up for pulse measurement
  //detachInterrupt(digitalPinToInterrupt(2));
  TCNT1  = 1;//initialize counter value to 1, reset heatbeat, do not set to 0 as this triggers ISR
  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

void ISRrising() {
  detachInterrupt(digitalPinToInterrupt(2));
  
  TCNT1  = 1;//initialize counter value to 1, reset heatbeat, do not set to 0 as this triggers ISR
  prev_time = TCNT1;
  inPulse = true;
  updatedLEDs = false;

  attachInterrupt(digitalPinToInterrupt(2), ISRfalling, FALLING);
}

void ISRfalling() {
  detachInterrupt(digitalPinToInterrupt(2));
  pwm_value = TCNT1 - prev_time;

  if ((pwm_value <= 4000) && (pwm_value >= 2000)) //4400 ~=2.2mS
  {
    patternHistory.unshift(map(pwm_value, 2000, 4000, 0, (ARRAY_SIZE(gPatterns)))); //4400 was 2200ms and 1600 was 800ms
  }
  else if ((pwm_value > 4000) && (pwm_value <= 4400))
  {
    gCommands[map(pwm_value, 4001, 4400, 0, (ARRAY_SIZE(gCommands)))]();
  }
  else
  {
      patternHistory.unshift(noSignalPattern);
  }

  prev_time = 0;
  inPulse = false;
}

void ledUpdate()
{
  //check that the pattern value has been stable
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    if (patternHistory[0] == patternHistory[i])
      patternStable = true;
  }

  if (patternStable) {
    gPatterns[patternHistory[0]]();
  }
  else {
    gPatterns[patternHistory.last()];
  }

  patternStable = false;

  // send the 'leds' array out to the actual LED strip
  //30uS per LED for addressable
  if (addressableStrip == true) {
    //update LED display
    FastLED.show();
  }
  
  //attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);  
  updatedLEDs = true;
  attachInterrupt(digitalPinToInterrupt(2), ISRreset, FALLING);

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
      if(writeEEPROM)
          EEPROM.write(0, addressableStrip);
    }
    else
    {
      Black();
      digitalWrite(IND_PIN,LOW);
      addressableStrip = true;
      //EEPROM write takes 3.3ms
      if(writeEEPROM)
        EEPROM.write(0, addressableStrip);
    }
  
}

void setStripSelect(bool newStripState)
{
    if (newStripState != addressableStrip)
       toggleStripSelect();
}









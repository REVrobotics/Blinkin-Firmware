#include "BlinkinFirmware.h"

void setup() {
  delay(1000); // 1 second delay for start-up

  // Setup a default palette and blend style
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND; //or NOBLEND;

  // Hardware enabled inturrupt pin for PWM input
  pinMode(2, INPUT);
  digitalWrite(2, INPUT_PULLUP);

  // RGB Common Annode Strip pins -- connected to transistor, active high
  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);

  // Status LED pins, common annode RGB LED, active low
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

  initEEPROM();
  
  // Set-up the custom palette using the team colors COLOR1 and COLOR2
  SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);

  // Fill the pattern history buffer with values
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    patternHistory.unshift(noSignalPatternDisplay);
  }

//  // FastLED declaration
//  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//  // Enable at a safe starting brightness
//  FastLED.setBrightness( brightness );
//  //FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1023, 10, 255));

  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(brightness);

  // Dual LEDs for indicating which type of LED strip is selected
  //LOW for 5V, HIGH for 12V
  pinMode(IND_PIN, OUTPUT);

  if (addressableStrip == true) {
    digitalWrite(IND_PIN, LOW);
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    FastLED.show();
  }
  else {
    digitalWrite(IND_PIN, HIGH);
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    FastLED.show();
  }

  //FastLED.setMaxRefreshRate(200, true);

  //call timer set-up routine from interrupt tab
  timerConfiguration();

  // when pin D2(Servo input) goes high, call the rising function
  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

//List of patterns.  Each is defined as a separate function in the specified tab.
typedef void (*FunctionList)();
typedef void (*CommandList)(char);

//List of commands.  Each is defined as a separate function in the specified tab.
const CommandList gCommands[]  = {
  cmdNoStrip,                   // 0  ........ PWM_0_Command
  cmd5VStrip,                   // 1  ........ PWM_0_Command
  cmd5VStrip,                   // 2  ........ PWM_0_Command
  cmd12VStrip,                  // 3  ........ PWM_0_Command
  cmd12VStrip,                  // 4  ........ PWM_0_Command
  cmdChangeColor1,              // 5  ........ PWM_0_Command
  cmdChangeColor2,              // 6  ........ PWM_0_Command
  cmdChangeDefaultPattern,      // 7  ........ PWM_0_Command
  cmdSetLinearBlend,            // 8  ........ PWM_0_Command
  cmdSetNoBlend,                // 9  ........ PWM_0_Command
};

const FunctionList gPatterns[]  = {
rainbow_RGB,           //  0..... PWM_1_Standard
rainbow_Party,         //  1..... PWM_1_Standard
rainbow_Ocean,         //  2..... PWM_1_Standard
rainbow_Lava,          //  3..... PWM_1_Standard
rainbow_Forest,        //  4..... PWM_1_Standard
rainbowWithGlitter,    //  5..... PWM_1_Standard
confetti,              //  6..... PWM_1_Standard
shot_Red,              //  7..... PWM_1_Standard
shot_Blue,             //  8..... PWM_1_Standard
shot_White,            //  9..... PWM_1_Standard
sinelon_RGB,           // 10..... PWM_1_Standard
sinelon_Party,         // 11..... PWM_1_Standard
sinelon_Ocean,         // 12..... PWM_1_Standard
sinelon_Lava,          // 13..... PWM_1_Standard
sinelon_Forest,        // 14..... PWM_1_Standard
bpm_RGB,               // 15..... PWM_1_Standard
bpm_Party,             // 16..... PWM_1_Standard
bpm_Ocean,             // 17..... PWM_1_Standard
bpm_Lava,              // 18..... PWM_1_Standard
bpm_Forest,            // 19..... PWM_1_Standard
Fire2012_Low,          // 20..... PWM_1_Standard
Fire2012_High,         // 21..... PWM_1_Standard
drawTwinkles_RGB,      // 22..... PWM_1_Standard
drawTwinkles_Party,    // 23..... PWM_1_Standard
drawTwinkles_Ocean,    // 24..... PWM_1_Standard
drawTwinkles_Lava,     // 25..... PWM_1_Standard
drawTwinkles_Forest,   // 26..... PWM_1_Standard
colorwaves_RGB,        // 27..... PWM_1_Standard
colorwaves_Party,      // 28..... PWM_1_Standard
colorwaves_Ocean,      // 29..... PWM_1_Standard
colorwaves_Lava,       // 30..... PWM_1_Standard
colorwavess_Forest,    // 31..... PWM_1_Standard
larsonScanner_Red,     // 32..... PWM_1_Standard
larsonScanner_Gray,    // 33..... PWM_1_Standard
lightChase_Red,        // 34..... PWM_1_Standard
lightChase_Blue,       // 35..... PWM_1_Standard
lightChase_Gray,       // 36..... PWM_1_Standard
heartBeat_Red,         // 37..... PWM_1_Standard
heartBeat_Blue,        // 38..... PWM_1_Standard
heartBeat_White,       // 39..... PWM_1_Standard
heartBeat_Gray,        // 40..... PWM_1_Standard
breath_Red,            // 41..... PWM_1_Standard
breath_Blue,           // 42..... PWM_1_Standard
breath_Gray,           // 43..... PWM_1_Standard
strobe_Red,            // 44..... PWM_1_Standard
strobe_Blue,           // 45..... PWM_1_Standard
strobe_Gold,           // 46..... PWM_1_Standard
strobe_White,           // 47..... PWM_1_Standard
EndtoEndStaticBlend_1, // 48..... PWM_2_Color1
larsonScanner_Team1,   // 49..... PWM_2_Color1
lightChase_Team1,      // 50..... PWM_2_Color1
heartBeat_Team1s,      // 51..... PWM_2_Color1
heartBeat_Team1m,      // 52..... PWM_2_Color1
heartBeat_Team1f,      // 53..... PWM_2_Color1
breath_Team1s,         // 54..... PWM_2_Color1
breath_Team1f,         // 55..... PWM_2_Color1
shot_Team1,            // 56..... PWM_2_Color1
strobe_Team1,          // 57..... PWM_2_Color1
EndtoEndStaticBlend_2, // 58..... PWM_3_Color2
larsonScanner_Team2,   // 59..... PWM_3_Color2
lightChase_Team2,      // 60..... PWM_3_Color2
heartBeat_Team2s,      // 61..... PWM_3_Color2
heartBeat_Team2m,      // 62..... PWM_3_Color2
heartBeat_Team2f,      // 63..... PWM_3_Color2
breath_Team2s,         // 64..... PWM_3_Color2
breath_Team2f,         // 65..... PWM_3_Color2
shot_Team2,            // 66..... PWM_3_Color2
strobe_Team2,          // 67..... PWM_3_Color2
teamSparkle,           // 68..... PWM_4_Color1_2
teamSparkleinv,        // 69..... PWM_4_Color1_2
rainbow_Team,          // 70..... PWM_4_Color1_2
bpm_Team,              // 71..... PWM_4_Color1_2
EndtoEndBlend,         // 72..... PWM_4_Color1_2
EndtoEndStaticBlend,   // 73..... PWM_4_Color1_2
testPattern,           // 74..... PWM_4_Color1_2
drawTwinkles_Team,     // 75..... PWM_4_Color1_2
colorwaves_Team,       // 76..... PWM_4_Color1_2
sinelon_Team,          // 77..... PWM_4_Color1_2
HotPink,               // 78..... PWM_5_Solid
DarkRed,               // 79..... PWM_5_Solid
Red,                   // 80..... PWM_5_Solid
RedOrange,             // 81..... PWM_5_Solid
Orange,                // 82..... PWM_5_Solid
Gold,                  // 83..... PWM_5_Solid
Yellow,                // 84..... PWM_5_Solid
LawnGreen,             // 85..... PWM_5_Solid
Lime,                  // 86..... PWM_5_Solid
DarkGreen,             // 87..... PWM_5_Solid
Green,                 // 88..... PWM_5_Solid
BlueGreen,             // 89..... PWM_5_Solid
Aqua,                  // 90..... PWM_5_Solid
SkyBlue,               // 91..... PWM_5_Solid
DarkBlue,              // 92..... PWM_5_Solid
Blue,                  // 93..... PWM_5_Solid
BlueViolet,            // 94..... PWM_5_Solid
Violet,                // 95..... PWM_5_Solid
White,                 // 96..... PWM_5_Solid
Gray,                  // 97..... PWM_5_Solid
DarkGray,              // 98..... PWM_5_Solid
Black,                 // 99..... PWM_5_Solid
};


void loop() {

  if ((inPulse == false) && (updatedLEDs == false)) {
    detachInterrupt(2);
    ledUpdate();
  }

  EVERY_N_MILLISECONDS( 25 ) {
    startIndex = startIndex + patternSpeed; /* motion speed */ 

    // slowly cycle the "base color" through the rainbow, used for some patterns
    gHue++;

    buttonHandler();
  }


  // do some periodic updates
  EVERY_N_MILLISECONDS( 150 ) {

    if (inSetup) {
      readUserInputs();
    }

    //Pot 1 - User set pattern density, not used for all patterns
    patternAdj = map(analogRead(COLOR1_PIN), 0, 1024, 0, 30);

    //Pot 2 - User set pattern movement speed, not used for all patterns
    patternSpeed = map(analogRead(COLOR2_PIN), 0, 1024, 0, 30);
    //startIndex = startIndex + patternSpeed; /* motion speed */

    if ((noSignal == true))
      setStatusNoSignal();
    else
      setStatusMode();

    //Pot 3 - user set strip brightness
    if ( inSetup == false ) {
      brightness = map(analogRead(LENGTH_PIN), 0, 1024, 40, calculate_max_brightness_for_power_vmA(leds, stripLength, 255, 5, 4800));
      FastLED.setBrightness(brightness);
    }
  }
}


void ledUpdate()
{

  bool patternStable = true;

  //check that the pattern value has been stable. Pattern value is the pattern requested by the user via PWM pulse width measurement.
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    if (patternHistory[0] != patternHistory[i]){
      patternStable = false; //if any of the elements of the patternHistory buffer don't match, the pattern is not stable
      //setStatusError();
    }
  }

  if ((cmdDisableOutput == false) && (inSetup == false))
  {
    if (patternStable) //if the pattern is stable, output and update the current pattern value
    {
      currentPattern = patternHistory[0];
      gPatterns[currentPattern]();
    }
    else //if pattern is not stable, use the previous stable pattern
    {
      gPatterns[currentPattern]();
    }
  }
  else
  {
    //gPatterns[testPatternDisplay]();
    gPatterns[noSignalPatternDisplay]();
  }

  // send the 'leds' array out to the actual LED strip
//  //30uS per LED for addressable
  if (addressableStrip == true) {
    //update LED display
    FastLED.show();
  }


  updatedLEDs = true; //set status flag to show that LEDs have been updates to latest requested value

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}


// This function takes the incoming RGB values and outputs the values
// on three analog PWM output pins to the r, g, and b values respectively.
void showAnalogRGB( const CRGB& rgb)
{
  analogWrite(REDPIN,   rgb.r );
  analogWrite(GREENPIN, rgb.g );
  analogWrite(BLUEPIN,  rgb.b );
}

void displaySolid( CRGB rgb)
{
  if (addressableStrip == true) {
    //fill_solid( leds, NUM_LEDS, rgb );
    fill_solid( leds, stripLength, rgb );
  }
  else {
    showAnalogRGB( rgb.fadeLightBy(255 - map(analogRead(LENGTH_PIN), 0, 1024, 20, 255)) );
  }
}


void toggleStripSelect()
{
  cmdDisableOutput = false;
  if (addressableStrip)
  {
    Black();
    FastLED.show();
    digitalWrite(IND_PIN, HIGH);
    addressableStrip = false;

    //EEPROM write takes 3.3ms
    if(writeEEPROM)
        EEPROM.write(SS_EE, addressableStrip);
  }
  else
  {
    Black();
    digitalWrite(IND_PIN, LOW);
    addressableStrip = true;

    //EEPROM write takes 3.3ms
    if(writeEEPROM)
      EEPROM.write(SS_EE, addressableStrip);
  }

}


void setStripSelect(bool newStripState)
{
  cmdDisableOutput = false;
  if (newStripState != addressableStrip)
    toggleStripSelect();
}



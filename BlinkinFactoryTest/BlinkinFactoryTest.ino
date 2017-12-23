#include <TimerOne.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 240
#define FRAMES_PER_SECOND 120



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

boolean cmdBrightness = false;
boolean cmdDisableOutput = false;

boolean addressableStrip = true;

#define HOT_PINK 0xFF00AA
#define DARK_RED 0x990000
#define RED      0xFF0000
#define RED_ORNG 0xFF6A00
#define ORANGE   0xFF8C00
#define GOLD     0xFFEA00
#define YELLOW   0xFFFF00
#define LAWN_GRN 0xBFFF00
#define LIME     0x80FF00
#define DARK_GRN 0x009900
#define GREEN    0x00FF00
#define BLUE_GRN 0x00FFAA
#define AQUA     0x00FFFF
#define SKYBLUE  0x0080FF
#define DARK_BLU 0x000099
#define BLUE     0x0000FF
#define B_VIOLET 0x8000FF
#define VIOLET   0xAA00FF
#define WHITE    0xFFFFFF
#define GRAY     0x4D4D4D
#define DARKGRAY 0x1A1A1A
#define BLACK    0x000000

                                //0xFF00AA, 0x990000, 0xFF0000, 0xFF6A00, 0xFF8C00, 0xFFEA00, 0xFFFF00, 0xBFFF00, 0x80FF00, 0x009900, 0x00FF00, 0x00FFAA, 0x00FFFF, 0x0080FF, 0x000099, 0x0000FF, 0x8000FF, 0xAA00FF, 0xFFFFFF, 0x4D4D4D, 0x1A1A1A, 0x000000, 
//const CRGB colorList[] PROGMEM = {HOT_PINK, DARK_RED, RED     , RED_ORNG, ORANGE  , GOLD    , YELLOW  , LAWN_GRN, LIME    , DARK_GRN, GREEN   , BLUE_GRN, AQUA    , SKYBLUE , DARK_BLU, BLUE    , B_VIOLET, VIOLET  , WHITE   , GRAY    , DARKGRAY, BLACK};


//void bpm(CRGBPalette16 palette);

#define TIMER_PERIOD  10000 //microseconds

#define LED_PIN     7 //Data line for addressable strip
#define IND_PIN     4 //Digital pin to indicate Analog vs Addressable LEDs Strip Select
#define MODE_PIN    A3 //Button 2 - Programming Mode Button
#define SS_PIN      A2 //LED Strip type select button

//Analog RGB LED Strip Pins
#define REDPIN   5
#define GREENPIN 6
#define BLUEPIN  3

////Analog Status RGB LED Pins
#define sREDPIN   9
#define sGREENPIN 10
#define sBLUEPIN  11

//Analog Inputs for Color Selections
#define COLOR1_PIN      A6 
#define COLOR2_PIN      A7 
#define LENGTH_PIN      A0

#define CLR_OFF         0
#define CLR_BLUE        1
#define CLR_GREEN       2
#define CLR_CYAN        3
#define CLR_RED         4
#define CLR_MAGENTA     5
#define CLR_YELLOW      6
#define CLR_WHITE       7

#define AN_WINDOW      100
#define AN_MIN         (511 - AN_WINDOW)
#define AN_MAX         (512 + AN_WINDOW)



int sytemState = 0;

//   1 __     __
//   0   |___|
//     0   1   2

volatile bool modePressed = false;
volatile bool modePressRelease = false;
volatile bool modeFalling = false;
volatile int modeSequence = 0;


volatile int modeState = 0;
volatile int prevModeState = 0;
volatile int modeHistory[3] = {1,1,1};
volatile int modeHistoryCount = 0;

volatile bool SSPressed = false;
volatile bool SSPressRelease = false;
volatile bool SSFalling = false;
volatile int SSSequence = 0;

volatile int SSState = 0;
volatile int prevSSState = 0;
volatile int SSHistory[3] = {1,1,1};
volatile int SSHistoryCount = 0;

volatile bool statusLEDBlink = true;
volatile long statusLEDBlinkPeriod = 50;
volatile int statusLEDColor = 0;
volatile long statusLEDCounter = 1;
volatile bool statusLEDToggle = false; 

volatile int testState = 0;
volatile bool testPass = false;

volatile int color1Value = 0;
volatile int color2Value = 0;
volatile int lengthValue = 0;

void addressableSetup()
{
  // FastLED declaration
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  // Enable at a safe starting brightness
  FastLED.setBrightness( 255 );

  
}

void setup() {
  addressableSetup();

  
  //Button pins
  pinMode(MODE_PIN, INPUT);
  pinMode(SS_PIN, INPUT);
  digitalWrite(MODE_PIN, INPUT_PULLUP);
  digitalWrite(SS_PIN, INPUT_PULLUP);

  //RGB Strip pins
  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);

  //Status LED Pins
  pinMode(sREDPIN,   OUTPUT);
  pinMode(sGREENPIN, OUTPUT);
  pinMode(sBLUEPIN,  OUTPUT);

  digitalWrite(sREDPIN, HIGH);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, HIGH);

  pinMode(IND_PIN, OUTPUT);
  digitalWrite(IND_PIN, LOW);
  
  //TODO - Setup Addressable Strip stuff

  statusLEDColor = CLR_RED;
  statusLEDBlinkPeriod = 5;   //set status LED blink period
  statusLEDBlink = true;        //set status LED to blink
  
  Timer1.initialize(TIMER_PERIOD);
  Timer1.attachInterrupt(timerISR);
  Timer1.start();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(modePressRelease)
  {
    modePressRelease = false;
    //digitalWrite(IND_PIN, HIGH);
    statusLEDColor = CLR_BLUE;
  }

  if(SSPressRelease)
  {
    SSPressRelease = false;
    statusLEDColor = CLR_YELLOW;
  }

  switch(testState)
  {
    case 0:
      setStatusLEDTesting();
      setRGBStripColor(CLR_OFF);
      
      fill_solid( leds, NUM_LEDS, CRGB::Black );
      FastLED.show();
      
      color1Value = analogRead(COLOR1_PIN);
      color2Value = analogRead(COLOR2_PIN);
      lengthValue = analogRead(LENGTH_PIN);
      if(color1Value > AN_MAX || color1Value < AN_MIN)
      {
        testState = 6;
      }
      else if(color2Value > AN_MAX || color2Value < AN_MIN)
      {
        testState = 6;
      }
      else if(lengthValue > AN_MAX || lengthValue < AN_MIN)
      {
        testState = 6;
      }
      else
      {
        testState++;
      }
      break;
    case 1:
      //Indicate Low
      digitalWrite(IND_PIN, LOW);
      if(modePressRelease)
      {
        modePressRelease = false;
        testState = 6;
      }
      else if(SSPressRelease)
      {
        SSPressRelease = false;
        testState++;
      }
      break;
    case 2:
      //Indicate high
      digitalWrite(IND_PIN, HIGH);
      if(SSPressRelease)
      {
        SSPressRelease = false;
        testState = 6;
      }
      else if(modePressRelease)
      {
        modePressRelease = false;
        testState++;
      }
      break;
    case 3:
      setRGBStripColor(CLR_RED);
      statusLEDColor = CLR_RED;
      statusLEDBlink = false;

      fill_solid( leds, NUM_LEDS, CRGB::Red );
      FastLED.show();
      
      if(SSPressRelease)
      {
        SSPressRelease = false;
        testState = 6;
      }
      else if(modePressRelease)
      {
        modePressRelease = false;
        testState++;
      }
      break;
    case 4:
      setRGBStripColor(CLR_GREEN);
      statusLEDColor = CLR_GREEN;
      statusLEDBlink = false;

      fill_solid( leds, NUM_LEDS, CRGB::Green );
      FastLED.show();
      
      if(SSPressRelease)
      {
        SSPressRelease = false;
        testState = 6;
      }
      else if(modePressRelease)
      {
        modePressRelease = false;
        testState++;
      }
      break;
    case 5:
      setRGBStripColor(CLR_BLUE);
      statusLEDColor = CLR_BLUE;
      statusLEDBlink = false;

      fill_solid( leds, NUM_LEDS, CRGB::Blue );
      FastLED.show();
      
      if(SSPressRelease)
      {
        SSPressRelease = false;
        testState = 6;
      }
      else if(modePressRelease)
      {
        modePressRelease = false;
        testState = 7;
      }
      break;
      break;
    case 6:
      setStatusLEDFailed();
      if(SSPressRelease)
      {
        SSPressRelease = false;
        testState = 0;
      }
      break;
    case 7:
      setStatusLEDPassed();
      if(SSPressRelease)
      {
        SSPressRelease = false;
        testState = 0;
      }
      break;  
      
  }

}

void setStatusLEDTesting()
{
  statusLEDColor = CLR_YELLOW;
  statusLEDBlinkPeriod = 50;
  statusLEDBlink = true;
}

void setStatusLEDFailed()
{
  statusLEDColor = CLR_RED;
  statusLEDBlinkPeriod = 5;
  statusLEDBlink = true;
}

void setStatusLEDPassed()
{
  statusLEDColor = CLR_GREEN;
  statusLEDBlinkPeriod = 5;
  statusLEDBlink = true;
}

void setStatusLEDColor(int color)
{
  switch(color)
  {
    case CLR_OFF:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_BLUE:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_GREEN:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_CYAN:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_RED:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, LOW);
      break;
    case CLR_MAGENTA:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, LOW);
      break;
    case CLR_YELLOW:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, LOW);
      break;
    case CLR_WHITE:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, LOW);
      break;
    default:
      break;
  }
}

void setRGBStripColor(int color)
{
  switch(color)
  {
    case CLR_OFF:
      digitalWrite(BLUEPIN, LOW);
      digitalWrite(GREENPIN, LOW);
      digitalWrite(REDPIN, LOW);
      break;
    case CLR_BLUE:
      digitalWrite(BLUEPIN, HIGH);
      digitalWrite(GREENPIN, LOW);
      digitalWrite(REDPIN, LOW);
      break;
    case CLR_GREEN:
      digitalWrite(BLUEPIN, LOW);
      digitalWrite(GREENPIN, HIGH);
      digitalWrite(REDPIN, LOW);
      break;
    case CLR_CYAN:
      digitalWrite(BLUEPIN, HIGH);
      digitalWrite(GREENPIN, HIGH);
      digitalWrite(REDPIN, LOW);
      break;
    case CLR_RED:
      digitalWrite(BLUEPIN, LOW);
      digitalWrite(GREENPIN, LOW);
      digitalWrite(REDPIN, HIGH);
      break;
    case CLR_MAGENTA:
      digitalWrite(BLUEPIN, HIGH);
      digitalWrite(GREENPIN, LOW);
      digitalWrite(REDPIN, HIGH);
      break;
    case CLR_YELLOW:
      digitalWrite(BLUEPIN, LOW);
      digitalWrite(GREENPIN, HIGH);
      digitalWrite(REDPIN, HIGH);
      break;
    case CLR_WHITE:
      digitalWrite(BLUEPIN, HIGH);
      digitalWrite(GREENPIN, HIGH);
      digitalWrite(REDPIN, HIGH);
      break;
    default:
      break;
  }
}

void statusLEDHandler()
{
  if(statusLEDBlink)
  {
    if(statusLEDCounter % statusLEDBlinkPeriod == 0)
    {
      statusLEDCounter = 1;
      if(statusLEDToggle)
      {
        statusLEDToggle = false;
      }
      else
      {
        statusLEDToggle = true;
      }
    }
    else
    {
      statusLEDCounter++;
    }
  }
  else
  {
    statusLEDCounter = 0;
    statusLEDToggle = true;
  }

  if(statusLEDToggle)
  {
    setStatusLEDColor(statusLEDColor);
  }
  else
  {
    setStatusLEDColor(CLR_OFF);
  }
    
}

void buttonHandler()
{
  int modeSum = 0;
  int SSSum = 0;
  
  modeHistory[modeHistoryCount] = digitalRead(MODE_PIN);
  SSHistory[SSHistoryCount] = digitalRead(SS_PIN);

  modeHistoryCount++;
  if(modeHistoryCount > 2)
  {
    modeHistoryCount = 0;
  }

  for(int i = 0; i <= 2; i++)
  {
    modeSum += modeHistory[i];
  }

  SSHistoryCount++;
  if(SSHistoryCount > 2)
  {
    SSHistoryCount = 0;
  }

  for(int i = 0; i <= 2; i++)
  {
    SSSum += SSHistory[i];
  }

  if(modeSum == 0)
  {
    prevModeState = modeState;
    modeState = 0;
  }
  else if(modeSum == 3)
  {
    prevModeState = modeState;
    modeState = 1;
  }
  
  if(SSSum == 0)
  {
    prevSSState = SSState;
    SSState = 0;
  }
  else if(SSSum == 3)
  {
    prevSSState = SSState;
    SSState = 1;
  }

  switch(modeSequence)
  {
    case 0:
      // button is not pressed
      modePressed = false;
      if(modeState < prevModeState) //falling edge
      {
        modeSequence = 1;
      }
      break;
    case 1:
      //button is currently pressed
      modePressed = true;
      
      if(modeState > prevModeState) //rising edge
      {
        modeSequence = 2;
      }
      break;
    case 2:
      //button has been pressed and released
      modePressed = false;
      modePressRelease = true; //this will be cleared upon checking if there was a press and release.
      modeSequence = 0;
      break;
    default:
      break;
  }

  switch(SSSequence)
  {
    case 0:
      // button is not pressed
      SSPressed = false;
      if(SSState < prevSSState) //falling edge
      {
        SSSequence = 1;
      }
      break;
    case 1:
      //button is currently pressed
      SSPressed = true;
      if(SSState > prevSSState) //rising edge
      {
        SSSequence = 2;
      }
      break;
    case 2:
      //button has been pressed and released
      SSPressed = false;
      SSPressRelease = true; //this will be cleared upon checking if there was a press and release.
      SSSequence = 0;
      break;
    default:
      break;
  }
}

void timerISR()
{
  buttonHandler();
  statusLEDHandler();
  
}


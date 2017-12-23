//#define FASTLED_INTERRUPT_RETRY_COUNT 3
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include <EEPROM.h>

#define CIRCULAR_BUFFER_XS
#include <CircularBuffer.h>

FASTLED_USING_NAMESPACE

#define writeEEPROM false

#define LED_PIN     7 //Data line for addressable strip
#define IND_PIN     4 //Digital pin to indicate Analog vs Addressable LEDs Strip Select
#define MODE_PIN    A3 //Button 2 - Programming Mode Button
#define SS_PIN      A2 //LED Strip type select button

//Analog RGB LED Strip Pins
#define REDPIN   5  //OC0B
#define GREENPIN 6  //OC0A
#define BLUEPIN  3  //OC2B

////Analog Status RGB LED Pins
#define sREDPIN   9   //OC1A
#define sGREENPIN 10  //OC1B
#define sBLUEPIN  11  //OC2A

//Analog Inputs for Color Selections
#define COLOR1_PIN      A6 
#define COLOR2_PIN      A7 
#define LENGTH_PIN      A0 

#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 144
#define FRAMES_PER_SECOND 120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define NOSIGNALPATTERN 8
#define TESTPATTERN 25

//unsigned int NUM_LEDS = 240;

CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

boolean cmdBrightness = false;
boolean cmdDisableOutput = false;

volatile uint16_t pwm_value = 1500;
volatile uint16_t prev_time = 0;

volatile boolean inPulse = false;
volatile boolean updatedLEDs = true;
volatile boolean inSetup = false;
boolean noSignal = false;

unsigned long modeButtonHoldCount = 0;
unsigned int ssButtonHoldCount = 0;
unsigned int programButtonHoldCount = 0;
boolean setupTransistion = false;
boolean stripTransistion = false;

boolean addressableStrip = true;

boolean commandSeq = false;
char currCommand = 0;

CRGBPalette16 currentPalette;
CRGBPalette16 teamPalette;
TBlendType    currentBlending;

CircularBuffer<short,5> patternHistory;
short currentPattern = 0;  
bool patternStable = true;

CircularBuffer<short,15> lengthHistory; 
bool lengthStable = false;

CircularBuffer<short,5> color1History; 
bool color1Stable = false;
char COLOR1 = 13;
char COLOR1temp = COLOR1;

CircularBuffer<short,5> color2History;  
bool color2Stable = false;
char COLOR2 = 5;
char COLOR2temp = COLOR2;

uint8_t noSignalPatternDisplay = NOSIGNALPATTERN;
uint8_t testPatternDisplay = TESTPATTERN;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
static uint8_t startIndex = 0;
uint8_t patternSpeed = 5;
uint8_t patternAdj = 3;



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
const CRGB colorList[] PROGMEM = {HOT_PINK, DARK_RED, RED     , RED_ORNG, ORANGE  , GOLD    , YELLOW  , LAWN_GRN, LIME    , DARK_GRN, GREEN   , BLUE_GRN, AQUA    , SKYBLUE , DARK_BLU, BLUE    , B_VIOLET, VIOLET  , WHITE   , GRAY    , DARKGRAY, BLACK};


void bpm(CRGBPalette16 palette);

//#define FASTLED_INTERRUPT_RETRY_COUNT 3
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include <EEPROM.h>

#define CIRCULAR_BUFFER_XS
#include <CircularBuffer.h>

//FASTLED_USING_NAMESPACE

#define writeEEPROM false

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

#define BRIGHTNESS  120
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 240
#define FRAMES_PER_SECOND 120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

//unsigned int NUM_LEDS = 240;

CRGB leds[NUM_LEDS];
CRGB endclr;
CRGB midclr;

volatile uint16_t pwm_value = 1500;
volatile uint16_t prev_time = 0;

volatile boolean inPulse = false;
volatile boolean updatedLEDs = true;
volatile boolean inSetup = false;

unsigned long modeButtonHoldCount = 0;
unsigned int ssButtonHoldCount = 0;

boolean addressableStrip = true;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CircularBuffer<short,5> patternHistory;  
bool patternStable = true;

CircularBuffer<short,15> lengthHistory; 
bool lengthStable = true;

CircularBuffer<short,5> color1History; 
bool color1Stable = true;
char COLOR1 = 5;

CircularBuffer<short,5> color2History;  
bool color2Stable = true;
char COLOR2 = 3;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
static uint8_t startIndex = 0;
uint8_t patternSpeed = 5;


const CRGB colorList[] PROGMEM = {CRGB::HotPink, CRGB::DarkRed, CRGB::Red, CRGB::DarkOrange, CRGB::Yellow, CRGB::Gold, CRGB::LawnGreen, CRGB::Green, CRGB::DarkGreen, CRGB::Aqua, CRGB::Teal, CRGB::Blue, CRGB::Navy, CRGB::Fuchsia, CRGB::BlueViolet , CRGB::DimGray, CRGB::White, CRGB::Black};



#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define LED_PIN     6
#define IND_PIN     5

#define COLOR1_PIN      A0
#define COLOR2_PIN      A1
#define ALT_PIN         A2
#define LENGTH_PIN      A3

#define NUM_LEDS    131
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGB leds[NUM_LEDS];

volatile uint16_t pwm_value = 0;
volatile uint16_t prev_time = 0;
volatile uint16_t old_pwm = 0;
volatile uint16_t prev_pwm = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern const TProgmemPalette16 BlueYellow PROGMEM;
extern const TProgmemPalette16 Yellow PROGMEM;
extern const TProgmemPalette16 Blue PROGMEM;

void setup() {

  Serial.begin(9600);

  pinMode(IND_PIN, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // when pin D2 goes high, call the rising function
  attachInterrupt(0, rising, RISING);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, Fire2012 };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 
void loop() {

  noInterrupts();
  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[pwm_value]();

  interrupts();

  // send the 'leds' array out to the actual LED strip
  FastLED.show(); 
  
  prev_time = 0;
  
  // insert a delay to keep the framerate modest
  delay(20); 

  gHue = map(analogRead(ALT_PIN), 0, 1024, 0, 256);
}
 
void rising() {
  attachInterrupt(0, falling, FALLING);
  prev_time = micros();
}

void falling() {
  old_pwm = pwm_value;
  attachInterrupt(0, rising, RISING);
  pwm_value = micros()-prev_time;
  

  if (pwm_value > 2090)
  {
    pwm_value = old_pwm;
  }
  if (pwm_value < 750)
  {
    pwm_value = old_pwm;
  }
  else
  {
    pwm_value = map(pwm_value, 762, 2080, 0, (ARRAY_SIZE(gPatterns)));
  }

  Serial.println(pwm_value);
  
  prev_time = 0;

}








void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

#define SPARKING 120
#define COOLING  55
bool gReverseDirection = false;
void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

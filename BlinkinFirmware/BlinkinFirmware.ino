//#define FASTLED_INTERRUPT_RETRY_COUNT 3
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define CIRCULAR_BUFFER_XS
#include <CircularBuffer.h>

//FASTLED_USING_NAMESPACE

#define LED_PIN     7 //Data line for addressable strip
#define IND_PIN     4 //Digital pin to indicate Analog vs Addressable LEDs Strip Select
#define MODE_PIN    A3 //Button 2 - Programming Mode Button
#define SS_PIN      A2 //LED Strip type select button

//Analog RGB LED Strip Pins
#define REDPIN   5
#define GREENPIN 6
#define BLUEPIN  3

//Analog Inputs for Color Selections
#define COLOR1_PIN      A0 //Analog2
#define COLOR2_PIN      A6 //Analog0
#define LENGTH_PIN      A7 //Analog1

#define BRIGHTNESS  128
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 144
#define FRAMES_PER_SECOND 120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGB leds[NUM_LEDS];

volatile uint16_t pwm_value = 1500;
volatile uint16_t prev_time = 0;
volatile uint16_t old_pwm = 1500;
volatile uint16_t prev_pwm = 1500;

volatile boolean inPulse = false;
volatile boolean updatedLEDs = true;
volatile uint16_t pulseCount = 1500;

volatile boolean prevModeButtonState = HIGH;
volatile boolean currModeButtonState = HIGH;
volatile uint16_t buttonHoldCount = 0;

volatile boolean prevSSButtonState = HIGH;
volatile boolean currSSButtonState = HIGH;

boolean addressableStrip = true;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

volatile int currentPattern = 0;
//volatile int prevPattern = 0;
//volatile int prev2Pattern = 0;

//int patternHistory[] = { 0, 0, 0, 0 ,0 };
CircularBuffer<short,5> patternHistory; // uses 538 bytes 
bool patternStable = true;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//typedef void (*SimplePatternList[])();
//SimplePatternList potColors = { "HotPink", "Red", "DarkOrange", "Yellow", "Gold", "Lime", "Green", "Aqua", "Blue", "BlueViolet", "White", "Black"};


//char* potColors[] = { "HotPink", "Red", "DarkOrange", "Yellow", "Gold", "Lime", "Green", "Aqua", "Blue", "BlueViolet", "White", "Black"};


void setup() {

  //Intialize ring buffer
  patternHistory.unshift(0);
  patternHistory.unshift(0);
  patternHistory.unshift(0);
  patternHistory.unshift(0);
  patternHistory.unshift(0);

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 1;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 65000;//8192;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  //TCCR1B |= (1 << CS12) | (1 << CS10);
  // Set CS11 for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  // enable timer overflow interrupt
  //TIMSK1 |= (1 << TOIE1);
  

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  pinMode(IND_PIN, OUTPUT);

  pinMode(2, INPUT);
  digitalWrite(2, INPUT_PULLUP);

  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);

  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, INPUT_PULLUP);  // set pullup on analog pin 0
  pinMode(SS_PIN, INPUT);
  digitalWrite(SS_PIN, INPUT_PULLUP);  // set pullup on analog pin 0


  if (addressableStrip == true) {
    digitalWrite(IND_PIN, LOW);
  }
  else {
    digitalWrite(IND_PIN, HIGH);
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    FastLED.show();
  }

  // when pin D2(Servo input) goes high, call the rising function
  attachInterrupt(digitalPinToInterrupt(2), rising, RISING);

}

//List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { Black, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, Fire2012, teamSparkle, HotPink, Red, DarkOrange, Yellow, Gold, Lime, Green, Aqua, Blue, BlueViolet, White };


void loop() {


  if ((inPulse == false) && (updatedLEDs == false))
  {

    // send the 'leds' array out to the actual LED strip
    //30uS per LED for addressable
    if (addressableStrip == true) {
      //gPatterns[currentPattern]();

      //check that the pattern value has been stable 
      for (int i = 0 ; i< patternHistory.capacity() ; i++){
        if (patternHistory[0] != patternHistory[i])
          patternStable = false;
      }


      if (patternStable){
        gPatterns[patternHistory[0]]();        
      }
      else{
        gPatterns[patternHistory.last()];
      }

      
      //update LED display
      FastLED.show();

      patternStable = true;
    }

    attachInterrupt(digitalPinToInterrupt(2), rising, RISING);
    updatedLEDs = true;

  }

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}


ISR(TIMER1_COMPA_vect) { //timer1 interrupt 1Hz
//ISR(TIMER1_OVF_vect){
  currentPattern = 13;
  //gPatterns[currentPattern]();
  updatedLEDs = false;
  inPulse = false;
  //TCNT1  = 0;//initialize counter value to 0, reset heatbeat


  attachInterrupt(digitalPinToInterrupt(2), rising, RISING);


  /*currSSButtonState = digitalRead(SS_PIN);

    if (currSSButtonState == LOW && prevSSButtonState == LOW){
    if (addressableStrip) {
      Black();
      FastLED.show();
      digitalWrite(IND_PIN,HIGH);
      addressableStrip = false;
       //delay(20);
    }
    else{
      Black();
      digitalWrite(IND_PIN,LOW);
      addressableStrip = true;
      //delay(20);
    }
    }
    prevSSButtonState = currSSButtonState;*/
}

void rising() {
  //prev_time = micros();
  TCNT1  = 1;//initialize counter value to 0, reset heatbeat
  prev_time = TCNT1;
  inPulse = true;
  updatedLEDs = false;

  attachInterrupt(digitalPinToInterrupt(2), falling, FALLING);
}

void falling() {
  detachInterrupt(digitalPinToInterrupt(2));
  old_pwm = pwm_value;
  //pwm_value = micros() - prev_time;
  pwm_value = TCNT1 - prev_time;
  prev_time = 0;
  //TCNT1  = 1;//initialize counter value to 0, reset heatbeat

  //If time value is reasonable call and update the LED outputs
  //if value is unreasonable, disregard reading and use last value to update LED outputs
  //reset interrupt


  //if ((pwm_value < 2300) && (pwm_value > 750))
  if ((pwm_value < 4800) && (pwm_value > 1200))
  {
    //pwm_value
    //currentPattern = map(pwm_value, 799, 2201, 0, (ARRAY_SIZE(gPatterns)));
    currentPattern = map(pwm_value, 1600, 4400, 0, (ARRAY_SIZE(gPatterns)));
    patternHistory.unshift(map(pwm_value, 1600, 4400, 0, (ARRAY_SIZE(gPatterns))));
  }

  /*    if ((pwm_value < 2300) && (pwm_value > 750))
      {
        //pwm_value
        //currentPattern = map(pwm_value, 799, 2201, 0, (ARRAY_SIZE(gPatterns)));
        if ((prevPattern != map(pwm_value, 799, 2201, 0, (ARRAY_SIZE(gPatterns)))) && (prevPattern != prev2Pattern)){
          currentPattern = prev2Pattern;
          prev2Pattern = prevPattern;
          prevPattern = map(pwm_value, 799, 2201, 0, (ARRAY_SIZE(gPatterns)));
        }
        currentPattern = map(pwm_value, 799, 2201, 0, (ARRAY_SIZE(gPatterns)));
        prev2Pattern = prevPattern;
        prevPattern = currentPattern;
      }*/

  //prevPattern = currentPattern;

  //TCNT1  = 1;//initialize counter value to 0, reset heatbeat

  prev_time = 0;
  inPulse = false;
}

// showAnalogRGB: this is like FastLED.show(), but outputs on
// analog PWM output pins instead of sending data to an intelligent,
// pixel-addressable LED strip.
//
// This function takes the incoming RGB values and outputs the values
// on three analog PWM output pins to the r, g, and b values respectively.
void showAnalogRGB( const CRGB& rgb)
{
  analogWrite(REDPIN,   rgb.r );
  analogWrite(GREENPIN, rgb.g );
  analogWrite(BLUEPIN,  rgb.b );
}

void testPattern()
{
  if (addressableStrip == true) {
    // Fill --. with primary and secondary color from Pot Values
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
  }
  else {
    // Long blink Primary and short Blink Secondary, then crossfade, repeat?
    gHue = gHue + 1;
    // Use FastLED automatic HSV->RGB conversion
    showAnalogRGB( CHSV( gHue, 255, 255) );
    delay(5);
  }
}

void rainbow()
{
  if (addressableStrip == true) {
    // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
  }
  else {
    gHue = gHue + 1;
    // Use FastLED automatic HSV->RGB conversion
    showAnalogRGB( CHSV( gHue, 255, 255) );
    delay(5);
  }
}

void rainbowWithGlitter()
{
  if (addressableStrip == true) {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    addGlitter(80, CRGB::White);
  }
  else {
    gHue = gHue + 1;
    // Use FastLED automatic HSV->RGB conversion
    showAnalogRGB( CHSV( gHue, 255, 255) );
    delay(15);
  }
}

void addGlitter( fract8 chanceOfGlitter, CRGB gColor)
{
  if (addressableStrip == true) {
    if ( random8() < chanceOfGlitter) {
      //leds[ random16(NUM_LEDS) ] += CRGB::White;
      leds[ random16(NUM_LEDS) ] = gColor;
    }
  }
  else {

  }
}

void confetti()
{
  if (addressableStrip == true) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( gHue + random8(64), 200, 255);
  }
  else {
    //flash bright rand colors
  }

}

void sinelon()
{
  if (addressableStrip == true) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
    leds[pos] += CHSV( gHue, 255, 192);
  }
  else {

  }

}

void bpm()
{
  if (addressableStrip == true) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
  }
  else {

  }
}

void juggle() {
  if (addressableStrip == true) {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for ( int i = 0; i < 8; i++) {
      leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }
  else {

  }

}


#define SPARKING 100
#define COOLING  65
bool gReverseDirection = false;
void Fire2012()
{

  if (addressableStrip == true) {
    // Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for ( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for ( int k = NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if ( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160, 255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for ( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if ( gReverseDirection ) {
        pixelnumber = (NUM_LEDS - 1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
  }
  else {
    //flash firey pulses
  }
}


void teamSparkle() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Indigo );
    addGlitter(500, CRGB::Gold);

    //      for(int dot = 0; dot < NUM_LEDS; dot++) {
    //          leds[dot] = CRGB::Blue;
    //          FastLED.show();
    //          // clear this led for the next time around the loop
    //          leds[dot] = CRGB::Gold;
    //          delay(30);
    //      }
  }
  else {
    showAnalogRGB( CRGB::HotPink );
  }
}

void HotPink() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::HotPink );
  }
  else {
    showAnalogRGB( CRGB::HotPink );
  }
}

void Red() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Red );
  }
  else {
    showAnalogRGB( CRGB::Red );
  }
}

void DarkOrange() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::DarkOrange );
  }
  else {
    showAnalogRGB( CRGB::DarkOrange );
  }
}

void Yellow() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Yellow );
  }
  else {
    showAnalogRGB( CRGB::Yellow );
  }
}

void Gold() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Gold );
  }
  else {
    showAnalogRGB( CRGB::Gold );
  }
}

void Lime() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Lime );
  }
  else {
    showAnalogRGB( CRGB::Lime );
  }
}

void Green() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Green );
  }
  else {
    showAnalogRGB( CRGB::Green );
  }
}

void Aqua() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Aqua );
  }
  else {
    showAnalogRGB( CRGB::Aqua );
  }
}

void Blue() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Blue );
  }
  else {
    showAnalogRGB( CRGB::Blue );
  }
}

void BlueViolet() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::BlueViolet );
  }
  else {
    showAnalogRGB( CRGB::BlueViolet );
  }
}

void White() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::White );
  }
  else {
    showAnalogRGB( CRGB::White );
  }
}

void Black() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Black );
  }
  else {
    showAnalogRGB( CRGB::Black );
  }
}





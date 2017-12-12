#include "BlinkinFirmware.h"

void setup() {

  //Intialize ring buffer

  addressableStrip = EEPROM.read(0);
  COLOR1 = EEPROM.read(1);
  COLOR2 = EEPROM.read(2);
  //NUM_LEDS = 240;//EEPROM.read(3);

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

//List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  Black,
  rainbow,
  rainbowWithGlitter,
  confetti,
  sinelon,
  EndtoEndBlend,
  EndtoEndStaticBlend,
  juggle,
  bpm,
  Fire2012,
  teamSparkle,
  HotPink,
  Red,
  DarkOrange,
  Yellow,
  Gold,
  Lime,
  Green,
  Aqua,
  Blue,
  BlueViolet,
  White
};


void loop() {

  buttonHandler();
  if (inSetup) {
    readUserInputs();
  }

  if ((inPulse == false) && (updatedLEDs == false)) {
    ledUpdate();
    startIndex = startIndex + 1; /* motion speed */
  }

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
    //FastLED.setBrightness(map(analogRead(LENGTH_PIN), 0, 1023, 10, 255));
  }
}


ISR(TIMER1_COMPA_vect) {

  patternHistory.unshift(14);

  updatedLEDs = false;
  inPulse = false;

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
}

void ISRrising() {
  TCNT1  = 1;//initialize counter value to 0, reset heatbeat
  prev_time = TCNT1;
  inPulse = true;
  updatedLEDs = false;

  attachInterrupt(digitalPinToInterrupt(2), ISRfalling, FALLING);
}

void ISRfalling() {
  detachInterrupt(digitalPinToInterrupt(2));
  pwm_value = TCNT1 - prev_time;

  if ((pwm_value < 4800) && (pwm_value > 1200))
  {
    patternHistory.unshift(map(pwm_value, 1600, 4400, 0, (ARRAY_SIZE(gPatterns))));
  }

  prev_time = 0;
  inPulse = false;
}

void ledUpdate()
{
  //check that the pattern value has been stable
  for (int i = 0 ; i < patternHistory.capacity() ; i++) {
    if (patternHistory[0] != patternHistory[i])
      patternStable = false;
  }

  if (patternStable) {
    gPatterns[patternHistory[0]]();
  }
  else {
    gPatterns[patternHistory.last()];
  }

  patternStable = true;

  // send the 'leds' array out to the actual LED strip
  //30uS per LED for addressable
  if (addressableStrip == true) {
    //update LED display
    FastLED.show();
  }

  attachInterrupt(digitalPinToInterrupt(2), ISRrising, RISING);
  updatedLEDs = true;
}











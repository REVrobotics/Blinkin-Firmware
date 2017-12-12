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




void rainbow()
{
  if (addressableStrip == true) {

    uint8_t colorIndex = startIndex;
    currentBlending = NOBLEND;
    
    // FastLED's built-in rainbow generator
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
        colorIndex += 3;
    }
  }
  else {
    gHue = gHue + 1;
    // Use FastLED automatic HSV->RGB conversion
    showAnalogRGB( CHSV( gHue, 255, 255) );
    delay(5);
  }
}

//
//void rainbow()
//{
//  if (addressableStrip == true) {
//    // FastLED's built-in rainbow generator
//    fill_rainbow( leds, NUM_LEDS, gHue, 7);
//  }
//  else {
//    gHue = gHue + 1;
//    // Use FastLED automatic HSV->RGB conversion
//    showAnalogRGB( CHSV( gHue, 255, 255) );
//    delay(5);
//  }
//}

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
  // a colored dot sweeping 
  // back and forth, with 
  // fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  static int prevpos = 0;
  if( pos < prevpos ) { 
    fill_solid( leds+pos, (prevpos-pos)+1, CHSV(gHue,220,255));
  } else { 
    fill_solid( leds+prevpos, (pos-prevpos)+1, CHSV( gHue,220,255));
  }
  prevpos = pos;
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


#define SPARKING 120
#define COOLING  80
bool gReverseDirection = false;
void Fire2012()
{

  if (addressableStrip == true) {
    // Array of temperature readings at each simulation cell
    static byte heat[240];

    
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



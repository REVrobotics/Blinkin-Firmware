void HotPink() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::HotPink );
  }
  else {
    showAnalogRGB( CRGB::HotPink );
    showAnalogRGB( CHSV( 250 , 255, map(analogRead(LENGTH_PIN), 0, 1023, 50, 255)) );
  }
}

void Red() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Red );
  }
  else {
    //showAnalogRGB( CRGB::Red );
    showAnalogRGB( CHSV( 0 , 255, map(analogRead(LENGTH_PIN), 0, 1023, 50, 255)) );
  }
}

void DarkOrange() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::DarkOrange );
  }
  else {
    //showAnalogRGB( CRGB::DarkOrange );
    showAnalogRGB( CHSV( 15 , 255, map(analogRead(LENGTH_PIN), 0, 1023, 50, 255)) );
  }
}

void Gold() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Gold );
  }
  else {
    //showAnalogRGB( CRGB::Gold );
    showAnalogRGB( CHSV( 25 , 255, map(analogRead(LENGTH_PIN), 0, 1023, 50, 255)) );
  }
}

void Yellow() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, CRGB::Yellow );
  }
  else {
    //showAnalogRGB( CRGB::Yellow );
    showAnalogRGB( CHSV( 40 , 255, map(analogRead(LENGTH_PIN), 0, 1023, 50, 255)) );
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

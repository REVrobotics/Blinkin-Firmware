
void rainbow_Team(){rainbow(teamPalette);}
void bpm_Team(){bpm(teamPalette);}


void EndtoEndBlend() {
  uint8_t speed = beatsin8(6,0,255);
  endclr = blend(colorList[COLOR1], colorList[COLOR2], speed);
  midclr = blend(colorList[COLOR2], colorList[COLOR1], speed);

  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS/2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, midclr, NUM_LEDS-1, endclr);
}

void EndtoEndStaticBlend() {
  fill_gradient_RGB(leds, 0, colorList[COLOR1], NUM_LEDS/2, colorList[COLOR2]);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, colorList[COLOR2], NUM_LEDS-1, colorList[COLOR1]);
}

void teamSparkle() {
  if (addressableStrip == true) {
    fill_solid( leds, NUM_LEDS, colorList[COLOR1] );
    addGlitter(255, colorList[COLOR2]);
  }
  else {
    showAnalogRGB( colorList[COLOR1] );
  }
}

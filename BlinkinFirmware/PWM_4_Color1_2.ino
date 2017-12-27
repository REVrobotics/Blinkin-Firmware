void colorwavess_Team() {   colorwaves(teamPalette);    }
void drawTwinkles_Team(){drawTwinkles(teamPalette);}
void rainbow_Team(){rainbow(teamPalette);}
void bpm_Team(){bpm(teamPalette);}


void EndtoEndBlend() {
  uint8_t speed = beatsin8(6,0,255);
  CRGB endclr = blend(colorList[COLOR1], colorList[COLOR2], speed);
  CRGB midclr = blend(colorList[COLOR2], colorList[COLOR1], speed);
 
  fill_gradient_RGB(leds, 0, endclr, stripLength/2, midclr);
  fill_gradient_RGB(leds, stripLength/2+1, midclr, stripLength-1, endclr);
}

void EndtoEndStaticBlend() {
  fill_gradient_RGB(leds, 0, colorList[COLOR1], stripLength/2, colorList[COLOR2]);
  fill_gradient_RGB(leds, stripLength/2+1, colorList[COLOR2], stripLength-1, colorList[COLOR1]);

}

void teamSparkle() {
  if (addressableStrip == true) {
    fill_solid( leds, stripLength, colorList[COLOR1] );
    addGlitter(255, colorList[COLOR2]);
  }
  else {
    showAnalogRGB( colorList[COLOR1] );
  }
}

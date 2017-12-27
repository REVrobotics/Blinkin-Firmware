void EndtoEndBlend_2() {
  uint8_t speed = beatsin8(6,0,255);
  CRGB endclr = blend(BLACK, colorList[COLOR2], speed);
  CRGB midclr = blend(colorList[COLOR2], BLACK, speed);

  fill_gradient_RGB(leds, 0, endclr, stripLength/2, midclr);
  fill_gradient_RGB(leds, stripLength/2+1, midclr, stripLength-1, endclr);
}

void EndtoEndStaticBlend_2() {
  fill_gradient_RGB(leds, 0, BLACK, stripLength/2, colorList[COLOR2]);
  fill_gradient_RGB(leds, stripLength/2+1, colorList[COLOR2], stripLength-1, BLACK);

}

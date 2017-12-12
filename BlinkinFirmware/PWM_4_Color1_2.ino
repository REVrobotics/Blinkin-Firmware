void EndtoEndBlend() {
  uint8_t speed = beatsin8(6,0,255);
  endclr = blend(CRGB::Aqua, CRGB::Orange, speed);
  midclr = blend(CRGB::Orange, CRGB::Aqua, speed);

  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS/2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, midclr, NUM_LEDS-1, endclr);

}

void EndtoEndStaticBlend() {
  //uint8_t speed = beatsin8(6,0,255);
  //endclr = blend(CRGB::Aqua, CRGB::Orange, speed);
  //midclr = blend(CRGB::Orange, CRGB::Aqua, speed);

  fill_gradient_RGB(leds, 0, CRGB::Aqua, NUM_LEDS/2, CRGB::Orange);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, CRGB::Orange, NUM_LEDS-1, CRGB::Aqua);

}

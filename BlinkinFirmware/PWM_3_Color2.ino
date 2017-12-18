void EndtoEndBlend_2() {
  uint8_t speed = beatsin8(6,0,255);
  endclr = blend(BLACK, colorList[COLOR2], speed);
  midclr = blend(colorList[COLOR2], BLACK, speed);

  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS/2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, midclr, NUM_LEDS-1, endclr);
}

void EndtoEndStaticBlend_2() {
  fill_gradient_RGB(leds, 0, BLACK, NUM_LEDS/2, colorList[COLOR2]);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, colorList[COLOR2], NUM_LEDS-1, BLACK);

}

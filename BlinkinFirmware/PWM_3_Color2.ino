void larsonScanner_Team2()  {   larsonScanner(colorList[COLOR2]);  }
void lightChase_Team2() { lightChase(colorList[COLOR2]); }
void heartBeat_Team2s() {  heartBeat(colorList[COLOR2], 1500); }
void heartBeat_Team2m() {  heartBeat(colorList[COLOR2], 1000); }
void heartBeat_Team2f() {  heartBeat(colorList[COLOR2], 800); }
void breath_Team2s()     { breath(colorList[COLOR2], 1); }
void breath_Team2f()     { breath(colorList[COLOR2], 3); }

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

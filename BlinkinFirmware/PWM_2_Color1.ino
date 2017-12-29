void larsonScanner_Team1()  {   larsonScanner(colorList[COLOR1]);  }
void lightChase_Team1() { lightChase(colorList[COLOR1]); }
void heartBeat_Team1s() {  heartBeat(colorList[COLOR1], 1500); }
void heartBeat_Team1m() {  heartBeat(colorList[COLOR1], 1000); }
void heartBeat_Team1f() {  heartBeat(colorList[COLOR1], 800); }
void breath_Team1s()     { breath(colorList[COLOR1], 1); }
void breath_Team1f()     { breath(colorList[COLOR1], 3); }


void EndtoEndBlend_1() {
  uint8_t speed = beatsin8(6,0,255);
  CRGB endclr = blend(BLACK, colorList[COLOR1], speed);
  CRGB midclr = blend(colorList[COLOR1], BLACK, speed);



  fill_gradient_RGB(leds, 0, endclr, stripLength/2, midclr);
  fill_gradient_RGB(leds, stripLength/2+1, midclr, stripLength-1, endclr);
}

void EndtoEndStaticBlend_1() {
  fill_gradient_RGB(leds, 0, BLACK, stripLength/2, colorList[COLOR1]);
  fill_gradient_RGB(leds, stripLength/2+1, colorList[COLOR1], stripLength-1, BLACK);
  
}

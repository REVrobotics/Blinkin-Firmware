void setupMode()
{
  inSetup = !inSetup;

  if (inSetup){
      setStatusSetup();
  }
  //Exit set-up
  else{
      setStatusRun();

    //write variable to EEPROM
    if(writeEEPROM)
    {
      EEPROM.write(1, COLOR1);  
      EEPROM.write(2, COLOR2);  
      EEPROM.write(3, NUM_LEDS);  
    }
      
    //resume normal operation
  }

}

void readUserInputs()
{
  //read Pot value and translate to colors/strip Length

//  lengthHistory.unshift(map(analogRead(LENGTH_PIN), 0, 1024, 1, 240));
//
//    //check that the pattern value has been stable 
//    for (int i = 0 ; i< lengthHistory.capacity() ; i++){
//      if (lengthHistory[0] != lengthHistory[i])
//        lengthStable = false;
//    }
//    
//    if (lengthStable){
//      if (NUM_LEDS > lengthHistory[0]){
//         //need loop to only update pixels to black that are > then new length
//         Black();
//         FastLED.show();
//      }
//      //NUM_LEDS = lengthHistory[0];
//      //FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);           
//    }     
//
//    lengthStable = true;


    color1History.unshift(map(analogRead(COLOR1_PIN), 0, 1024, 0, (ARRAY_SIZE(colorList))));
    
    //check that the pattern value has been stable 
    for (int i = 0 ; i< color1History.capacity() ; i++){
      if (color1History[0] != color1History[i])
        lengthStable = false;
    }
    
    if (color1Stable){
      COLOR1 = color1History[0];         
    }   

    color1Stable = true;

    color2History.unshift(map(analogRead(COLOR2_PIN), 0, 1024, 0, (ARRAY_SIZE(colorList))));

    //check that the pattern value has been stable 
    for (int i = 0 ; i< color2History.capacity() ; i++){
      if (color2History[0] != color2History[i])
        lengthStable = false;
    }
    
    if (color2Stable){
      COLOR2 = color2History[0];         
    }   

    SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);
    //SetupCustomPalette(hueList[COLOR1], hueList[COLOR2]);
    color2Stable = true;

  
}

void buttonHandler()
{
  

  if(digitalRead(SS_PIN) == LOW)
  {
    ssButtonHoldCount++;
  }
  else
  {
    if (ssButtonHoldCount > 1000){
      ssButtonHoldCount = 0;
      //SetupCustomPalette(colorList[1], colorList[6]);
      toggleStripSelect();
    }
    ssButtonHoldCount = 0;
  }


  if(digitalRead(MODE_PIN) == LOW)
  {
    modeButtonHoldCount++;
    if (inSetup){
      if (modeButtonHoldCount > 5000){
        modeButtonHoldCount = 0;
        setupMode();
      }
    }
    else{
      if (modeButtonHoldCount > 100000){
        modeButtonHoldCount = 0;
        setupMode();
      }
    }
  }
  else
  {
    modeButtonHoldCount = 0;
  }
}



//void SetupCustomPalette(uint8_t HUE_color1, uint8_t HUE_color2)
void SetupCustomPalette(CRGB color1, CRGB color2)
{
  teamPalette = CRGBPalette16(
                                 color2,  color1,  color1,  color1, 
                                 color1, color1, color1,  color1,
                                 color1,  color2,  color2,  color2,
                                 color2, color1, color1,  color2 );
}

void testPattern()
{
    //SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);
    //currentBlending = NOBLEND;
  
  if (addressableStrip == true) {

    uint8_t colorIndex = 1;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( teamPalette, colorIndex, BRIGHTNESS, currentBlending);
        colorIndex += 3; 
        //fill_rainbow( leds, NUM_LEDS, gHue, 7);
    }
  }
  else {
    // Long blink Primary and short Blink Secondary, then crossfade, repeat?
    gHue = gHue + 1;
    // Use FastLED automatic HSV->RGB conversion
    showAnalogRGB( CHSV( gHue, 255, 255) );
    delay(5);
  }
}

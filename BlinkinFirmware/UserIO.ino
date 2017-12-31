void setupMode()
{
  inSetup = !inSetup;

  if (inSetup) {
    // Update status LED color
    setStatusSetup();

    // Set the current display pattern to the test pattern (a pattern which shows the two team colors)
    noSignalPatternDisplay = TESTPATTERN;    
  }
  else {
    // Exit set-up
    // User exited setup

    COLOR1 = EEPROM.read(COLOR1_EE);
    COLOR2 = EEPROM.read(COLOR2_EE);
    stripLength = EEPROM.read(LED_EE);
    noSignalPatternDisplay = EEPROM.read(PATTERN_EE); 

    fill_solid( leds, NUM_LEDS, CRGB::Black );

    SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);
    
    // Update Status LED Color
    setStatusRun();  

    //resume normal operation
  }
}

void readUserInputs()
{
  //read Pot value and translate to colors/strip length

  uint8_t newLength = constrain(map(analogRead(LENGTH_PIN), 0, 1024, 1, 241), 0, 240);

  if ((stripLength > newLength) && (addressableStrip == true)){
    //need loop to only update pixels to black that are > then new length
    //for stripLength to lengthHistory[0]
    for( int i = NUM_LEDS-1; i >= newLength; i--) {
      leds[i] =  CRGB::Black;
    }
    FastLED.show();
  }
  stripLength = newLength;           

  COLOR1 = map(analogRead(COLOR1_PIN), 0, 1024, 0, (ARRAY_SIZE(colorList)));   
  COLOR2 = map(analogRead(COLOR2_PIN), 0, 1024, 0, (ARRAY_SIZE(colorList)));

  SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);
}

void buttonHandler()
{
  // If both buttons are pressed increment the both buttons pressed counter
  if((digitalRead(MODE_PIN) == LOW) && (digitalRead(SS_PIN) == LOW)) {
    programButtonHoldCount++;
  }

  // Strip Select Button Logic
  if(digitalRead(SS_PIN) == LOW) {
    ssButtonHoldCount++;
    
    if ((digitalRead(MODE_PIN) == HIGH) && (programButtonHoldCount == 0) && (inSetup == false)) {
      if (ssButtonHoldCount > 40) {
        ssButtonHoldCount = 0;
        toggleStripSelect();
        stripTransistion = true;
      }
    }
  }
  else {
  // SS_PIN == HIGH
    if ((ssButtonHoldCount > 5) && (programButtonHoldCount == 0) ) {
      if ((noSignal == true)) {
        if (stripTransistion == true) {
          stripTransistion = false;
        }
        else {
        //increment output pattern
        noSignalPatternDisplay++; //RAM need to add constrain
        }
      }
      ssButtonHoldCount = 0;
    }
  }

  // Mode Button Logic
  if(digitalRead(MODE_PIN) == LOW) {
    modeButtonHoldCount++;
    
    if ((digitalRead(SS_PIN) == HIGH) && (programButtonHoldCount == 0)) {
      if (modeButtonHoldCount > 130) {
        modeButtonHoldCount = 0;
        setupMode();
        setupTransistion = true;
      }
    }
  }
  else { 
    // MODE_PIN == HIGH
    if ((modeButtonHoldCount > 5) && (programButtonHoldCount == 0) ) {
      if ((noSignal == true)) {
        if (setupTransistion == true) {
          setupTransistion = false;
        }
        else {
        //increment output pattern
        noSignalPatternDisplay--;  //RAM need to add contrain
        }
      }
      modeButtonHoldCount = 0;
    }
  }

  // Both Buttons Pressed Logic
  if ((programButtonHoldCount > 110) && (inSetup == true)) {
    //setStatusEEPROM(); 
    setStatusError();
    
    //write EEPROM
    saveDefaults();
      
    setupMode();
    
    ssButtonHoldCount = 0;
    modeButtonHoldCount = 0;
  }

  // Reset all the counters if neither button is pressed
  if(digitalRead(MODE_PIN) == HIGH && digitalRead(SS_PIN) == HIGH) {
    programButtonHoldCount = 0;
    ssButtonHoldCount = 0;
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
  if (addressableStrip == true) {
    uint8_t colorIndex = 1;
    
    for( int i = 0; i < stripLength; i++) {
        leds[i] = ColorFromPalette( teamPalette, colorIndex, brightness, NOBLEND);
        colorIndex += 3; 
    }
  }
  else {    
    displaySolid( ColorFromPalette(teamPalette, gHue ));
    
    // Long blink Primary and short Blink Secondary, then crossfade, repeat?
//    gHue = gHue + 1;
//    // Use FastLED automatic HSV->RGB conversion
//    showAnalogRGB( CHSV( gHue, 255, 255) );
//    delay(5);
  }
}

void saveDefaults()
{  
  //EEPROM write takes 3.3ms
  if(writeEEPROM) {
    EEPROM.update(SS_EE, addressableStrip);
    EEPROM.update(COLOR1_EE, COLOR1);
    EEPROM.update(COLOR2_EE, COLOR2);
    EEPROM.update(LED_EE, stripLength);
    if (noSignalPatternDisplay != TESTPATTERN)
      EEPROM.update(PATTERN_EE, noSignalPatternDisplay);
  }  
}

void initEEPROM()
{
  if((digitalRead(MODE_PIN) == LOW) && (digitalRead(SS_PIN) == LOW)) {
    //EEPROM write takes 3.3ms
    if(writeEEPROM) {      
      EEPROM.write(SS_EE, addressableStrip);
      EEPROM.write(COLOR1_EE, COLOR1);
      EEPROM.write(COLOR2_EE, COLOR2);
      EEPROM.write(LED_EE, stripLength);
      EEPROM.write(PATTERN_EE, noSignalPatternDisplay);
    }
  }
  else {
    // Check if defaults have been stored in EEPROM
    if (EEPROM.read(COLOR1_EE) == 0xFF) {
      //defaults have not been saved, load variables into them

      //EEPROM write takes 3.3ms
      if(writeEEPROM) {      
        EEPROM.write(SS_EE, addressableStrip);
        EEPROM.write(COLOR1_EE, COLOR1);
        EEPROM.write(COLOR2_EE, COLOR2);
        EEPROM.write(LED_EE, stripLength);
        EEPROM.write(PATTERN_EE, noSignalPatternDisplay);
      }
    }
    else {
      //reload from saved values
      addressableStrip = EEPROM.read(SS_EE);
      COLOR1 = EEPROM.read(COLOR1_EE);
      COLOR2 = EEPROM.read(COLOR2_EE);
      stripLength = EEPROM.read(LED_EE);
      noSignalPatternDisplay = EEPROM.read(PATTERN_EE);
    }
  }
}

void setStatusCommand() {
  // "Purple"
  digitalWrite(sREDPIN, LOW);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, LOW);
}

void setStatusSetup() {
  // "Yellow"
  digitalWrite(sREDPIN, LOW);
  digitalWrite(sGREENPIN, LOW);
  digitalWrite(sBLUEPIN, HIGH);
}

void setStatusRun() {
  // "Blue"
  digitalWrite(sREDPIN, HIGH);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, LOW);
}

void setStatusMode() {

  if (inSetup == false) {
    setStatusRun();
  }   // "Blue"
  else {
    setStatusSetup();
  }  // "Yellow"
}


void setStatusNoSignal() 
{
  // "Blink Output"
  if (inSetup == false) {
    digitalWrite(sGREENPIN, HIGH);
    digitalWrite(sREDPIN, HIGH);

    if (digitalRead(sBLUEPIN) == HIGH) {
      digitalWrite(sBLUEPIN, LOW);
    }
    else {
      digitalWrite(sBLUEPIN, HIGH);
    }
  }
  else {
    digitalWrite(sBLUEPIN, HIGH);

    if (digitalRead(sGREENPIN) == HIGH) {
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, LOW);
    }
    else {
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, HIGH);
    }
  }
}

void setStatusError() 
{
  // "Red"
  digitalWrite(sREDPIN, LOW);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, HIGH);
}


void setStatusEEPROM() 
{
  // "cyan"
  digitalWrite(sREDPIN, HIGH);
  digitalWrite(sGREENPIN, LOW);
  digitalWrite(sBLUEPIN, LOW);
}


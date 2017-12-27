void cmdNoStrip(char var)             {  
  displaySolid(BLACK);  
  cmdDisableOutput = true;
}

void cmd5VStrip(char var)             {  setStripSelect(true); }

void cmd12VStrip(char var)            {  setStripSelect(false); }

void cmdIncreaseBrightness(char var)  {}

void cmdDecreaseBrightness(char var)  {}

void cmdChangeColor1(char var)        {  
  COLOR1 = constrain(map(var, 78, 99, 0, ARRAY_SIZE(colorList)), 0, 21); 
  SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);}

void cmdChangeColor2(char var)        {  
  COLOR2 = constrain(map(var, 78, 99, 0, ARRAY_SIZE(colorList)), 0, 21); 
  SetupCustomPalette(colorList[COLOR1], colorList[COLOR2]);
}

void cmdChangeDefaultPattern(char var){  noSignalPatternDisplay = var; }

void cmdSaveSettings(char var){
  
  //EEPROM write takes 3.3ms
//  if(writeEEPROM)
//  {
//      EEPROM.update(SS_EE, addressableStrip);
//      EEPROM.update(COLOR1_EE, COLOR1);
//      EEPROM.update(COLOR2_EE, COLOR2);
//      EEPROM.update(LED_EE, stripLength);
//      EEPROM.update(PATTERN_EE, noSignalPatternDisplay);
//  }

}

void cmdSetNoBlend(char var)          {  currentBlending = NOBLEND; }

void cmdSetLinearBlend(char var)      {  currentBlending = LINEARBLEND; }



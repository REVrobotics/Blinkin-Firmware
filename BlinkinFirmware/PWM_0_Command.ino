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
  //if(writeEEPROM)
  {
      //EEPROM.update(0, addressableStrip);
      //EEPROM.update(1, COLOR1);
      //EEPROM.update(2, COLOR2);
      //EEPROM.update(3, NUM_LEDS);
      //EEPROM.update(4, noSignalPatternDisplay);
  }

}

void cmdSetNoBlend(char var)          {  currentBlending = NOBLEND; }

void cmdSetLinearBlend(char var)      {  currentBlending = LINEARBLEND; }



void cmdNoStrip(char var)             {  
  displaySolid(BLACK);  
  cmdDisableOutput = true;
}

void cmd5VStrip(char var)             {  setStripSelect(true); }

void cmd12VStrip(char var)            {  setStripSelect(false); }

void cmdIncreaseBrightness(char var)  {}

void cmdDecreaseBrightness(char var)  {}

void cmdChangeColor1(char var)        {  COLOR1 = constrain(map(var, 78, 99, 0, ARRAY_SIZE(colorList)), 0, 21); }

void cmdChangeColor2(char var)        {  COLOR2 = constrain(map(var, 78, 99, 0, ARRAY_SIZE(colorList)), 0, 21); }

void cmdChangeDefaultPattern(char var){  noSignalPattern = var; }

void cmdSaveSettings(char var){
  
  //EEPROM write takes 3.3ms
  //if(writeEEPROM)
  {
      //EEPROM.write(0, addressableStrip);
      //EEPROM.write(1, COLOR1);
      //EEPROM.write(2, COLOR2);
      //EEPROM.write(3, NUM_LEDS);
      //EEPROM.write(4, noSignalPattern);
  }

}

void cmdSetNoBlend(char var)          {  currentBlending = NOBLEND; }

void cmdSetLinearBlend(char var)      {  currentBlending = LINEARBLEND; }



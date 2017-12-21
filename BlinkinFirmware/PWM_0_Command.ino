void cmdNoStrip(char var)    {  displaySolid(BLACK);  }

void cmd5VStrip(char var)    {  
    setStripSelect(true);  
  }

void cmd12VStrip(char var)   {  
    setStripSelect(false);  
  }

void cmdIncreaseBrightness(char var){
    
    if (var == 0)
      programSeq = true;
    else {
      
      programSeq = false;
    }
  }

void cmdDecreaseBrightness(char var){
    if (var == 0)
      programSeq = true;
    else {
      
      programSeq = false;
    }
  
  }

void cmdChangeColor1(char var){
  
      programSeq = false;
      
      COLOR1 = constrain(map(var, 78, 99, 0, ARRAY_SIZE(colorList)), 0, 21);

      digitalWrite(sREDPIN, HIGH);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sBLUEPIN, LOW);
  }

void cmdChangeColor2(char var){

      programSeq = false;
      
      COLOR2 = constrain(map(var, 78, 99, 0, ARRAY_SIZE(colorList)), 0, 21);

      digitalWrite(sREDPIN, HIGH);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sBLUEPIN, LOW);
  
  }

void cmdChangeDefaultPattern(char var){
    if (var == 0)
      programSeq = true;
    else {
      noSignalPattern = var;
      programSeq = false;
    }
  
  }

  void cmdSaveSettings(char var){
    programSeq = false;
    
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


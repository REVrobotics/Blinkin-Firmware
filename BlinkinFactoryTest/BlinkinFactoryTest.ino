#include <TimerOne.h>

#define TIMER_PERIOD  10000 //microseconds

#define LED_PIN     7 //Data line for addressable strip
#define IND_PIN     4 //Digital pin to indicate Analog vs Addressable LEDs Strip Select
#define MODE_PIN    A3 //Button 2 - Programming Mode Button
#define SS_PIN      A2 //LED Strip type select button

//Analog RGB LED Strip Pins
#define REDPIN   5
#define GREENPIN 6
#define BLUEPIN  3

////Analog Status RGB LED Pins
#define sREDPIN   9
#define sGREENPIN 10
#define sBLUEPIN  11

//Analog Inputs for Color Selections
#define COLOR1_PIN      A6 
#define COLOR2_PIN      A7 
#define LENGTH_PIN      A0

#define CLR_OFF         0
#define CLR_BLUE        1
#define CLR_GREEN       2
#define CLR_CYAN        3
#define CLR_RED         4
#define CLR_MAGENTA     5
#define CLR_YELLOW      6
#define CLR_WHITE       7


int sytemState = 0;

//   1 __     __
//   0   |___|
//     0   1   2

bool modePressed = false;
bool modePressRelease = false;
bool modeFalling = false;
int modeSequence = 0;


int modeState = 0;
int prevModeState = 0;
int modeHistory[3] = {1,1,1};
int modeHistoryCount = 0;

bool SSPressed = false;
bool SSPressRelease = false;
bool SSFalling = false;
int SSSequence = 0;

int SSState = 0;
int prevSSState = 0;
int SSHistory[3] = {1,1,1};
int SSHistoryCount = 0;

bool statusLEDBlink = true;
long statusLEDBlinkPeriod = 50;
int statusLEDColor = 0;
long statusLEDCounter = 0;
bool statusLEDToggle = false; 

void setup() {

  //Button pins
  pinMode(MODE_PIN, INPUT_PULLUP);
  pinMode(SS_PIN, INPUT_PULLUP);

  //RGB Strip pins
  pinMode(REDPIN,   OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN,  OUTPUT);

  digitalWrite(REDPIN, LOW);
  digitalWrite(GREENPIN, LOW);
  digitalWrite(BLUEPIN, LOW);

  //Status LED Pins
  pinMode(sREDPIN,   OUTPUT);
  pinMode(sGREENPIN, OUTPUT);
  pinMode(sBLUEPIN,  OUTPUT);

  digitalWrite(sREDPIN, HIGH);
  digitalWrite(sGREENPIN, HIGH);
  digitalWrite(sBLUEPIN, HIGH);
  
  //TODO - Setup Addressable Strip stuff

  statusLEDColor = CLR_RED;
  statusLEDBlinkPeriod = 50;   //set status LED blink period
  statusLEDBlink = true;        //set status LED to blink
  
  Timer1.initialize(TIMER_PERIOD);
  Timer1.attachInterrupt(timerISR);
  Timer1.start();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(modePressRelease)
  {
    modePressRelease = false;
    statusLEDColor = CLR_BLUE;
  }

  if(SSPressRelease)
  {
    SSPressRelease = false;
    statusLEDColor = CLR_YELLOW;
  }

}

void setStatusLEDColor(int color)
{
  switch(color)
  {
    case CLR_OFF:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_BLUE:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_GREEN:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_CYAN:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, HIGH);
      break;
    case CLR_RED:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, LOW);
      break;
    case CLR_MAGENTA:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, HIGH);
      digitalWrite(sREDPIN, LOW);
      break;
    case CLR_YELLOW:
      digitalWrite(sBLUEPIN, HIGH);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, LOW);
      break;
    case CLR_WHITE:
      digitalWrite(sBLUEPIN, LOW);
      digitalWrite(sGREENPIN, LOW);
      digitalWrite(sREDPIN, LOW);
      break;
    default:
      break;
  }
}

void statusLEDHandler()
{
  if(statusLEDBlink)
  {
    if(statusLEDCounter % statusLEDBlinkPeriod == 0)
    {
      statusLEDCounter = 0;
      if(statusLEDToggle)
      {
        statusLEDToggle = false;
      }
      else
      {
        statusLEDToggle = true;
      }
    }
    else
    {
      statusLEDCounter++;
    }
  }
  else
  {
    statusLEDCounter = 0;
    statusLEDToggle = true;
  }

  if(statusLEDToggle)
  {
    setStatusLEDColor(statusLEDColor);
  }
  else
  {
    setStatusLEDColor(CLR_OFF);
  }
    
}

void buttonHandler()
{
  int modeSum = 0;
  int SSSum = 0;
  
  modeHistory[modeHistoryCount] = digitalRead(MODE_PIN);
  SSHistory[SSHistoryCount] = digitalRead(SS_PIN);

  modeHistoryCount++;
  if(modeHistoryCount > 2)
  {
    modeHistoryCount = 0;
  }

  for(int i = 0; i > 2; i++)
  {
    modeSum += modeHistory[i];
  }

  SSHistoryCount++;
  if(SSHistoryCount > 2)
  {
    SSHistoryCount = 0;
  }

  for(int i = 0; i > 2; i++)
  {
    SSSum += SSHistory[i];
  }

  if(modeSum == 0)
  {
    prevModeState = modeState;
    modeState = 0;
  }
  else if(modeSum == 3)
  {
    prevModeState = modeState;
    modeState = 1;
  }
  
  if(SSSum == 0)
  {
    prevSSState = SSState;
    SSState = 0;
  }
  else if(SSSum == 3)
  {
    prevSSState = SSState;
    SSState = 1;
  }

  switch(modeSequence)
  {
    case 0:
      // button is not pressed
      modePressed = false;
      if(modeState < prevModeState) //falling edge
      {
        modeSequence = 1;
      }
      break;
    case 1:
      //button is currently pressed
      modePressed = true;
      if(modeState > prevModeState) //rising edge
      {
        modeSequence = 2;
      }
      break;
    case 2:
      //button has been pressed and released
      modePressed = false;
      modePressRelease = true; //this will be cleared upon checking if there was a press and release.
      modeSequence = 0;
      break;
    default:
      break;
  }

  switch(SSSequence)
  {
    case 0:
      // button is not pressed
      SSPressed = false;
      if(SSState < prevSSState) //falling edge
      {
        SSSequence = 1;
      }
      break;
    case 1:
      //button is currently pressed
      SSPressed = true;
      if(SSState > prevSSState) //rising edge
      {
        SSSequence = 2;
      }
      break;
    case 2:
      //button has been pressed and released
      SSPressed = false;
      SSPressRelease = true; //this will be cleared upon checking if there was a press and release.
      SSSequence = 0;
      break;
    default:
      break;
  }
}

void timerISR()
{
  buttonHandler();
  statusLEDHandler();
}


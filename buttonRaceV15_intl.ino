/*
 * 20220630, added button animations for winning player, replaced delays with millis in step_finalScores
 * 20220708, added scrolling startup animation
 */

#include <stdio.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int maxScore;
int player;
int button_values[] = {916, 431, 269, 180, 109};
int btn_tol = 30;
int analogValue_p1 = 0;
int analogValue_p2 = 0;
int pin_p1 = A0;
int pin_p2 = A6;
int gameMode = 12;
int leds_cnt = 5;
int p1_leds[5] = {2,3,4,5,6};
int p2_leds[5] = {A2,A3,A4,A5,A1};
int LEDatButtonAtPlayer;
int LEDvalueAtPlayer1;
int LEDvalueAtPlayer2;
int buttonPressedByPlayer1;
int buttonPressedByPlayer2;
int scorePlayer1 = 0;
int scorePlayer2 = 0;
int LEDState = HIGH;
int m = 0;
int n = 0;
int showText = 0;

bool turnButtonsOff = true;
bool step_startupAnimation = true;
bool step_newRound = false;
bool step_showScores = false;
bool step_balancingPlayer1 = false;
bool step_balancingPlayer2 = false;
bool step_finalScores = false;
bool step_buttonAnimation = false;
bool valueHasChangedForPlayer1 = true;
bool valueHasChangedForPlayer2 = true;
bool wrongButtonPlayer1 = false;
bool wrongButtonPlayer2 = false;
bool xPlayer1 = false;
bool xPlayer2 = false;
bool yPlayer1 = false;
bool yPlayer2 = false;
bool kidMode = true;
bool penaltyServedPlayer1 = false;
bool penaltyServedPlayer2 = false;
bool FinalScoresShown = false;
bool startupAnimationDone = false;
bool step1 = false;
bool step2 = false;

unsigned long xPreviousMillisPlayer1 = 0; 
unsigned long xPreviousMillisPlayer2 = 0; 
unsigned long xCurrentMillisPlayer1;
unsigned long xCurrentMillisPlayer2;
const long xInterval = 500; 

unsigned long yPreviousMillisPlayer1 = 0; 
unsigned long yPreviousMillisPlayer2 = 0; 
unsigned long yCurrentMillisPlayer1;
unsigned long yCurrentMillisPlayer2;
const long yInterval = 500; 

const long onDuration = 75;
const long offDuration = 250;
long currentTime = 0;

const long onDurationFinalScores = 750;
const long offDurationFinalScores = 750;
long currentTimeFinalScores = 0;


// ----------------------------------------------------------------

void setup() {
  
//Serial.begin(9600);

  randomSeed(analogRead(A7));
  
  pinMode(pin_p1, INPUT);
  pinMode(pin_p2, INPUT);
  pinMode(gameMode, INPUT);

  for (int i = 0; leds_cnt > i; i++) {
    pinMode(p1_leds[i], OUTPUT);
  }
  for (int i = 0; leds_cnt > i; i++) {
    pinMode(p2_leds[i], OUTPUT);
  }  

  P.begin(2);
  P.setZone(0,0,3);
  P.setZone(1,4,7);
//P.setSpeed(50);
  
  if (digitalRead(gameMode) == 0)  {
    maxScore = 10;
    P.setSpeed(60); // the higher the number, the slower is the srolling speed
    kidMode = true;
  }
  else {
    maxScore = 100;
    P.setSpeed(25); // the lower the number, the faster is the scrolling speed
    kidMode = false;
  }

}

// ----------------------------------------------------------------

void loop() {

  if ( valueHasChangedForPlayer1 == true && scorePlayer1 > 0) {
   xPlayer1 = true;
  }

  if (xPlayer1 == true)  {
    xCurrentMillisPlayer1 = millis();
    if (xCurrentMillisPlayer1 - xPreviousMillisPlayer1 >= xInterval) { 
      xPreviousMillisPlayer1 = xCurrentMillisPlayer1;           
      valueHasChangedForPlayer1 = false;
      xPlayer1 = false;
    }
  }

if ( valueHasChangedForPlayer2 == true && scorePlayer2 > 0) {
   xPlayer2 = true;
  }

  if (xPlayer2 == true)  {
    xCurrentMillisPlayer2 = millis();
    if (xCurrentMillisPlayer2 - xPreviousMillisPlayer2 >= xInterval) { 
      xPreviousMillisPlayer2 = xCurrentMillisPlayer2;           
      valueHasChangedForPlayer2 = false;
      xPlayer2 = false;
    }
  }

// ----------------------------------------------------------------

  if  (step_startupAnimation) {
  
    if  (P.displayAnimate())  {
      if  (step2 == true)  {
      step1 = true;
        }
    
    if (startupAnimationDone == false) {
      startupAnimation();
      startupAnimationDone = true;
      step2 = true;
    }    
  }
  if  (step1 == true && step2 == true)  {
    step_newRound = true;
    }
  }

// ----------------------------------------------------------------
 
  if (step_newRound) {
    
    step_startupAnimation = false;
    
    step_newRound = false;
    
    step_showScores = true;
    
    LEDvalueAtPlayer1 = 0;
    LEDvalueAtPlayer2 = 0;

    digitalWrite(p1_leds[LEDvalueAtPlayer1], HIGH);
    digitalWrite(p2_leds[LEDvalueAtPlayer2], HIGH);
  
    step_balancingPlayer1 = true;   
    step_balancingPlayer2 = true;
    
  }

// ----------------------------------------------------------------

  if (step_showScores) {
      showScores();
      }

// ----------------------------------------------------------------

  if (step_balancingPlayer1) {
    buttonPressedByPlayer1 = get_pressedButtonNo(1);
    
    if (buttonPressedByPlayer1 != LEDvalueAtPlayer1 && buttonPressedByPlayer1 != 99)  {
      if (valueHasChangedForPlayer1 == false && scorePlayer1 > 0 ) {
        wrongButtonPlayer1 = true;
      }
    }
    else if (buttonPressedByPlayer1 == LEDvalueAtPlayer1 && step_finalScores == false) {
        wrongButtonPlayer1 = false;
        digitalWrite(p1_leds[LEDvalueAtPlayer1], LOW);
        wrongButtonPlayer1 = false;
        scorePlayer1++;
        valueHasChangedForPlayer1 = true;
        LEDvalueAtPlayer1 = randomiseButtons(1);
        xPreviousMillisPlayer1 = millis();
        penaltyServedPlayer1 = false;
    }
  }

// ---------------------------------      

  if (step_balancingPlayer2) {
    buttonPressedByPlayer2 = get_pressedButtonNo(2);

    if (buttonPressedByPlayer2 != LEDvalueAtPlayer2 && buttonPressedByPlayer2 != 99)  {
        if (valueHasChangedForPlayer2 == false && scorePlayer2 > 0 ) {
          wrongButtonPlayer2 = true;
        }
    }
    else if (buttonPressedByPlayer2 == LEDvalueAtPlayer2 && step_finalScores == false) {
        digitalWrite(p2_leds[LEDvalueAtPlayer2], LOW);
        wrongButtonPlayer2 = false;
        scorePlayer2++;
        valueHasChangedForPlayer2 = true;
        LEDvalueAtPlayer2 = randomiseButtons(2);
        xPreviousMillisPlayer2 = millis();
        penaltyServedPlayer2 = false;
    }
  }

// ----------------------------------------------------------------
    
if  (scorePlayer1 >= maxScore || scorePlayer2 >= maxScore)  {
    step_finalScores = true;
    step_buttonAnimation = true;

    step_newRound = false;
    step_showScores = false;
    step_balancingPlayer1 = false;
    step_balancingPlayer2 = false;
    }

// ----------------------------------------------------------------

if (step_buttonAnimation) {
  if (scorePlayer1 > scorePlayer2) {
    buttonAnimation(1);    
  }
  else  {
    buttonAnimation(2);
  }
}

// ----------------------------------------------------------------

if (step_finalScores == true) {
  if (showText == 1) {
    if ( (millis()- currentTimeFinalScores) >= onDurationFinalScores )  {   
      showText = 0;
      currentTimeFinalScores = millis();
  
      if (scorePlayer1 > scorePlayer2) {
        P.displayZoneText(0, "Winner", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        } else {
        P.displayZoneText(1, "Winner", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      }
      P.displayAnimate();  
    }
  }
  else if (showText == 0)  {
    if ( (millis()- currentTimeFinalScores) >= offDurationFinalScores)  {     
      showText = 1;
      currentTimeFinalScores = millis();
      
      char Score1[80];
      sprintf(Score1, "%d", scorePlayer1);
      char *chrdisp[] = {Score1};
  
      char Score2[80];
      sprintf(Score2, "%d", scorePlayer2);
      char *chrdisp2[] = {Score2};

      P.displayZoneText(0, chrdisp[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      P.displayZoneText(1, chrdisp2[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);    
      P.displayAnimate();
    }
  }
}

// ----------------------------------------------------------------

if (step_buttonAnimation) {
  if (turnButtonsOff == true) {
    digitalWrite(p1_leds[LEDvalueAtPlayer1], LOW);
    digitalWrite(p2_leds[LEDvalueAtPlayer2], LOW);
    turnButtonsOff = false;
  }   
}

// ----------------------------------------------------------------

}

void showScores() {

  if (wrongButtonPlayer1 == false) {
    char Score1[80];
    sprintf(Score1, "%d", scorePlayer1);
    char *chrdisp[] = {Score1};
    P.displayZoneText(0, chrdisp[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    P.displayAnimate();
  }
  else if (wrongButtonPlayer1 == true && kidMode == false)  {
  P.displayZoneText(0, "-1", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  penaltyPlayer1();
  yPlayer1 = true;
  }

  if (yPlayer1 == true)  {
    yCurrentMillisPlayer1 = millis();
    if (yCurrentMillisPlayer1 - yPreviousMillisPlayer1 >= yInterval) { 
      yPreviousMillisPlayer1 = yCurrentMillisPlayer1;           
      P.displayClear(0);
      wrongButtonPlayer1 = false;
      yPlayer1 = false;
      penaltyServedPlayer1 == false;
    }
  }

// ---------------------------------
  
  if (wrongButtonPlayer2 == false) {
    char Score2[80];
    sprintf(Score2, "%d", scorePlayer2);
    char *chrdisp2[] = {Score2};
    P.displayZoneText(1, chrdisp2[0], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    P.displayAnimate();
  }
  else if (wrongButtonPlayer2 == true && kidMode == false)  {
  P.displayZoneText(1, "-1", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();
  penaltyPlayer2(); 
  yPlayer2 = true;
  }

  if (yPlayer2 == true)  {
    yCurrentMillisPlayer2 = millis();
    if (yCurrentMillisPlayer2 - yPreviousMillisPlayer2 >= yInterval) { 
      yPreviousMillisPlayer2 = yCurrentMillisPlayer2;           
      P.displayClear(1);
      wrongButtonPlayer2 = false;
      yPlayer2 = false;
      penaltyServedPlayer2 == false;
      Serial.println("Penalty player 2 RESET DONE");
    }
  }

}

// ----------------------------------------------------------------

int randomiseButtons (int player) {
  LEDatButtonAtPlayer = random(0,5);
  if (player == 1) {
    while (LEDvalueAtPlayer1 == LEDatButtonAtPlayer)  {
      LEDatButtonAtPlayer = random(0,5);  
      }
    digitalWrite(p1_leds[LEDatButtonAtPlayer], HIGH);
  }
  if (player == 2) {
    while (LEDvalueAtPlayer2 == LEDatButtonAtPlayer)  {
      LEDatButtonAtPlayer = random(0,5);  
      }
    digitalWrite(p2_leds[LEDatButtonAtPlayer], HIGH);
  }
  return LEDatButtonAtPlayer;
}

// ----------------------------------------------------------------

int get_pressedButtonNo (int player) {

  int analogPin;
  int analogValue;

  if (player == 1) {
    analogPin = A0;
    }
  if (player == 2) {
    analogPin = A6;
    }
      
  analogValue = analogRead(analogPin);

  int buttonValue = 99;
  for (int n=0; n < 5; n++) {
    if ( analogValue < button_values[n] + btn_tol and analogValue > button_values[n] - btn_tol )  {
      buttonValue = n;
    }
  }
  return buttonValue;   

}

// ----------------------------------------------------------------

void penaltyPlayer1() {
  if (penaltyServedPlayer1 == false)  {
    scorePlayer1--;
    penaltyServedPlayer1 = true;   
    Serial.println("Penalty player 1 served");
  }
}

// ---------------------------------

void penaltyPlayer2() {
  if (penaltyServedPlayer2 == false)  {
    scorePlayer2--;
    penaltyServedPlayer2 = true;   
    Serial.println("Penalty player 2 served");
  }
}

// ----------------------------------------------------------------

void buttonAnimation (int player) {
  if( LEDState ==HIGH )
   {
      if ( (millis()- currentTime) >= onDuration )  {   
      LEDState = LOW;
      currentTime = millis();
      }
   }
   else {   
      while (n == m)  {
        n = random(0,5);
      }
      if ( (millis()- currentTime) >= offDuration)  {     
      LEDState =HIGH;
      currentTime=millis();
      m = n;
      }
   }
    if (player == 1) {
      digitalWrite(p1_leds[m], LEDState);
    }
    if (player == 2) {
     digitalWrite(p2_leds[m], LEDState);
    }
}

// ----------------------------------------------------------------

void startupAnimation(void)  {


  if (kidMode == true) {
    if (P.getZoneStatus(0) && P.getZoneStatus(1)) {
      P.displayZoneText(0, "Kid 1, ready, GO!", PA_CENTER, P.getSpeed(), P.getPause(), PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayZoneText(1, "Kid 2, ready, GO!", PA_CENTER, P.getSpeed(), P.getPause(), PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayReset(0);
      P.displayReset(1);
      P.synchZoneStart();
    }
  }
  else if (kidMode == false)  {
    if (P.getZoneStatus(0) && P.getZoneStatus(1)) {
      P.displayZoneText(0, "Player 1, ready, GO!", PA_CENTER, P.getSpeed(), P.getPause(), PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayZoneText(1, "Player 2, ready, GO!", PA_CENTER, P.getSpeed(), P.getPause(), PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayReset(0);
      P.displayReset(1);
      P.synchZoneStart();
    }
  }
}

// ----------------------------------------------------------------

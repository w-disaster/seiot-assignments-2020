/*
 * PARTECIPANTI DEL GRUPPO
 * - Luigi Olivieri
 * - Luca Fabri
 */
 
#define EI_ARDUINO_INTERRUPTED_PIN
#include "game_lib.h"
#include <EnableInterrupt.h>


long tMin;
/* red led variables */
int brightness;
int verse;

/* the lit up led */
int pinOffset;
int score;
bool isPlaying;
bool missedLed;
bool canStart;

void setup() {
  
  score = 0;
  isPlaying = false;
  brightness = 255;
  verse = -1;
  pinOffset = 0;
  missedLed = false;
  canStart = true;

  pinMode(RED_LED, OUTPUT);
  for(int i = 0; i <= LED_MAX - LED_MIN; i++){
    pinMode(LED_MIN + i, OUTPUT);
  }
  
  for(int i = 0; i <= BUTTON_MAX - BUTTON_MIN; i++){
    pinMode(BUTTON_MIN + i, INPUT);
    enableInterrupt(BUTTON_MIN + i, buttonPressed, RISING);
  } 

  /* inizializzo il generatore di numeri pseudocasuali su un pin disconnesso in modo tale 
  *  che dal momento che ArduinoUNO si connette la sequenza è sempre diversa.
  */
  randomSeed(analogRead(A1));

  Serial.begin(9600);
  Serial.println("Track to Led Fly Game. Press Key T1 to Start\n");
  
  MiniTimer1.init();
}

void loop() {
  if(!isPlaying && !missedLed){
    noInterrupts();
    fadeRedLed();
    interrupts();
  } else if(missedLed){
    noInterrupts();
    canStart = false;
    interrupts();
    
    analogWrite(RED_LED, brightness);
    delay(2 * MILLIS_TO_SECONDS);
    missedLed = false;
    
    noInterrupts();
    canStart = true;
    interrupts();
  }
  delay(10);
}

void buttonPressed(){
  if(!isPlaying){
    if(arduinoInterruptedPin == BUTTON_MIN){
      startGame();
    }
  }
  else if(isPlaying && arduinoInterruptedPin == BUTTON_MIN + pinOffset){
    MiniTimer1.stop();
    digitalWrite(LED_MIN + pinOffset, LOW);
    score++;
    nextPinOffset();

    digitalWrite(LED_MIN + pinOffset, HIGH);
    tMin = tMin * REDUCING_FACTOR;
    Serial.println(String("good: ") + tMin);

    MiniTimer1.setPeriod(random(tMin, tMin * K));
    MiniTimer1.start();
  }
  else if(isPlaying) {
    missedLed = true;
    gameOver();
  }
}

void fadeRedLed(){
    analogWrite(RED_LED, brightness);
 
    brightness = brightness + verse * FADE_STEP;
    if(brightness == 255 || brightness == 0){
      verse = -(verse);
    }
}


void nextPinOffset(){
  int dir = random(0,2);
  int next;
  
  /* muovo la fly a destra o sinistra randoimicamente */
  noInterrupts();
  next = dir ? pinOffset + 1  : pinOffset - 1;
  interrupts();

  /* overflow control */
  if(next > BUTTON_MAX - BUTTON_MIN) next = 0;
  if(next < 0) next = BUTTON_MAX-BUTTON_MIN;

  noInterrupts();
  pinOffset = next;
  interrupts();
}

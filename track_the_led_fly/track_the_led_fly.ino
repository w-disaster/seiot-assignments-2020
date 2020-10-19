/*
 * PARTECIPANTI DEL GRUPPO
 * - Luigi Olivieri
 * - Luca Fabri
 */

#define EI_ARDUINO_INTERRUPTED_PIN

#include <EnableInterrupt.h>
#include "game_lib.h"

int pin[LED_MAX - LED_MIN];

int tMin;

int brightness;
int verse;

int flyPosition;

int score;
int isHigh;

bool isOver;
bool isPlaying;

void setup() {
  score = 0;
  isPlaying = false;
  brightness = 255;
  verse = -1;
  pinMode(RED_LED, OUTPUT);
  
  isHigh = 0;

  for(int i = 0; i <= LED_MAX - LED_MIN; i++){
    pinMode(LED_MIN + i, OUTPUT);
  }
  
  for(int i = 0; i <= BUTTON_MAX - BUTTON_MIN; i++){
    pinMode(BUTTON_MIN + i, INPUT);
    enableInterrupt(BUTTON_MIN + i, gameOver, RISING);
  } 

  /* inizializzo il generatore di numeri pseudocasuali su un pin disconnesso in modo tale 
  *  che dal momento che ArduinoUNO si connette la sequenza è sempre diversa.
  */
  randomSeed(analogRead(A1));
  
  Serial.begin(9600);
  /* da documentazione solo con il pulsante T1 si può iniziare a giocare */
  Serial.println("Track to Led Fly Game. Press Key T1 to Start \n");
}

void moveFly(){
  noInterrupts();
  
  int nextStep = random(0,1);
  /* muovo la fly a destra o sinistra randoimicamente */
  nextStep ? flyPosition++ : flyPosition--;

  /* overflow control */
  if(flyPosition > BUTTON_MAX-BUTTON_MIN) flyPosition = 0;
  if(flyPosition < 0) flyPosition = BUTTON_MAX-BUTTON_MIN;

 interrupts(); 
}

void loop() {
  if(isPlaying){
    
    isOver = true;
    /* disabilitiamo l'interrupt poichè se il bottone viene premuto è quello giusto */
    disableInterrupt(BUTTON_MIN + flyPosition);
    digitalWrite(LED_MIN + flyPosition, HIGH);

    /* numero random che rappresenta il tempo disponibile per premere il pulsante */ 
    long rndTimer = random(tMin, tMin*K);
    Serial.println(rndTimer);
    unsigned long tstart = millis();
    while(millis() - tstart < rndTimer){
      /* leggiamo lo stato del pulsante finchè la disuguaglianza è soddisfatta */
      int buttonState = digitalRead(BUTTON_MIN + flyPosition);
      /* se è ad HIGH e non è già stato premuto, l'ho premuto in tempo */
      if(buttonState == HIGH && isHigh == 0){
        isHigh = 1;
        /* la partita non è finita e quindi aumentiamo l'offset di TMIN */
        isOver = false;
        Serial.print("Tracking the fly: pos ");
        Serial.println(flyPosition + 1);
        score++;
      } else if(buttonState == LOW){
        isHigh = 0;
      }
    }
   

  /* se è ancora a true il pulsante non è stato premuto */
  if(isOver == true){
   
    /* chiamo il game over */
    gameOver();
 
    
  } else {
    isOver = true;
  }
  
  digitalWrite(LED_MIN + flyPosition, LOW);
  /* riabilitiamo l'interrupt del pulsante del led corrispondente appena spento */
  enableInterrupt(BUTTON_MIN + flyPosition, gameOver, RISING);

   moveFly();
  /* every round tMin decreases */
  tMin = tMin*REDUCING_FACTOR;
  delay(1000);

} else {
      analogWrite(RED_LED, brightness);
      delay(10);
      
      brightness = brightness + verse*FADE_STEP;
     
      if(brightness == 255 || brightness == 0){
        verse = -(verse);
      }
   }
}

/* se un pulsante tra quelli che non autorizzati vengono premuti o il pulsante corrente non è 
 *  stato premuto in tempo.
 */
void gameOver(){
  if(isPlaying){


    /* spengo il led acceso appena perdo */
    digitalWrite(LED_MIN + flyPosition, LOW);
  
    
    Serial.println("GAME OVER!");
    
    /* reset default values */
    brightness = 255;
    verse = -1;
    isPlaying = false;

    /* prints the score then resets it */
    Serial.print("Final score: ");
    Serial.println(score);
    score = 0;

    analogWrite(RED_LED, brightness);
    delay(2000);
  }else if (arduinoInterruptedPin == BUTTON_MIN ){  
    /* this is a routine to do once the game starts */


    /* turn red led off */
    analogWrite(RED_LED, 0);
    Serial.print("Go!");
    isPlaying = true;

    
    /* pulsante random tra il 2,3,4,5 */
    flyPosition = random(BUTTON_MAX - BUTTON_MIN + 1);

    /* 
     * maps the diffuclty level  
     * lvl 1 = 8 seconds
     * lvl 8 = 1 second
     */
    tMin = (8 - map(analogRead(POT), 0, 1023, 1, 8)) * 1000;
    Serial.print("difficulty: ");
    Serial.println(tMin);
  }
}

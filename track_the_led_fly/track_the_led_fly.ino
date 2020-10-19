/*
 * PARTECIPANTI DEL GRUPPO
 * - Luigi Olivieri
 * - Luca Fabri
 */

#define EI_ARDUINO_INTERRUPTED_PIN

#include <EnableInterrupt.h>
#include "game_lib.h"

int pin[PIN_MAX - PIN_MIN];

int tMin;

int brightness;
int verse;

int score;
int isHigh;
int offset;
bool isOver;
bool isPlaying;

void setup() {
  score = 0;
  isPlaying = false;
  brightness = 255;
  verse = -1;
  pinMode(RED_LED, OUTPUT);
  
  isHigh = 0;
  offset = 0;

  for(int i = 0; i <= PIN_MAX - PIN_MIN; i++){
    pinMode(PIN_MIN + i, OUTPUT);
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

void loop() {
  if(isPlaying){
    
    isOver = true;
    /* pulsante random tra il 2,3,4,5 */
    int rnd = random(BUTTON_MAX - BUTTON_MIN + 1);
    /* disabilitiamo l'interrupt poichè se il bottone viene premuto è quello giusto */
    disableInterrupt(BUTTON_MIN + rnd);
    digitalWrite(PIN_MIN + rnd, HIGH);

    /* numero random che rappresenta il tempo disponibile per premere il pulsante */ 
    long rndTimer = random(tMin - offset, tMin*K);
    Serial.println(rndTimer);
    unsigned long tstart = millis();
    while(millis() - tstart < rndTimer){
      /* leggiamo lo stato del pulsante finchè la disuguaglianza è soddisfatta */
      int buttonState = digitalRead(BUTTON_MIN + rnd);
      /* se è ad HIGH e non è già stato premuto, l'ho premuto in tempo */
      if(buttonState == HIGH && isHigh == 0){
        isHigh = 1;
        /* la partita non è finita e quindi aumentiamo l'offset di TMIN */
        isOver = false;
        offset = offset + 10;
        Serial.print("Tracking the fly: pos ");
        Serial.println(rnd + 1);
        score++;
      } else if(buttonState == LOW){
        isHigh = 0;
      }
    }
   

  /* se è ancora a true il pulsante non è stato premuto */
  if(isOver == true){
    
    offset = 0;
    
    /* spengo il led acceso appena perdo */
    digitalWrite(PIN_MIN + rnd, LOW);

    
    /* chiamo il game over */
    gameOver();
  
    /* tengo acceso il led rosso per 2 sec*/
    analogWrite(RED_LED, brightness);
    delay(2000);
    
  } else {
    isOver = true;
  }
  
  digitalWrite(PIN_MIN + rnd, LOW);
  /* riabilitiamo l'interrupt del pulsante del led corrispondente appena spento */
  enableInterrupt(BUTTON_MIN + rnd, gameOver, RISING);

  /* every round tMin decreases */
  tMin = tMin/REDUCING_FACTOR;
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
    
    Serial.println("GAME OVER!");
    
    /* reset default values */
    brightness = 255;
    verse = -1;
    isPlaying = false;

    /* prints the score then resets it */
    Serial.print("Final score: ");
    Serial.println(score);
    score = 0;
    
  }else if (arduinoInterruptedPin == BUTTON_MIN ){  
    /* this is a routine to do once the game starts */
  
    analogWrite(RED_LED, 0);
    Serial.print("Go!");
    isPlaying = true;

    /* 
     * maps the diffuclty level  
     * lvl 1 = 8 seconds
     * lvl 8 = 1 second
     */
    tMin = (8 - map(analogRead(POT), 0, 1023, 0, 8)) * 1000;
    Serial.println(tMin);
  }
}

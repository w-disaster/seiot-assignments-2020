#include <EnableInterrupt.h>

#define PIN_MIN 10
#define PIN_MAX 13
#define BUTTON_MIN 2
#define BUTTON_MAX 5

#define RED_LED 6
#define FADE_STEP 5

#define TMIN 250
#define TMAX 1500

int pin[PIN_MAX - PIN_MIN];

int brightness;
int verse;

int isHigh;
int offset;
bool isOver;
bool isPlaying;

void setup() {
  
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
  Serial.println("PRESS A BUTTON TO START!");
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
    long rndTimer = random(TMIN - offset, TMAX);
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
        Serial.println("bravo!");
      } else if(buttonState == LOW){
        isHigh = 0;
      }
    }
   

  /* se è ancora a true il pulsante non è stato premuto */
  if(isOver == true){
    offset = 0;
    gameOver();
  } else {
    isOver = true;
  }
  
  digitalWrite(PIN_MIN + rnd, LOW);
  /* riabilitiamo l'interrupt del pulsante del led corrispondente appena spento */
  enableInterrupt(BUTTON_MIN + rnd, gameOver, RISING);
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
    brightness = 255;
    verse = -1;
    isPlaying = false;
  
  }else{
    Serial.print("GO!");
    analogWrite(RED_LED, 0);
    isPlaying = true;
  }
}

#include "async_fsm.h"
#include "Led.h"
#include "MsgServiceBT.h"
#include "MsgService.h"
#include "SerialMsgRecv.h"
#include "RiverData.h"
#include "State.h"
#include "ServoMotorImpl.h"
#include "TimerOne.h"
#include "ControlData.h"

#define SERVO_MOTOR_PIN 12
#define L_DO_BOUND 0
#define U_DO_BOUND 100
#define RX 2
#define TX 3
#define T_BLINK 200000

/*
 * @author: Luca Fabri
 */

class MyAsyncFSM : public AsyncFSM {
  public:
    MyAsyncFSM(MsgServiceBT* btChannel, ServoMotor* servoMotor){
      this->state = NORMAL;
      this->btChannel = btChannel;
      this->servoMotor = servoMotor;
      Led.switchOff();
      btChannel->registerObserver(this);
      MsgService.registerObserver(this);
      Timer1.initialize(T_BLINK);
    }

    /* function to minimize repetitions */
    void sendMsgAndMoveServo(RiverData* riverData, Event* ev){
      this->btChannel->sendMsg(ev->getMsg());
      if(riverData->containsDamOpening()){
        int angle = map(riverData->getDamOpening(), L_DO_BOUND, U_DO_BOUND, 0, 180);
        this->servoMotor->setPosition(angle); 
      }
    }
  
    void handleEvent(Event* ev) {
      State currentState = state;
      switch (this->state) {
        case NORMAL:  
        case PRE_ALARM:
          /* If msg comes from DS:
            - Switch the led off
            - Forward the msg to DM
            - Move the servo if DamMode == AUTO
          */
          if (ev->getType() == SERIAL_MSG_RECV){
            RiverData* riverData = new RiverData(ev->getMsg());
            sendMsgAndMoveServo(riverData, ev);
            currentState = riverData->getRiverState();
            
            /* If current state == ALARM => Timer1.attachInterrupt */
            if(currentState == ALARM){
              Timer1.attachInterrupt(Led.blink);
            }
            delete(riverData); 

          /* If a message comes from DM and the state isn't ALARM
            then it can only be a json request for the last sampled data
            registered: forward it to DS
          */
          } else {
            MsgService.sendMsg(ev->getMsg());
          }
          break; 
      case ALARM: 
        if (ev->getType() == SERIAL_MSG_RECV){
          RiverData* riverData = new RiverData(ev->getMsg());
          sendMsgAndMoveServo(riverData, ev);
          currentState = riverData->getRiverState();
          
          /* If current state != ALARM => Timer1.detachInterrupt && Led.switchOff */
          if(currentState != ALARM){
            Timer1.detachInterrupt();
            Led.switchOff();
          }
          delete(riverData);
        } else {
          /* If msg comes from DM:
            - Move servo if dam opening is set, switch the led on
            - Forward the msg to DS
          */
          ControlData* controlData = new ControlData(ev->getMsg());
          if(controlData->containsDamOpening()){
            Timer1.detachInterrupt();
            Led.switchOn();
            int angle = map(controlData->getDamOpening(), L_DO_BOUND, U_DO_BOUND, 0, 180);
            this->servoMotor->setPosition(angle);
          }
          MsgService.sendMsg(ev->getMsg());
          delete(controlData);
        }
        break;
      }
      this->state = currentState;
    }

  private:
    MsgServiceBT* btChannel;
    ServoMotor* servoMotor;
    State state;
};

MyAsyncFSM* myAsyncFSM;
MsgServiceBT* btChannel;

void setup() {
  btChannel = new MsgServiceBT(RX, TX);
  btChannel->init();
  
  /* Serial MsgService */
  MsgService.init();

  /* Servo motor */
  ServoMotor* servoMotor = new ServoMotorImpl(SERVO_MOTOR_PIN);
  servoMotor->on();

  /* Async FSM */
  myAsyncFSM = new MyAsyncFSM(btChannel, servoMotor);
}

void loop() {
  myAsyncFSM->checkEvents();

  /* As SerialEvent function is called at the end of loop(), 
    check if there is available data from SoftwareSerial and, if so, generate the event
  */
  btChannel->btEvent();
}

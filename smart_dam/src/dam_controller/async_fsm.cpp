#include "async_fsm.h"
#include "Arduino.h"

#define MAX_EVQUEUE_SIZE 20 

/* ------------------------ Event ------------------------ */

Event::Event(int type, String msg){
  this->type = type;
  this->msg = msg;
} 
  
int Event::getType(){
  return this->type;  
}

String Event::getMsg(){
  return this->msg;
}

/* --------------------- EventSource ------------------- */

void EventSource::generateEvent(Event* ev) {
  if (observer != NULL){
    observer->notifyEvent(ev);  
  }
}

void EventSource:: registerObserver(Observer* observer){
  this->observer = observer;
}

/* ------------------------ EventQueue ------------------------ */

EventQueue::EventQueue(){
  head = tail = 0; 
}

bool EventQueue::isEmpty() {
  return head == tail; 
}

void EventQueue::enqueue(Event* ev){
  queue[tail] = ev;
  tail = (tail+1) % MAX_EVQUEUE_SIZE;
}

Event* EventQueue::dequeue(){
    if (isEmpty()){
      return 0;
    } else {
      Event* pev = queue[head];
      head = (head+1) % MAX_EVQUEUE_SIZE;
      return pev; 
    }
}

/* ------------------------ AsyncFSM ------------------------ */

AsyncFSM::AsyncFSM(){}
    
void AsyncFSM::notifyEvent(Event* ev){
  cli();
  eventQueue.enqueue(ev);
  sei();
}

void AsyncFSM::checkEvents(){
    noInterrupts();
    bool isEmpty = eventQueue.isEmpty();
    interrupts();

    if (!isEmpty){
      noInterrupts();
      Event* ev = eventQueue.dequeue();
      interrupts();

      if (ev != NULL){
        handleEvent(ev);
        delete ev;
      }
    }    
}
#include "SchedulerImpl.h"
#include "Arduino.h"

SchedulerImpl::SchedulerImpl(){
    this->setState(State::STATE::READY);
}

void SchedulerImpl::schedule(){
    
}

void SchedulerImpl::setState(State::STATE state){
    this->state = state;
}

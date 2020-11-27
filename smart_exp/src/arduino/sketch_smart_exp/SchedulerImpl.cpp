#include "SchedulerImpl.h"
#include "ReadyAgent.h"
#include "BlinkTask.h"
#include "Arduino.h"

#define L1PIN 12
#define L2PIN 13
#define ERROR_TIME 5
#define MILLIS_TO_SEC 1000

SchedulerImpl::SchedulerImpl()
{
    this->state = State::STATE::READY;
    this->agents[0] = new ReadyAgent(3, 2);

    // creiamo i due led e li aggiungiamo
    Led *L1 = new Led(L1PIN);
    Led *L2 = new Led(L2PIN);
    this->agents[0]->setLeds(L1, L2);

    // creiamo il task che fa blinking sul secondo led
<<<<<<< HEAD
    Task* blinkTask = new BlinkTask(L2);
=======
    Task *blinkTask = new BlinkingTask(L2);
>>>>>>> b520fa29b30271aef8863cdea11a0a152aa351d1
    // settiamo il suo periodo
    blinkTask->init(200);

    // aggiungiamo il task all'agent
    this->agents[0]->setTask(blinkTask);
    // settiamo il periodo dell'agent
    this->agents[0]->init(ERROR_TIME * MILLIS_TO_SEC);

    // settiamo l'agent corrente
    setCurrentAgent(this->state);
}

void SchedulerImpl::init(int basePeriod)
{
    this->basePeriod = basePeriod;
    timer.setupPeriod(this->basePeriod);
}

void SchedulerImpl::schedule()
{
    long t_start = micros();
    if (this->current != NULL)
    {
        timer.waitForNextTick();
        if (this->current->getTask()->updateAndCheckTime(this->basePeriod))
        {
            this->current->getTask()->tick();
        }
        setCurrentAgent(this->current->updateTimeAndCheckEvent(this->basePeriod));
    }
    long end = micros() - t_start;
    Serial.println(end);
}

<<<<<<< HEAD
void SchedulerImpl::setCurrentAgent(State::STATE state){
    switch (state){
        case State::STATE::READY:
            this->current = this->agents[0];
            this->current->init(ERROR_TIME * MILLIS_TO_SEC);
            this->current->getTask()->init(200);
            break;
        case State::STATE::SUSPENDED:
        case State::STATE::ERROR:
        case State::STATE::EXPERIMENTATION:
            this->current = NULL;
            break;
=======
void SchedulerImpl::setCurrentAgent(State::STATE state)
{
    switch (state)
    {
    case State::STATE::READY:
        this->current = this->agents[0];
        break;
    case State::STATE::SUSPENDED:
    case State::STATE::ERROR:
    case State::STATE::EXPERIMENTATION:
        this->current = NULL;
        break;
        z
>>>>>>> b520fa29b30271aef8863cdea11a0a152aa351d1
    }
}
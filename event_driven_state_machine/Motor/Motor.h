#ifndef _MOTOR_H
#define _MOTOR_H

#include "SM.h"

typedef enum motor_state{
    IDLE,
    START,
    CHANGE_SPEED,
    STOP,
    MOTOR_STATES_COUNT
}motor_state_t;

class Motor : public FStateMachine{
    private:

        Motor();

        unsigned int ST_MAX_STATES = motor_state::MOTOR_STATES_COUNT;
        
        STATE_DECLARE(Motor, Idle, FSMEvent)
        STATE_DECLARE(Motor, Start, FSMEvent)
        STATE_DECLARE(Motor, ChangeSpeed, FSMEvent)
        STATE_DECLARE(Motor, Stop, FSMEvent)

        BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&Idle)
        STATE_MAP_ENTRY(&Start)
        STATE_MAP_ENTRY(&ChangeSpeed)
        STATE_MAP_ENTRY(&Stop)
        END_STATE_MAP

    protected:

    public:
        void initialize();
        static Motor* get_instance();
};

#endif
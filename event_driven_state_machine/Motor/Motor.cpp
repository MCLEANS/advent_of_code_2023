#include "Motor.h"
#include "iostream"

Motor::Motor(): FStateMachine(motor_state::MOTOR_STATES_COUNT,
                                motor_state::IDLE,
                                "Motor State Machine"){

}

void Motor::initialize(){
    FStateMachine::init();
}

STATE_DEFINE(Motor, Idle, FSMEvent){
    switch(ev->sig){
        case FSM_ENTRY_SIGNAL:
            std::cout<<"Executing Idle Entry Signal"<<std::endl;
            break;
        case FSM_EXIT_SIGNAL:
            std::cout<<"Executing Idle Exit Signal"<<std::endl;
            break;
        case FSM_RUN_SIGNAL:
            FSM_TRANS(motor_state::START);
        default:
            break;
    };
}

STATE_DEFINE(Motor, Start, FSMEvent){
    switch(ev->sig){
        case FSM_ENTRY_SIGNAL:
            std::cout<<"Executing Start Entry Signal"<<std::endl;
            break;
        case FSM_EXIT_SIGNAL:
            std::cout<<"Executing Start Exit Signal"<<std::endl;
            break;
        case FSM_RUN_SIGNAL:
            FSM_TRANS(motor_state::CHANGE_SPEED);
        default:
            break;
    };
}

STATE_DEFINE(Motor, ChangeSpeed, FSMEvent){
    switch(ev->sig){
        case FSM_ENTRY_SIGNAL:
            std::cout<<"Executing ChangeSpeed Entry Signal"<<std::endl;
            break;
        case FSM_EXIT_SIGNAL:
            std::cout<<"Executing ChangeSpeed Exit Signal"<<std::endl;
            break;
        case FSM_RUN_SIGNAL:
            FSM_TRANS(motor_state::STOP);
        default:
            break;
    };
}

STATE_DEFINE(Motor, Stop, FSMEvent){
    switch(ev->sig){
        case FSM_ENTRY_SIGNAL:
            std::cout<<"Executing Stop Entry Signal"<<std::endl;
            break;
        case FSM_EXIT_SIGNAL:
            std::cout<<"Executing Stop Exit Signal"<<std::endl;
            break;
        case FSM_RUN_SIGNAL:
            FSM_TRANS(motor_state::IDLE);
        default:
            break;

    };
}

Motor* Motor::get_instance(){
    static Motor motor;
    return &motor;
}
#include "SM.h"
#include <iostream>

#define TRACE_GROUP "FSM"

FStateMachine::FStateMachine(uint8_t maxStates, uint8_t initialState, const char* name) :
    MAX_STATES(maxStates),
    m_initialState(initialState),
    m_currentState(initialState),
    m_newState(initialState),
    m_FSMName(name)
{


}

fsm_error_t FStateMachine::init()
{
    FSM_TRANS(m_initialState);
    m_initialized = false;
    return FSM_OK;
}

fsm_error_t FStateMachine::run()
{
    const StateMapRow* pStateMap = GetStateMap();
    assert(pStateMap != nullptr);
    StateEngine(pStateMap);
    return FSM_OK;
}


fsm_error_t FStateMachine::FSM_TRANS(uint8_t new_State)
{

    m_newState = new_State;
    return FSM_OK;

}

std::chrono::time_point<std::chrono::steady_clock> FStateMachine::GetStateElapsedTime()
{
    return std::chrono::steady_clock::now();//(std::chrono::steady_clock::now() - m_stateTimer);
}

//----------------------------------------------------------------------------
// StateEngine
//----------------------------------------------------------------------------
void FStateMachine::StateEngine(const StateMapRow* const pStateMap)
{

    // Get the pointer from the state map
    const FStateBase* state = pStateMap[m_currentState].State;
    assert(state != NULL);


    //check if theres a transition
    if (m_newState != m_currentState) {

        //tr_debug("[%s] TRANS %u -> %u", m_FSMName, m_currentState, m_newState);
        std::cout<<"["<<m_FSMName<<"] TRANSITION : "<< m_currentState <<" -> "<< m_newState <<std::endl;

        // Error check that the new state is valid before proceeding
        assert(m_newState < MAX_STATES);


        FSMEvent exit_event(FSM_EXIT_SIGNAL);

        state->InvokeStateAction(this, &exit_event);

        // Switch to the new current state
        SetCurrentState(m_newState);
        state = pStateMap[m_currentState].State;

        m_initialized =  false;
    }


    if (!m_initialized) {
        FSMEvent entry_event(FSM_ENTRY_SIGNAL);
        state->InvokeStateAction(this, &entry_event);
        m_stateTimer =  std::chrono::steady_clock::now();
        m_initialized =  true;
    }

    //@todo get events from event queue
    FSMEvent exec_event(FSM_RUN_SIGNAL);
    state->InvokeStateAction(this, &exec_event);

}
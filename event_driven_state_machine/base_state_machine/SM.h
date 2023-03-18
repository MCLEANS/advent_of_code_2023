#ifndef F_STATE_MACHINE_H
#define F_STATE_MACHINE_H

#include <chrono>
#include "assert.h"


typedef enum fsm_errors {
    FSM_OK,
    FSM_RUNNING,
    FSM_IDLE,
    FSM_DELAY,
    FSM_DONE,
    FSM_ERROR = 100,
    FSM_ST_OUT_OF_RANGE_ERROR,
    FSM_NO_MEMORY_ERROR,
    FSM_UNHANDLED_ERROR,
    FSM_UNRECOVERABLE_ERROR
} fsm_error_t;


/**
 * FSM base signals
 * For custom user signals then enum should start from @param FSM_USER_SIGNAL
*/
enum fsm_signals {

    FSM_ENTRY_SIGNAL = 0,
    FSM_INIT_SIGNAL,
    FSM_EXIT_SIGNAL,
    FSM_RUN_SIGNAL,
    FSM_USER_SIGNAL
};


/**
 * @brief Unique state machine event must inherit from this class.
 */
class FSMEvent {
public:
    /**
     * @param[in] signal - Event signal
    */
    FSMEvent(unsigned int signal): sig(signal) {}

    virtual ~FSMEvent() {}

    unsigned int sig;

};

typedef FSMEvent NoEventData;

class FStateMachine;

/**
 * @brief Abstract state base class that all states inherit from.
 */
class FStateBase {
public:

    /**
     * state handler function
     * If a guard condition exists and it evaluates to false, the state action will not execute.
     * @param[in] sm - A state machine instance.
     * @param[in] data - The event data.
     */
    virtual void InvokeStateAction(FStateMachine* sm, const FSMEvent* data) const = 0;
};


/**
 * current implementation of FStateBase
 * @param[in] SM -  State Machine class
 * @param[in] Data - event data type (derived from FSMEvent)
 * @param[in] Func - State machine member function that handles the state
*/
template <class SM, class Data, void (SM::*Func)(const Data*)>
class FState : public FStateBase {
public:

    virtual void InvokeStateAction(FStateMachine* sm, const FSMEvent* data) const
    {
        // Downcast the state machine and event data to the correct derived type
        SM* derivedSM = static_cast<SM*>(sm);

        // If this check fails, there is a mismatch between the STATE_DECLARE
        // event data type and the data type being sent to the state function.
        const Data* derivedData = dynamic_cast<const Data*>(data);
        assert(derivedData != NULL);

        // Call the state function
        (derivedSM->*Func)(derivedData);
    }
};

/**
 * @brief A structure to hold a single row within the state map.
 */
struct StateMapRow {
    const FStateBase* const State;
};

// ++++++++++++++++++++++++ EXTENDED FUNCTIONALITY +++++++++++++++++++++++++++++++++ //

/**
 * @brief Abstract guard base class that all guards classes inherit from.
 */
class GuardBase {
public:
    /**
     * Called by the state machine engine to execute a guard condition action.
     * If guard condition evaluates to TRUE the state action is executed. If FALSE, no state transition is performed.
     * @param[in] sm - A state machine instance.
     * @param[in] data - The event data.
     * @return Returns TRUE if no guard condition or the guard condition evaluates to TRUE.
     */
    virtual bool InvokeGuardCondition(FStateMachine* sm, const FSMEvent* data) const = 0;
};

/**
 * @brief GuardCondition takes three template arguments: A state machine class,
 * a state function event data type (derived from FSMEvent) and a state machine member function pointer.
 */
template <class SM, class Data, bool (SM::*Func)(const Data*)>
class GuardCondition : public GuardBase {
public:
    virtual bool InvokeGuardCondition(FStateMachine* sm, const FSMEvent* data) const
    {
        SM* derivedSM = static_cast<SM*>(sm);
        const Data* derivedData = dynamic_cast<const Data*>(data);
        assert(derivedData != NULL);

        // Call the guard function
        return (derivedSM->*Func)(derivedData);
    }
};

/**
 * @brief Abstract entry base class that all entry classes inherit from.
 */
class EntryBase {
public:
    /// Called by the state machine engine to execute a state entry action. Called when
    /// entering a state.
    /// @param[in] sm - A state machine instance.
    /// @param[in] data - The event data.
    virtual void InvokeEntryAction(FStateMachine* sm, const FSMEvent* data) const = 0;
};

/**
 * @brief EntryAction takes three template arguments: A state machine class,
 * a state function event data type (derived from FSMEvent) and a state machine
 * member function pointer.
 */
template <class SM, class Data, void (SM::*Func)(const Data*)>
class EntryAction : public EntryBase {
public:
    virtual void InvokeEntryAction(FStateMachine* sm, const FSMEvent* data) const
    {
        SM* derivedSM = static_cast<SM*>(sm);
        const Data* derivedData = dynamic_cast<const Data*>(data);
        assert(derivedData != NULL);

        // Call the entry function
        (derivedSM->*Func)(derivedData);
    }
};

/**
 * @brief Abstract exit base class that all exit classes inherit from.
 */
class ExitBase {
public:
    /// Called by the state machine engine to execute a state exit action. Called when
    /// leaving a state.
    /// @param[in] sm - A state machine instance.
    virtual void InvokeExitAction(FStateMachine* sm) const = 0;
};

/**
 * @brief ExitAction takes two template arguments: A state machine class and
 * a state machine member function pointer.
 */
template <class SM, void (SM::*Func)(void)>
class ExitAction : public ExitBase {
public:
    virtual void InvokeExitAction(FStateMachine* sm) const
    {
        SM* derivedSM = static_cast<SM*>(sm);

        // Call the exit function
        (derivedSM->*Func)();
    }
};


/**
 * @brief A structure to hold a single row within the extended state map.
 */
struct StateMapRowEx {
    const FStateBase* const State;
    const GuardBase* const Guard;
    const EntryBase* const Entry;
    const ExitBase* const Exit;
};



// ----------------------------- EXTENDED FUNCTIONALITY ----------------------------- //

/**
 * @brief FStateMachine implementation of State Machine
 */
class FStateMachine {
public:

    enum { INITIAL_FSM_STATE = 0xFF};

    /**
     * @param[in] maxStates - the maximum number of state machine states.
     * @param[in] initialState - defaults to zero
     * @param[in] name -  name of FSM class, helps with debugging
     */
    FStateMachine(uint8_t maxStates, uint8_t initialState = 0, const char* name = nullptr);

    virtual ~FStateMachine() {}

    /**
     * Gets the current state machine state.
     * @return Current state machine state.
     */
    uint8_t GetCurrentState()
    {
        return m_currentState;
    }

    /// Gets the maximum number of state machine states.
    /// @return The maximum state machine states.
    uint8_t GetMaxStates()
    {
        return MAX_STATES;
    }

    /**
     * transitions state machine to initial state
    */
    fsm_error_t init();

    /**
     * state machine run function, it executes the current state function
     * if no event is available it generates an FSM_RUN_SIGNAL to the current state function
     * this implementation is non blocking, executes only once, to use in a threaded tight loop
     * you will have to wrap this function in a tight while loop
    */
    fsm_error_t run();

protected:

    fsm_error_t FSM_TRANS(uint8_t new_State);

    std::chrono::time_point<std::chrono::steady_clock> GetStateElapsedTime();

private:
    /// The maximum number of state machine states.
    const uint8_t MAX_STATES;

    // initial state of State Machine
    uint8_t m_initialState;

    /// The current state machine state.
    uint8_t m_currentState;

    /// The new state the state machine has yet to transition to.
    uint8_t m_newState;

    const char* m_FSMName = "FSM";

    std::chrono::time_point<std::chrono::steady_clock> m_stateTimer = {};

    bool m_initialized = false;


    /// Gets the state map as defined in the derived class. The BEGIN_STATE_MAP,
    /// STATE_MAP_ENTRY and END_STATE_MAP macros are used to assist in creating the
    /// map.
    /// @return An array of StateMapRow pointers with the array size MAX_STATES
    virtual const StateMapRow* GetStateMap() = 0;


    /// Set a new current state.
    /// @param[in] newState - the new state.
    void SetCurrentState(uint8_t newState)
    {
        m_currentState = newState;
    }

    /// State machine engine
    void StateEngine(const StateMapRow* const pStateMap);

};

// HELPER MACROS

#define STATE_DECLARE(stateMachine, stateName, eventClass) \
    void ST_##stateName(const eventClass*); \
    FState<stateMachine, eventClass, &stateMachine::ST_##stateName> stateName;

#define STATE_DEFINE(stateMachine, stateName, eventClass) \
    void stateMachine::ST_##stateName(const eventClass* ev)


#define BEGIN_STATE_MAP \
    private:\
    virtual const StateMapRowEx* GetStateMapEx() { return NULL; }\
    virtual const StateMapRow* GetStateMap() {\
        static const StateMapRow STATE_MAP[] = {

#define STATE_MAP_ENTRY(stateName)\
    stateName,

#define END_STATE_MAP \
    }; \
    assert((sizeof(STATE_MAP)/sizeof(StateMapRow)) == ST_MAX_STATES); \
    return &STATE_MAP[0]; }


#endif // F_STATE_MACHINE_H
